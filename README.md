# IrrShootingSimulator

## 概述
本项目主要由一个Demo游戏和Irrlicht鬼火引擎的源代码组成。Demo游戏可在Windows，Linux，MacOS下编译运行。并且希望能够更加依赖GPU显卡等硬件，希望发挥云端服务器的渲染显卡优势。

## 项目结构

## Demo编译运行
### Windows
请include在demo/source/include文件下的所有文件，之后将main.cpp加入vs的工程中并选择相应的sdk，配置工程选择Win10sdk，并选择相应的路径指向lib库中的静态库文件。

### Linux
直接make（目前编译环境为ArchLinux）

### OSX
请include在demo/source/include文件下的所有文件，并在Xcode中添加IOkit, Cocoa, OpenGL以及lib下的OSX中的libIrrilicht.a添加到工程中，
请选择Xcode 3.2 版本，平台为10.7。

## 游戏简介

游戏与CS等FPS游戏有高度相似性，可以在macos和windows系统上进行游戏。
