# 驾驶员检测系统

本科毕设,使用计算机视觉完成驾驶员的疲劳检测.最终想法是嵌入式,所以直接选择linux平台.基于OpenCV和dlib框架,分支master使用CLion调试, qt分支使用QT Creator调试
## 开发环境

- Ubuntu 18.04
- g++ 7.3/c++ 11
- Clion
- OpenCV 2.4.10
- Dlib 19.16 
- Qt Creator

## 68关键点分布图
![68点分布图](./res/1910e9cc-0fb4-11e7-987b-0fecce2c829e.JPG)

## 2019-4-18更新日志
碧蓝更新, 没得游戏肝, 肝代码. 工作内容如下:
- QT界面支持
- 将qmake构建的GUI工程改为cmake构建
- 将视频读取改为多线程