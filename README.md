leap-motion-x11
----------------

Leap Motion [Interactive with X11](https://developer.leapmotion.com/gallery/interactive-with-x11) just like [laser keyboard](http://www.robopeak.com/blog/?p=282) Leap Motion与X11交互，类似[激光键盘](http://www.robopeak.com/blog/?p=315)

It uses 它使用

* stabilizedPalmPosition to move MOUSE CURSOR by XWarpPointer, and generate 
KEY PRESS event by XTestFakeKeyEvent 借助XWrapPointer通过stabilizedPalmPosition
的坐标来移动鼠标指针，借助XTestFakeKeyEvent产生键盘点击事件
* Gesture::TYPE_KEY_TAP and Gesture::TYPE_SCREEN_TAP gestures to generate 
MOUSE CLICK event by XQueryPointer and XSendEvent 通过KEY_TAP和SCREEN_TYPE手势
借助XQueryPointer和XSendEvent产生鼠标点击事件

so it is able to interactive with KDE - a famous desktop environment based on X11 for Linux 因此它可以与KDE基于X11进行交互

* move and left click the mouse 移动、点击鼠标 
* emit left top corner hot zone (hot key) 触发坐上角热区 
* generate left and right key press event when viewing photo or presentation 
在看图片、ppt的时候产生左、右键盘事件 


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


## HOWTO interactive with X11 怎么玩？与X11交互

```
./src/leap-motion-x11
```

![ScreenShot](https://developer-china-cdn.leapmotion.com/documentation/images/Leap_Axes.png)

* In the X-Z plane, move hand left or rightwards quickly, emit left and right 
key press event when viewing photo or presentation 在X-Z水平面，向左、右快速移动
手掌，触发向左、右键盘事件，可以用来看图片、ppt

Demo video 演示视频 http://v.youku.com/v_show/id_XOTQ0NDQ0NjQ4.html

* In the X-Z plane, move hand forwards quickly, emit the left top corner hot 
zone, move hand between the active windows, tap (click) one to raise it up
在X-Z水平面，快速做向前的动作，触发左上角热区，在活跃窗体之间游走，点击某个窗体，
抬起该窗体

Demo video 演示视频 http://v.youku.com/v_show/id_XOTQzNTMxMTQ0.html

* In the X-Z plane, move hand and tap (click) by your finger 在X-Z水平面，移动手掌，
用食指做点击动作

Demo video 演示视频 http://v.youku.com/v_show/id_XOTQzMDExNjY4.html

* In the X-Z plane, pinch in/out to zoom in/out KDE desktop 在X-Z水平面，缩放桌面

* In the X-Z plane, grab to move active window 在X-Z水平面，抓着活跃窗体移动

Demo video 演示视频 http://v.youku.com/v_show/id_XOTUzMTc3NjAw.html

## Sample 采样

```
./src/leap-motion-x11 -r
```
