#include "videoprocessor.h"
/**
 * 视频显示处理类
 * @brief VideoProcessor::VideoProcessor
 * @param parent
 */
VideoProcessor::VideoProcessor(QObject *parent) : QObject(parent)
{
    eye_bells.setSource(QUrl("qrc:/sound/res/4031.wav"));
    mouth_bells.setSource(QUrl("qrc:/sound/res/copen_mouth.wav"));
    head_bells.setSource(QUrl("qrc:/sound/res/11385.wav"));
    eye_bells.setVolume(100);
    mouth_bells.setVolume(100);
    head_bells.setVolume(100);

    QSettings *configIniRead = new QSettings("config.ini", QSettings::IniFormat);
    window_size = configIniRead->value("/vedio/window_size").toInt();//取出相应的初始角,直接在改变
    delete configIniRead;

    //将与视频有关的量放入构造函数
    frame_obj = new DlibInit();

    one.eye_status=true;//初始化true代表睁眼
    one.mouth_status=true;//初始化true代表闭嘴
    one.head_pose_status["x"]=0.0;//初始化位置为正前方
    one.head_pose_status["y"]=0.0;
    one.head_pose_status["z"]=0.0;

    //初始化队列，窗口大小从配置文件读取
    for(int i=0;i<window_size;i++)
        frame_vector.push_back(one);

    connect(this,
            SIGNAL(tired(int)),
            this,
            SLOT(playSound(int))
            );
}

void VideoProcessor::startVideo()
{
    QSettings *configIniRead = new QSettings("config.ini", QSettings::IniFormat);
    int camera_num = configIniRead->value("/vedio/camera").toInt();//取出相应的初始角,直接在改变
    delete configIniRead;
    using namespace cv;
    Mat inFrame, outFrame;
    camera.open(camera_num);//打开摄像头，
    stopped = false;
    while(camera.isOpened() && !stopped)
    {
        camera >> inFrame;
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

        cvtColor(inFrame,outFrame,CV_BGR2GRAY);

        frame_obj->find_68_point(inFrame);

        cv::Mat re = frame_obj->cal_detect_angle();
        double ear = frame_obj->eye_aspect_ratio();
        double open_mouth = frame_obj->mouth_aspect_ratio();

        frame_vector.erase(frame_vector.begin());

        if(re.at<double>(0)==0.0){
            frame_vector.push_back(one);
        }else{
            one_frame_data two;
            two.eye_status = ear > 0.18 ? true : false ;
            two.mouth_status = open_mouth > 0.6 ? false : true ;
            two.head_pose_status["x"] = re.at<double>(0);
            two.head_pose_status["y"] = re.at<double>(1);
            two.head_pose_status["z"] = re.at<double>(2);
            frame_vector.push_back(two);
        }

        std::vector<cv::Point2d> face_68_point = frame_obj->get_68_point();
        if(!face_68_point.empty()){
            for (unsigned int i = 0; i < 68; ++i){
               cv::circle(outFrame, cv::Point(face_68_point[i].x, face_68_point[i].y), 2, cv::Scalar(0, 0, 255), -1);
            }
        }

        int mouth_count = count_if(frame_vector.begin(),frame_vector.end(),comp_mouth);
        int eye_count = count_if(frame_vector.begin(),frame_vector.end(),comp_eye);

        //使用辅助函数进行处理头部姿态，分析
        //函数输入值是一个vector，输出检测结果。
        //std::cout<<head_pose_init<<" "<<window_size<<std::endl;
        bool head_status = comp_head_pose(frame_vector);

        if( mouth_count< 20){
            emit tired(0);
        }
        if( eye_count < 20) {
            emit tired(1);
        }
        if(head_status==false){
            emit tired(2);
        }

        outtext << "X: " << std::setprecision(3) <<re.at<double>(0);
        cv::putText(outFrame, outtext.str(), cv::Point(50, 40), cv::FONT_HERSHEY_SIMPLEX, 0.75, cv::Scalar(255, 0, 0));
        outtext.str("");
        outtext << "Y: " << std::setprecision(3) <<re.at<double>(1);
        cv::putText(outFrame, outtext.str(), cv::Point(50, 60), cv::FONT_HERSHEY_SIMPLEX, 0.75, cv::Scalar(255, 0, 0));
        outtext.str("");
        outtext << "Z: " << std::setprecision(3) <<re.at<double>(2);
        cv::putText(outFrame, outtext.str(), cv::Point(50, 80), cv::FONT_HERSHEY_SIMPLEX, 0.75, cv::Scalar(255, 0, 0));
        outtext.str("");
        outtext << "eye: " << std::setprecision(3) <<eye_count;
        cv::putText(outFrame, outtext.str(), cv::Point(50, 100), cv::FONT_HERSHEY_SIMPLEX, 0.75, cv::Scalar(255, 0, 0));
        outtext.str("");
        outtext << "mouth: " << std::setprecision(3) <<mouth_count;
        cv::putText(outFrame, outtext.str(), cv::Point(50, 120), cv::FONT_HERSHEY_SIMPLEX, 0.75, cv::Scalar(255, 0, 0));
        outtext.str("");
        frame_obj->clear_68_point();



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

/**
 * 暂停槽函数
 * @brief VideoProcessor::stopVideo
 */
void VideoProcessor::stopVideo()
{
    qDebug() << "Pause";
    stopped = true;
    camera.release();
}


void VideoProcessor::playSound(int level)
{
    if(level == 0){
        if(!mouth_bells.isPlaying())
        {
            mouth_bells.play();
            qDebug() << "mouth_called";
        }
    }else if(level == 1){
        if(!eye_bells.isPlaying())
        {
            eye_bells.play();
            qDebug() << "eye_called";
        }
    }else if(level == 2){
        if(!head_bells.isPlaying())
        {
            head_bells.play();
            qDebug() << "head_called";
        }
    }

}
