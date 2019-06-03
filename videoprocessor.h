#ifndef VIDEOPROCESSOR_H
#define VIDEOPROCESSOR_H

#include <QObject>
#include <QPixmap>
#include <QDebug>
#include <QtMultimedia/QSoundEffect>
#include <QMutex>
#include <QReadWriteLock>
#include <QSemaphore>
#include <QWaitCondition>
#include "opencv2/opencv.hpp"
#include "dlibinit.h"
#include "utilsfunction.h"

class VideoProcessor : public QObject
{
    Q_OBJECT
public:
    explicit VideoProcessor(QObject *parent = nullptr);

signals:
    void inDisplay(QPixmap pixmap);//原始图像显示信号
    void outDisplay(QPixmap pixmap);//处理后的图像显示信号
    void tired(int level);//疲劳信号

public slots:
    void startVideo();//视频开始槽函数
    void stopVideo();//视频结束槽函数
    void playSound(int);//播放报警声音函数

private:
    bool stopped;//视频是否停止信号
    DlibInit* frame_obj;//图像处理对象
    one_frame_data one;//一帧图像提取出来的参数
    std::ostringstream outtext;//图片显示文字流
    QSoundEffect eye_bells;//中度疲劳警报
    QSoundEffect mouth_bells;//轻度疲劳警报
    QSoundEffect head_bells;//重度疲劳警报
    cv::VideoCapture camera;//摄像头对象
    double head_pose_init;//摄像头安装位置的初始角度
    int window_size;//配置文件定义的滑动窗口大小
    std::vector<one_frame_data> frame_vector;//申请一个队列，格式是一个结构体，包含每一帧需要的数据
};

#endif // VIDEOPROCESSOR_H
