// Copyright (C) 2015 Leslie Zhai <xiang.zhai@i-soft.com.cn>

#ifndef MY_LISTENER_H
#define MY_LISTENER_H

#include <Leap.h>

using namespace Leap;

typedef void (*PositionChanged)(float, float, float, int);
typedef void (*Tapped)();

class MyListener : public Listener 
{
public:
    explicit MyListener();
    ~MyListener();

    void setPositionChanged(PositionChanged fptr);
    void setTapped(Tapped fptr);

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
    float m_preY = -200.0;
};

#endif // MY_LISTENER_H
