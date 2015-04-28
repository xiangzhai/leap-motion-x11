leap-motion-x11
----------------

Leap Motion interactive with X11, just like [laser keyboard] (http://www.robopeak.com/blog/?p=282) Leap Motion与X11交互，类似[激光键盘] (http://www.robopeak.com/blog/?p=315)

It uses:

* palmPosition to move MOUSE CURSOR by XWarpPointer, and generate KEY PRESS event by XTestFakeKeyEvent;
* Gesture::TYPE_KEY_TAP and Gesture::TYPE_SCREEN_TAP gestures to generate MOUSE CLICK event by XQueryPointer and XSendEvent;

so it is able to interactive with KDE, a famous desktop environment based on X11 for Linux:
* move and left click the mouse
* emit left top corner hot zone (hot key)
* generate left and right key press event when viewing photo or presentation


## Dependence for ArchLinux 依赖

* archlinux-leap-motion-driver 
https://gitcafe.com/xiangzhai/archlinux-leap-motion-driver

* archlinux-leap-motion-sdk 
https://gitcafe.com/xiangzhai/archlinux-leap-motion-sdk

```
sudo pacman -S libxtst
```

## Build && Install 编译、安装

```
cmake . -DCMAKE_INSTALL_PREFIX=/usr
make
sudo make install
```


## Interactive with X11 与X11交互

```
leap-motion-x11
```

* In the X and Z axis, move hand left/rightwards 在水平面，向左、右移动手掌

* In the X and Z axis, move hand with (or less than ) 4 fingers upwards, emit 
the left top corner hot zone, move hand in the active windows, click one window 
to raise it KDE4在水平面，用（小于等于）4根手指头做向上的动作，触发KDE4左上角热区，
在活跃窗体之间游走，点击某个窗体，抬起该窗体

Demo video 演示视频 http://v.youku.com/v_show/id_XOTQzNTMxMTQ0.html

* In the X and Z axis, move hand and click by your finger 在水平面，移动手掌，
用食指做点击动作

Demo video 演示视频 http://v.youku.com/v_show/id_XOTQzMDExNjY4.html

