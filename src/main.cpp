// Copyright (C) 2015 Leslie Zhai <xiang.zhai@i-soft.com.cn>

#include <unistd.h>
#include <signal.h>
#include <iostream>
#include <fstream>
#include <X11/Xutil.h>
#include <X11/extensions/XTest.h>

#include "mylistener.h"

static const float velcityFilter = 400.0;

static bool enableRecord = false;
static std::ofstream sampleFile;
static Display* display = nullptr;
static Window root = -1;
static bool quit = false;
static bool pressedCtrlF10 = false;
static float prePinchStrength = .0;
static bool grabbed = false;
static float preGrabStrength = .0;

static void sigHandler(int sig)
{
    if (sig == SIGINT) {
        std::cout << "Bye ;-)" << std::endl;
        quit = true;
    }
}

unsigned char* getWindowProperty(Window window, Atom atom, long *nitems, 
                                 Atom *type, int *size) 
{
    Atom actual_type;
    int actual_format;
    unsigned long _nitems;
    unsigned long bytes_after;
    unsigned char *prop;
    int status;

    status = XGetWindowProperty(display, window, atom, 0, (~0L),
                                False, AnyPropertyType, &actual_type,
                                &actual_format, &_nitems, &bytes_after,
                                &prop);
    if (status == BadWindow)
        return NULL;
    if (status != Success)
        return NULL;

    if (nitems != NULL)
        *nitems = _nitems;

    if (type != NULL)
        *type = actual_type;

    if (size != NULL)
        *size = actual_format;

    return prop;
}

static void getActiveWindow(Window *window_ret)
{
    Atom type;
    int size;
    long nitems;
    unsigned char *data;
    Atom request;

    request = XInternAtom(display, "_NET_ACTIVE_WINDOW", False);
    data = getWindowProperty(root, request, &nitems, &type, &size);
    if (nitems > 0)
        *window_ret = *((Window*)data); 
    
    if (data) {
        free(data);
        data = NULL;
    }
}

static void moveWindow(Window wid, int x, int y) 
{
    XWindowChanges wc;

    wc.x = x;
    wc.y = y;
    XConfigureWindow(display, wid, CWX | CWY, &wc);
    XFlush(display);
}

// 
// TODO: 
// Leap Motion palm axis 
// 手掌坐标 
// https://developer.leapmotion.com/documentation/cpp/api/Leap.Vector.html#cppstruct_leap_1_1_vector 
// 
// East and west is X axis, south and north is Z axis, up and downwards is Y axis 
// 东、西方向上是X轴，南、北是Z轴，上、下是Y轴
// 
// The range of X and Y axis might (I guess it is not from document) be [-100, 100] 
// 在X、Y轴的“经验”（尝试出来的，文档上貌似没有介绍？！）区间是[-100, 100] 
// 
// But when palm is in the edge, for example -100 
// 但是当手掌位于“边缘”比如-100时
// 
// Such Gesture::TYPE_KEY_TAP gesture is not accurate! so it needs to decrease 
// the range, for example [-60, 60] 
// 手势就不准了！所以需要把区间缩小！[-60, 60]
// 
// Matrix transform to current X11 screen resoluation 
// 坐标转换到当前分辨率下的X11“屏幕”坐标系
static void positionChanged(float x, float y, float z, int extendedFingers, 
                            Vector direction, Vector velcity) 
{
    int screen = 0;
    int screenWidth = DisplayWidth(display, screen);
    int screenHeight = DisplayHeight(display, screen);
    Window wid = 0;

    // Pitch is the angle between the negative z-axis and the projection of the 
    // vector onto the y-z plane. In other words, pitch represents rotation 
    // around the x-axis. If the vector points upward, the returned angle is 
    // between 0 and pi radians (180 degrees); if it points downward, the angle 
    // is between 0 and -pi radians.
    // 围绕着X轴，向上、下
    //std::cout << direction.pitch() << std::endl;
    
    // Yaw is the angle between the negative z-axis and the projection of the 
    // vector onto the x-z plane. In other words, yaw represents rotation around 
    // the y-axis. If the vector points to the right of the negative z-axis, 
    // then the returned angle is between 0 and pi radians (180 degrees); 
    // if it points to the left, the angle is between 0 and -pi radians.
    // 围绕着Y轴，在水平面上向左、右
    //std::cout << direction.yaw() << std::endl;

    // Roll is the angle between the y-axis and the projection of the vector 
    // onto the x-y plane. In other words, roll represents rotation around the 
    // z-axis. If the vector points to the left of the y-axis, then the returned 
    // angle is between 0 and pi radians (180 degrees); if it points to the 
    // right, the angle is between 0 and -pi radians.
    // 围绕着Z轴向左、右翻转
    //std::cout << direction.roll() << std::endl;

    if (velcity[0] > velcityFilter) {
        KeyCode leftKeyCode = XKeysymToKeycode(display, XK_Left);

        XTestFakeKeyEvent(display, leftKeyCode, True, 0);
        XTestFakeKeyEvent(display, leftKeyCode, False, 0);
        XFlush(display);
    } else if (velcity[0] < -velcityFilter) {
        KeyCode rightKeyCode = XKeysymToKeycode(display, XK_Right);

        XTestFakeKeyEvent(display, rightKeyCode, True, 0);
        XTestFakeKeyEvent(display, rightKeyCode, False, 0);
        XFlush(display);
    }

    // It is able to change the hot key code based on your desktop enviroment
    // such as KDE, GNOME, sort of ...
    // 你可以根据自己的桌面环境修改热键值
    if (velcity[2] < -velcityFilter && !pressedCtrlF10) {
        pressedCtrlF10 = true;
        KeyCode ctrlKeyCode = XKeysymToKeycode(display, XK_Control_L);
        KeyCode f10KeyCode = XKeysymToKeycode(display, XK_F10);
            
        XTestFakeKeyEvent(display, ctrlKeyCode, True, 0);
        XTestFakeKeyEvent(display, f10KeyCode, True, 0);
        XTestFakeKeyEvent(display, f10KeyCode, False, 0);
        XTestFakeKeyEvent(display, ctrlKeyCode, False, 0);
        XFlush(display);
    }

    // Move active window
    // 移动活跃窗体
    if (grabbed) {
        getActiveWindow(&wid);
        moveWindow(wid, (x + 60) * screenWidth / 120, (z + 60) * screenHeight / 120);
    }

    // Move mouse cursor 
    // 移动鼠标
    XSelectInput(display, root, KeyReleaseMask);
    XWarpPointer(display, None, root, 0, 0, 0, 0, (x + 60) * screenWidth / 120, 
                 (z + 60) * screenHeight / 120);
    XFlush(display);

    if (enableRecord) {
        sampleFile << x << " " << y << " " << z << "\n";
        sampleFile.flush();
    }
}

