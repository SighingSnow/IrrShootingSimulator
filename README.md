# IrrShootingSimulator

## 1 概述
本项目主要由一个Demo游戏和Irrlicht鬼火引擎的源代码组成。Demo游戏可在Windows，Linux，MacOS下编译运行。并且希望能够更加依赖GPU显卡等硬件，希望发挥云端服务器的渲染显卡优势。

## 2 Irrlicht编译运行
### Windows
请include在demo/source/include文件下的所有文件，之后将main.cpp加入vs的工程中并选择相应的sdk，配置工程选择Win10sdk，并选择相应的路径指向lib库中的静态库文件。

### Linux
直接make（目前编译环境为ArchLinux）

### OSX
请include在demo/source/include文件下的所有文件，并在Xcode中添加IOkit, Cocoa, OpenGL以及lib下的OSX中的libIrrilicht.a添加到工程中，

## 3 游戏简介

游戏与CS等FPS游戏有高度相似性，可以在macos和windows系统上进行游戏。游戏网络库基于SLikeNet，引擎为Irrlicht鬼火引擎。

### 3.1 编译运行

在windows上建议使用Visual Studio进行编译工作。

在macos可以直接解压工程文件进行编译。

### 3.2 目前问题

* 网络端表项良好，但是碰撞检测等问题尚待解决；
* UI界面仍需要优化，包括局内UI
