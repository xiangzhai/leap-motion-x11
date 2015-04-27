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
