// Copyright (C) 2015 Leslie Zhai <xiang.zhai@i-soft.com.cn>

#include <unistd.h>
#include <signal.h>
#include <iostream>
#include <X11/extensions/XTest.h>

#include "mylistener.h"

static Display* display = nullptr;
static Window root = -1;
static bool quit = false;

static void sig_handler(int sig)
{
    if (sig == SIGINT) {
        std::cout << "Bye ;-)" << std::endl;
        quit = true;
    }
}

// Leap Motion 手掌坐标在X轴的“经验”（试出来的，如果官方文档有最好）范围是
// [-100, 100] 坐标转换到当前分辨率下的X11“屏幕”坐标系
// https://developer.leapmotion.com/documentation/java/api/Leap.Vector.html#javaclasscom_1_1leapmotion_1_1leap_1_1_vector
static void positionChanged(float x, float y, float z) 
{
    int screen = 0;
    int screenWidth = DisplayWidth(display, screen);
    int screenHeight = DisplayHeight(display, screen);
    
    //std::cout << x << ", " << z << std::endl;
    XSelectInput(display, root, KeyReleaseMask);
    XWarpPointer(display, None, root, 0, 0, 0, 0, (x + 100) * screenWidth / 200, 
                 (z + 100) * screenHeight / 200);
    XFlush(display);
}

static void mouseClick(int button = Button1) 
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
