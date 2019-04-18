#ifndef UTILSFUNCTION_H
#define UTILSFUNCTION_H

#include <vector>
#include <opencv2/opencv.hpp>

double cal_ear(std::vector<cv::Point2d> &eye);

double getDistance(cv::Point2d pointA, cv::Point2d pointB);

double cal_mouth(std::vector<cv::Point2d> &mouth);

#endif // UTILSFUNCTION_H
