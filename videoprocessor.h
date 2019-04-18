#ifndef VIDEOPROCESSOR_H
#define VIDEOPROCESSOR_H

#include <QObject>
#include <QPixmap>
#include <QDebug>
#include <QMutex>
#include <QReadWriteLock>
#include <QSemaphore>
#include <QWaitCondition>
#include "opencv2/opencv.hpp"
#include "dlibinit.h"

class VideoProcessor : public QObject
{
    Q_OBJECT
public:
    explicit VideoProcessor(QObject *parent = nullptr);

signals:
    void inDisplay(QPixmap pixmap);//原始图像显示信号
    void outDisplay(QPixmap pixmap);//处理后的图像显示信号

public slots:
    void startVideo();//视频开始槽函数
    void stopVideo();//视频结束槽函数

private:
    bool stopped;//视频是否停止信号
    DlibInit* test_obj;//图像处理对象
    std::string eye_status;//眼睛状态量
    std::string mouth_status;//嘴巴状态量
    int eye_count_frame;//眼睛状态帧数
    int mouth_count_frame;//嘴巴状态帧数
    std::ostringstream outtext;//图片显示文字流
};

#endif // VIDEOPROCESSOR_H
