#ifndef UTILSFUNCTION_H
#define UTILSFUNCTION_H

#include<numeric>
#include <vector>
#include <opencv2/opencv.hpp>
#include <QSettings>
struct one_frame_data
{
    bool eye_status;
    bool mouth_status;
    std::map<std::string, double> head_pose_status;
};

//计算头部姿态计算方式的回调函数,计算head_pose_status中的x值,也就是俯仰角
struct AddFun
{
    double operator()(double n, one_frame_data& m) const
    {
        return n + m.head_pose_status["x"];
    }
};



double cal_ear(std::vector<cv::Point2d> &eye);

double getDistance(cv::Point2d pointA, cv::Point2d pointB);

double cal_mouth(std::vector<cv::Point2d> &mouth);

bool comp_mouth(one_frame_data);
bool comp_eye(one_frame_data);

bool comp_head_pose(std::vector<one_frame_data> &frame_vector);

#endif // UTILSFUNCTION_H
