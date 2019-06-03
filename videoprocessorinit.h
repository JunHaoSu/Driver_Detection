#ifndef VIDEOPROCESSORINIT_H
#define VIDEOPROCESSORINIT_H

#include <QObject>
#include <QPixmap>
#include <QDebug>
#include <QTime>
#include <QCoreApplication>
#include <QEventLoop>
#include <QtMultimedia/QSoundEffect>
#include "dlibinit.h"
#include "opencv2/opencv.hpp"

class videoprocessorInit : public QObject
{
    Q_OBJECT
public:
    explicit videoprocessorInit(QObject *parent = nullptr);

signals:
    void inDisplay(QPixmap pixmap);//原始图像显示信号
    void outDisplay(QPixmap pixmap);//处理后的图像显示信号

public slots:
    void startVideo();//视频开始槽函数
    void stopVideo();//视频结束槽函数
    void beginCameraInit();//开始相机初始化

private:
    cv::Mat inFrame;//每一帧的数据
    bool stopped;//视频是否停止信号
    cv::VideoCapture camera;//相机
    QSoundEffect dingdong_time;//语音提示
};

#endif // VIDEOPROCESSORINIT_H
