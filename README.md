leap-motion-x11
----------------

Leap Motion与X11交互，类似激光键盘 laserkbd http://www.robopeak.com/blog/?p=315


## 依赖 4 ArchLinux

* archlinux-leap-motion-driver 
https://gitcafe.com/xiangzhai/archlinux-leap-motion-driver

* archlinux-leap-motion-sdk 
https://gitcafe.com/xiangzhai/archlinux-leap-motion-sdk


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

移动手掌，用食指做键盘点击动作 ;-)

演示视频 http://v.youku.com/v_show/id_XOTQzMDExNjY4.html

