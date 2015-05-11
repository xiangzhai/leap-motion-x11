// Copyright (C) 2015 Leslie Zhai <xiang.zhai@i-soft.com.cn>

#ifndef MYLISTENER_H
#define MYLISTENER_H

#include <Leap.h>

using namespace Leap;

typedef void (*PositionChanged)(float, float, float, int, Vector, Vector);
typedef void (*Tapped)();
typedef void (*Pinch)(float);

class MyListener : public Listener 
{
public:
    explicit MyListener();
    ~MyListener();

    void setPositionChanged(PositionChanged fptr);
    void setTapped(Tapped fptr);
    void setPinch(Pinch fptr);

protected:
    virtual void onInit(const Controller&);                                        
    virtual void onConnect(const Controller&);                                     
    virtual void onDisconnect(const Controller&);                                  
    virtual void onExit(const Controller&);                                        
    virtual void onFrame(const Controller&);                                       
    virtual void onFocusGained(const Controller&);                                 
    virtual void onFocusLost(const Controller&);                                   
    virtual void onDeviceChange(const Controller&);                                
    virtual void onServiceConnect(const Controller&);                              
    virtual void onServiceDisconnect(const Controller&);

private:
    PositionChanged m_positionChanged = nullptr;
    Tapped m_tapped = nullptr;
    Pinch m_pinch = nullptr;
};

#endif // MYLISTENER_H
