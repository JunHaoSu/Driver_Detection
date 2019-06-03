#include "videoprocessorinit.h"

videoprocessorInit::videoprocessorInit(QObject *parent) : QObject(parent)
{
    dingdong_time.setSource(QUrl("qrc:/sound/res/4570.wav"));
    dingdong_time.setVolume(100);
}

void videoprocessorInit::startVideo()
{
    QSettings *configIniRead = new QSettings("config.ini", QSettings::IniFormat);
    int camera_num = configIniRead->value("/vedio/camera").toInt();//取出相应的初始角,直接在改变
    delete configIniRead;
    using namespace cv;
    camera.open(camera_num);
    stopped = false;
    while(camera.isOpened() && !stopped){
        camera>>inFrame;
        if(inFrame.empty())
            continue;
        emit inDisplay(
                    QPixmap::fromImage(
                        QImage(
                            inFrame.data,
                            inFrame.cols,
                            inFrame.rows,
                            inFrame.step,
                            QImage::Format_RGB888)
                        .rgbSwapped()));
    }
}

void videoprocessorInit::stopVideo()
{
    stopped = true;
    camera.release();
}

void videoprocessorInit::beginCameraInit()
{
  if(!dingdong_time.isPlaying())
  {
      dingdong_time.play();
      QTime delayTime = QTime::currentTime().addMSecs(5000);//在当前时间上增加3S
      while(QTime::currentTime() < delayTime)
          QCoreApplication::processEvents(QEventLoop::AllEvents, 100);

      DlibInit* frame_obj = new DlibInit();
      frame_obj->find_68_point(inFrame);
      cv::Mat re = frame_obj->cal_detect_angle();
      qDebug()<<"test:"<<re.at<double>(0);
      QSettings *configIniWrite = new QSettings("config.ini", QSettings::IniFormat);
      configIniWrite->setValue("/vedio/head_pose_init", re.at<double>(0));//写入初始角度
      delete configIniWrite;//释放内存
  }
}

