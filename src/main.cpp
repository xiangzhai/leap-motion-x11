// Copyright (C) 2015 Leslie Zhai <xiang.zhai@i-soft.com.cn>

#include <unistd.h>
#include <signal.h>
#include <iostream>
#include <X11/Xutil.h>
#include <X11/extensions/XTest.h>

#include "mylistener.h"

static Display* display = nullptr;
static Window root = -1;
static bool quit = false;
static float m_preZ = -200.0;

static void sig_handler(int sig)
{
    if (sig == SIGINT) {
        std::cout << "Bye ;-)" << std::endl;
        quit = true;
    }
}

// Leap Motion 手掌坐标 https://developer.leapmotion.com/documentation/cpp/api/Leap.Vector.html#cppstruct_leap_1_1_vector 
// 东、西方向上是X轴，南、北是Z轴，上、下是Y轴
// 在X、Y轴的“经验”（尝试出来的，文档上貌似没有介绍？！）区间是[-100, 100] 
// 但是当手掌位于“边缘”比如-100时
// Gesture::TYPE_KEY_TAP 手势就不准了！所以需要把区间缩小！[-70, 70]
// 坐标转换到当前分辨率下的X11“屏幕”坐标系
// FIXME: 由于手掌是有轻微抖动的，所以需要做曲线拟合（平滑）使得鼠标移动更加平滑
static void positionChanged(float x, 
                            float y, 
                            float z, 
                            int count/* 伸开的手指个数 */) 
{
    int screen = 0;
    int screenWidth = DisplayWidth(display, screen);
    int screenHeight = DisplayHeight(display, screen);
    
    //std::cout << x << ", " << z << ", " << count << std::endl;

    // 最好能带上加速度做为评估值
    if (z < m_preZ && m_preZ - z > 5.0) {
        // 如果伸开的手指个数小于4，在KDE4下触发Ctrl+F8唤出左上角热区
        if (count < 4) {
            KeyCode ctrlKeyCode = XKeysymToKeycode(display, XK_Control_L);
            KeyCode f8KeyCode = XKeysymToKeycode(display, XK_F8);
            
            XTestFakeKeyEvent(display, ctrlKeyCode, True, 0);
            XTestFakeKeyEvent(display, f8KeyCode, True, 0);
            XTestFakeKeyEvent(display, f8KeyCode, False, 0);
            XTestFakeKeyEvent(display, ctrlKeyCode, False, 0);
            XFlush(display);
        }
    }

    // 移动鼠标
    XSelectInput(display, root, KeyReleaseMask);
    XWarpPointer(display, None, root, 0, 0, 0, 0, (x + 70) * screenWidth / 140, 
                 (z + 70) * screenHeight / 140);
    XFlush(display);
    
    m_preZ = z;
}

static void mouseClick(int button = Button1/* 默认鼠标左键 */) 
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
