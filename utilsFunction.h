//
// Created by sujunhao on 19-4-3.
//

#ifndef DRIVER_DETECTION_UTILSFUNCTION_H
#define DRIVER_DETECTION_UTILSFUNCTION_H

#include <vector>
#include <opencv2/opencv.hpp>

double cal_ear(std::vector<cv::Point2d> &eye);

double getDistance(cv::Point2d pointA, cv::Point2d pointB);

#endif //DRIVER_DETECTION_UTILSFUNCTION_H
