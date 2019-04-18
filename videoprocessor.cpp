#include "videoprocessor.h"
#include <QDebug>
/**
 * 视频显示处理类
 * @brief VideoProcessor::VideoProcessor
 * @param parent
 */
VideoProcessor::VideoProcessor(QObject *parent) : QObject(parent)
{
    //将与视频有关的量放入构造函数
    test_obj = new DlibInit();
    eye_count_frame = 0;
    mouth_count_frame = 0;
}

void VideoProcessor::startVideo()
{
    using namespace cv;
    VideoCapture camera(0);
    Mat inFrame, outFrame;
    stopped = false;

    while(camera.isOpened() && !stopped)
    {
        camera >> inFrame;
        if(inFrame.empty())
            continue;
        //bitwise_not(inFrame, outFrame);

        emit inDisplay(
                    QPixmap::fromImage(
                        QImage(
                            inFrame.data,
                            inFrame.cols,
                            inFrame.rows,
                            inFrame.step,
                            QImage::Format_RGB888)
                        .rgbSwapped()));

        cvtColor(inFrame,outFrame,CV_BGR2GRAY);
        test_obj->find_68_point(inFrame);

        cv::Mat re = test_obj->cal_detect_angle();
        double ear = test_obj->eye_aspect_ratio();
        double open_mouth = test_obj->mouth_aspect_ratio();
        eye_status = "open";
        mouth_status = "close";
        if(ear < 0.29){
            eye_count_frame++;
            if(eye_count_frame > 10){
                eye_status = "close";
            }
        }else{
            eye_count_frame = 0;
        }

        //添加一个计数器,防止抖动,如果真的是闭眼,不会抖动超过十次,十次也是经验值
        if(open_mouth > 0.35){
             mouth_count_frame++;
             if(mouth_count_frame > 10){
                 mouth_status = "open";
                 if(mouth_count_frame > 30){
                      mouth_status = "Yawning";
                 }
             }
         }else{
             mouth_count_frame = 0;
         }


        std::vector<cv::Point2d> face_68_point = test_obj->get_68_point();
        if(!face_68_point.empty()){
            for (unsigned int i = 0; i < 68; ++i){
               cv::circle(outFrame, cv::Point(face_68_point[i].x, face_68_point[i].y), 2, cv::Scalar(0, 0, 255), -1);
            }
        }


        outtext << "X: " << std::setprecision(3) <<re.at<double>(0);
        cv::putText(outFrame, outtext.str(), cv::Point(50, 40), cv::FONT_HERSHEY_SIMPLEX, 0.75, cv::Scalar(0, 0, 0));
        outtext.str("");
        outtext << "Y: " << std::setprecision(3) <<re.at<double>(1);
        cv::putText(outFrame, outtext.str(), cv::Point(50, 60), cv::FONT_HERSHEY_SIMPLEX, 0.75, cv::Scalar(0, 0, 0));
        outtext.str("");
        outtext << "Z: " << std::setprecision(3) <<re.at<double>(2);
        cv::putText(outFrame, outtext.str(), cv::Point(50, 80), cv::FONT_HERSHEY_SIMPLEX, 0.75, cv::Scalar(0, 0, 0));
        outtext.str("");
        outtext << "eye: " << std::setprecision(3) <<eye_status;
        cv::putText(outFrame, outtext.str(), cv::Point(50, 100), cv::FONT_HERSHEY_SIMPLEX, 0.75, cv::Scalar(0, 0, 0));
        outtext.str("");
        outtext << "mouth: " << std::setprecision(3) <<mouth_status;
        cv::putText(outFrame, outtext.str(), cv::Point(50, 120), cv::FONT_HERSHEY_SIMPLEX, 0.75, cv::Scalar(0, 0, 0));
        outtext.str("");
        test_obj->clear_68_point();

        //qDebug() << open_mouth;
        //qDebug() << eye_count_frame;



        emit outDisplay(
                    QPixmap::fromImage(
                        QImage(
                            outFrame.data,
                            outFrame.cols,
                            outFrame.rows,
                            outFrame.step,
                            QImage::Format_Indexed8)
                        .rgbSwapped()));
    }
}


void VideoProcessor::stopVideo()
{
    qDebug() << Q_FUNC_INFO;
    stopped = true;
}