static void mouseClick(
    int button = Button1/* left button of mouse by default 默认鼠标左键 */) 
{
    XEvent event;
    
    memset(&event, 0, sizeof(event));
    
    event.type = ButtonPress;
    event.xbutton.button = button;
    event.xbutton.same_screen = True;
    
    XQueryPointer(display, root, &event.xbutton.root, &event.xbutton.window, 
                  &event.xbutton.x_root, &event.xbutton.y_root, 
                  &event.xbutton.x, &event.xbutton.y, &event.xbutton.state);
    
    event.xbutton.subwindow = event.xbutton.window;
    
    while (event.xbutton.subwindow) {
        event.xbutton.window = event.xbutton.subwindow;
        
        XQueryPointer(display, event.xbutton.window, &event.xbutton.root, 
                      &event.xbutton.subwindow, &event.xbutton.x_root, 
                      &event.xbutton.y_root, &event.xbutton.x, 
                      &event.xbutton.y, &event.xbutton.state);
    }
    
    XSendEvent(display, PointerWindow, True, 0xfff, &event);
    
    XFlush(display);
    
    usleep(100);
    
    event.type = ButtonRelease;
    event.xbutton.state = 0x100;
    
    XSendEvent(display, PointerWindow, True, 0xfff, &event);
    
    XFlush(display);
}

static void tapped() 
{
    pressedCtrlF10 = false;
    mouseClick();
}

static void pinch(float strength) 
{
    if (grabbed)
        return;
    
    KeyCode superKeyCode = XKeysymToKeycode(display, XK_Super_L);
    KeyCode plusKeyCode = XKeysymToKeycode(display, XK_plus);
    KeyCode minusKeyCode = XKeysymToKeycode(display, XK_minus);

    if (strength - prePinchStrength > 0.05) {
        XTestFakeKeyEvent(display, superKeyCode, True, 0);
        XTestFakeKeyEvent(display, plusKeyCode, True, 0);
        XTestFakeKeyEvent(display, plusKeyCode, False, 0);
        XTestFakeKeyEvent(display, superKeyCode, False, 0);
        XFlush(display);
    } else if (prePinchStrength > strength) {
        XTestFakeKeyEvent(display, superKeyCode, True, 0);
        XTestFakeKeyEvent(display, minusKeyCode, True, 0);
        XTestFakeKeyEvent(display, minusKeyCode, False, 0);
        XTestFakeKeyEvent(display, superKeyCode, False, 0);
        XFlush(display);
    }

    prePinchStrength = strength;
}

static void grab(float strength) 
{
    if (strength == 1.0)
        grabbed = true;
    else
        grabbed = false;
}

int main(int argc, char* argv[]) 
{
    MyListener listener;
    Controller controller;
    int c;

    signal(SIGINT, sigHandler);

    while ((c = getopt(argc, argv, "r")) != -1) {
        switch (c) {
        case 'r':
            std::cout << "Enable record sample" << std::endl;
            enableRecord = true;
            sampleFile.open("sample.txt");
            break;
        }
    }

    display = XOpenDisplay(nullptr);
    if (display == nullptr) {
        std::cout << "ERROR: fail to open display" << std::endl;
        return -1;
    }

    root = DefaultRootWindow(display);

    listener.setPositionChanged(positionChanged);
    listener.setTapped(tapped);
    listener.setPinch(pinch);
    listener.setGrab(grab);
    controller.addListener(listener);

    while (!quit) 
        usleep(100);

    controller.removeListener(listener);
    
    if (display) {
        XDestroyWindow(display, root);
        XCloseDisplay(display);
        display = nullptr;
    }

    sampleFile.close();

    return 0;
}
