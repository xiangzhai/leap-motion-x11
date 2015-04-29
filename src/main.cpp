// Copyright (C) 2015 Leslie Zhai <xiang.zhai@i-soft.com.cn>

#include <unistd.h>
#include <signal.h>
#include <iostream>
#include <X11/Xutil.h>
#include <X11/extensions/XTest.h>

#include "mylistener.h"

static const float velcityFilter = 10.0;

static Display* display = nullptr;
static Window root = -1;
static bool quit = false;
static float m_preX = -200.0;
static float m_preZ = -200.0;
static int m_preTimestamp = 0;

static void sig_handler(int sig)
{
    if (sig == SIGINT) {
        std::cout << "Bye ;-)" << std::endl;
        quit = true;
    }
}

static float velcity(float deltaDisplace, int deltaTime) 
{
    return deltaTime == 0 ? 0 : deltaDisplace / deltaTime * 10000;
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
// the range, for example [-70, 70] 
// 手势就不准了！所以需要把区间缩小！[-70, 70]
// 
// Matrix transform to current X11 screen resoluation 
// 坐标转换到当前分辨率下的X11“屏幕”坐标系
//
// FIXME: 
// Because palm might be slightly shake, so it needs to do Curve Smooth to make
// mouse cursor movement more smooth 
// 由于手掌是有轻微抖动的，所以需要做曲线拟合（平滑）使得鼠标移动更加平滑
//
static void positionChanged(float x, 
                            float y, 
                            float z, 
                            int count/* extend fingers count 伸开的手指个数 */, 
                            int timestamp) 
{
    int screen = 0;
    int screenWidth = DisplayWidth(display, screen);
    int screenHeight = DisplayHeight(display, screen);

    if (m_preTimestamp == 0)
        m_preTimestamp = timestamp;

    if (m_preX == -200.0) 
        m_preX = x;

    if (m_preZ == -200.0)
        m_preZ = z;

    if (abs(x - m_preX) > abs(z - m_preZ)) {
        if (x < m_preX && 
            velcity(m_preX - x, timestamp - m_preTimestamp) > velcityFilter) {
            std::cout << "Emit rightwards" << std::endl;
            KeyCode leftKeyCode = XKeysymToKeycode(display, XK_Left);

            XTestFakeKeyEvent(display, leftKeyCode, True, 0);
            XTestFakeKeyEvent(display, leftKeyCode, False, 0);
            XFlush(display);
        } else if (m_preX < x && 
                   velcity(x - m_preX, timestamp - m_preTimestamp) > velcityFilter) {
            std::cout << "Emit leftwards" << std::endl;
            KeyCode rightKeyCode = XKeysymToKeycode(display, XK_Right);

            XTestFakeKeyEvent(display, rightKeyCode, True, 0);
            XTestFakeKeyEvent(display, rightKeyCode, False, 0);
            XFlush(display);
        }
    } else {
        if (z < m_preZ && 
            velcity(m_preZ - z, timestamp - m_preTimestamp) > velcityFilter) {
            // emit left top corner hot zone for KDE 
            // 在KDE下触发Ctrl+F10唤出左上角热区
            std::cout << "Emit left top corner hot zone" << std::endl;
            KeyCode ctrlKeyCode = XKeysymToKeycode(display, XK_Control_L);
            KeyCode f10KeyCode = XKeysymToKeycode(display, XK_F10);
            
            XTestFakeKeyEvent(display, ctrlKeyCode, True, 0);
            XTestFakeKeyEvent(display, f10KeyCode, True, 0);
            XTestFakeKeyEvent(display, f10KeyCode, False, 0);
            XTestFakeKeyEvent(display, ctrlKeyCode, False, 0);
            XFlush(display);
        }
    }

    // 移动鼠标
    XSelectInput(display, root, KeyReleaseMask);
    XWarpPointer(display, None, root, 0, 0, 0, 0, (x + 70) * screenWidth / 140, 
                 (z + 70) * screenHeight / 140);
    XFlush(display);
    
    m_preX = x;
    m_preZ = z;
    m_preTimestamp = timestamp;
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
    std::cout << "Emit mouse click" << std::endl;
    mouseClick();
}

int main(int argc, char* argv[]) 
{
    MyListener listener;
    Controller controller;
   
    signal(SIGINT, sig_handler);

    display = XOpenDisplay(nullptr);
    if (display == nullptr) {
        std::cout << "ERROR: fail to open display" << std::endl;
        return -1;
    }

    root = DefaultRootWindow(display);

    listener.setPositionChanged(positionChanged);
    listener.setTapped(tapped);
    controller.addListener(listener);

    while (!quit) 
        usleep(100);

    controller.removeListener(listener);
    
    if (display) {
        XDestroyWindow(display, root);
        XCloseDisplay(display);
        display = nullptr;
    }

    return 0;
}
