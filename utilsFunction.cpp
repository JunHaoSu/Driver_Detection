//
// Created by sujunhao on 19-4-3.
//
#include "utilsFunction.h"


/**
 * ear值算法 左上右下+右上左下的距离除以左右眼角的两倍,大于0.3代表眼睛睁开
 * @param eye 眼睛的特征点坐标数组
 * @return ear值
 */
double cal_ear(std::vector<cv::Point2d> &eye)
{
    double top_left_bottom_right = getDistance(eye[1],eye[5]);
    double top_right_bottom_left = getDistance(eye[2],eye[4]);
    double width = getDistance(eye[0],eye[3]);
    double ear_number = (top_left_bottom_right + top_right_bottom_left) / (2.0 * width);
    return ear_number;
}


/**
 * 求两点之间的距离
 * @param pointA
 * @param pointB
 * @return
 */
double getDistance(cv::Point2d pointA, cv::Point2d pointB)
{
    double distance;
    distance = powf((pointA.x - pointB.x),2) + powf((pointA.y - pointB.y),2);
    return sqrtf(distance);
}
