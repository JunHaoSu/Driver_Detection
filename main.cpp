#include <iostream>

#include "DlibInit.h"
//#include "utils/ConfigManager.h"

int main()
{

    cv::VideoCapture cap(0);
    if (!cap.isOpened())
    {
        std::cout << "Unable to connect to camera" << std::endl;
        return EXIT_FAILURE;
    }
    std::ostringstream outtext;
    DlibInit* test_obj = new DlibInit();

    int count_frame = 0;
    while (1)
    {
        // Grab a frame
        cv::Mat temp;
        cap >> temp;
        cv::imshow("初始读入图片", temp);

        test_obj->find_68_point(temp);


        cv::Mat re = test_obj->cal_detect_angle();
        double ear = test_obj->eye_aspect_ratio();


       //添加一个计数器,防止抖动,如果真的是闭眼,不会抖动超过十次,十次也是经验值
       if(ear < 0.3){
            count_frame++;
            if(count_frame > 10){
                std::cout<<"闭眼"<<std::endl;
            }
        }else{
            count_frame = 0;
        }

        outtext << "X: " << std::setprecision(3) <<re.at<double>(0);
        cv::putText(temp, outtext.str(), cv::Point(50, 40), cv::FONT_HERSHEY_SIMPLEX, 0.75, cv::Scalar(0, 0, 0));
        outtext.str("");
        outtext << "Y: " << std::setprecision(3) <<re.at<double>(1);
        cv::putText(temp, outtext.str(), cv::Point(50, 60), cv::FONT_HERSHEY_SIMPLEX, 0.75, cv::Scalar(0, 0, 0));
        outtext.str("");
        outtext << "Z: " << std::setprecision(3) <<re.at<double>(2);
        cv::putText(temp, outtext.str(), cv::Point(50, 80), cv::FONT_HERSHEY_SIMPLEX, 0.75, cv::Scalar(0, 0, 0));
        outtext.str("");
        cv::imshow("结果处理图",temp);

        test_obj->clear_68_point();

        unsigned char key = cv::waitKey(1);
        if (key == 27)
        {
            break;
        }
    }


    return 0;
}