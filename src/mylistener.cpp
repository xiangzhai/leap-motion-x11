// Copyright (C) 2015 Leslie Zhai <xiang.zhai@i-soft.com.cn>

#include "mylistener.h"

#include <iostream>

MyListener::MyListener() 
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
}

MyListener::~MyListener() 
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
}

void MyListener::setPositionChanged(PositionChanged fptr) 
{
    m_positionChanged = fptr;
}

void MyListener::setTapped(Tapped fptr) 
{
    m_tapped = fptr;
}

void MyListener::onInit(const Controller& controller) 
{ 
    std::cout << "Initialized" << std::endl;
}

void MyListener::onConnect(const Controller& controller) 
{
    std::cout << "Connected" << std::endl;
    controller.enableGesture(Gesture::TYPE_CIRCLE);
    controller.enableGesture(Gesture::TYPE_KEY_TAP);
    controller.enableGesture(Gesture::TYPE_SCREEN_TAP);
    controller.enableGesture(Gesture::TYPE_SWIPE);
}

void MyListener::onDisconnect(const Controller& controller) 
{
    std::cout << "Disconnected" << std::endl;
}

void MyListener::onExit(const Controller& controller) 
{
    std::cout << "Exited" << std::endl;
}

void MyListener::onFrame(const Controller& controller) 
{
    const Frame frame = controller.frame();
    const GestureList gestures = frame.gestures();
    HandList hands = frame.hands();
    
    for (HandList::const_iterator hl = hands.begin(); hl != hands.end(); ++hl) {
        const Hand hand = *hl;
        Vector position = hand.palmPosition();

        if (m_preY == -200.0)
            m_preY = position[1];
        if (m_positionChanged)
            m_positionChanged(position[0], position[1], position[2], 
                              frame.fingers().extended().count(), 
                              hand.direction(),
                              hand.palmVelocity());

        m_preY = position[1];
    }

    for (int g = 0; g < gestures.count(); ++g) {
        Gesture gesture = gestures[g];
        switch (gesture.type()) {
        case Gesture::TYPE_KEY_TAP:
        case Gesture::TYPE_SCREEN_TAP:
            if (m_tapped)
                m_tapped();
            break;
        case Gesture::TYPE_SWIPE:
            std::cout << "Swip gesture is very difficult to experience?!" << std::endl;
            break;
        }
    }
}

void MyListener::onFocusGained(const Controller& controller) 
{
    std::cout << "Focus Gained" << std::endl;
}

void MyListener::onFocusLost(const Controller& controller) 
{
    std::cout << "Focus Lost" << std::endl;
}

void MyListener::onDeviceChange(const Controller& controller) 
{
    std::cout << "Device Changed" << std::endl;
    const DeviceList devices = controller.devices();

    for (int i = 0; i < devices.count(); ++i) {
        std::cout << "id: " << devices[i].toString() << std::endl;
        std::cout << "  isStreaming: " 
                  << (devices[i].isStreaming() ? "true" : "false") << std::endl;
    }
}

void MyListener::onServiceConnect(const Controller& controller) 
{
    std::cout << "Service Connected" << std::endl;
}

void MyListener::onServiceDisconnect(const Controller& controller) 
{
    std::cout << "Service Disconnected" << std::endl;
}
