leap-motion-x11
----------------

Leap Motion与X11交互，类似激光键盘 laserkbd http://www.robopeak.com/blog/?p=315


## 依赖 4 ArchLinux

* archlinux-leap-motion-driver 
https://gitcafe.com/xiangzhai/archlinux-leap-motion-driver

* archlinux-leap-motion-sdk 
https://gitcafe.com/xiangzhai/archlinux-leap-motion-sdk

```
sudo pacman -S libxtst
```

## 编译、安装

```
cmake . -DCMAKE_INSTALL_PREFIX=/usr
make
sudo make install
```


## 与X11交互

```
leap-motion-x11
```

* 在水平面，向左、右移动手掌

* 在水平面，用（小于等于）4根手指头做向上的动作，触发KDE4左上角热区，
在活跃窗体之间游走，点击某个窗体，抬起该窗体

演示视频 http://v.youku.com/v_show/id_XOTQzNTMxMTQ0.html

* 在水平面，移动手掌，用食指做点击动作 ;-)

演示视频 http://v.youku.com/v_show/id_XOTQzMDExNjY4.html

