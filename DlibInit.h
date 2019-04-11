//
// Created by sujunhao on 19-3-24.
//

#ifndef DRIVER_DETECTION_DLIBINIT_H
#define DRIVER_DETECTION_DLIBINIT_H

#include <opencv2/opencv.hpp>
#include <dlib/opencv.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/core/core.hpp>
#include <dlib/image_processing/full_object_detection.h>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing.h>

#include <vector>
#include <string>
#include <cstdio>

class DlibInit {
public:
    DlibInit();
    ~DlibInit();

    cv::Mat cal_detect_angle();//人脸欧拉角检测

    double mouth_aspect_ratio();

    void clear_68_point();//清空68点数据

    double eye_aspect_ratio();//人眼纵横比数据检测

    void find_68_point(cv::Mat &src_img);//找到68个点的坐标

private:
    dlib::shape_predictor predictor;//关键点检测器
    dlib::frontal_face_detector detector;//人脸检测器
    void image_processing(cv::Mat &img);//图像预处理函数

    std::vector<cv::Point3d> object_pts;//世界坐标系人脸3D坐标集合
    void init_object_pts();//初始化人脸3D坐标

    /**相当神奇,将这两个参数组成一个结构体可以解决该变量随机变化的问题.
     * 将这两个变量跟随类走而不放在函数中定义的目的是避免每一张图片都要处理这两个参数
     * 这样可以减少程序计算量,但是直接放在private中会出现随着程序进行而随机变化的BUG,
     * 单步调试推测是指针的问题,但是这个Opencv的锅,我不知道该如何解决,但是将他们放进一个结构体,
     * 神奇的BUG解决了*/
    struct cam_parameter{
        cv::Mat cam_matrix;//相机内参数矩阵
        cv::Mat dist_coeffs;//畸变系数
    } cam_parameter1;
    void init_cam_intrinsics();//初始化相机内参数矩阵和畸变系数
    std::vector<cv::Point2d> face_68_point;//人脸68点,属性私有,操作方法公有
};


#endif //DRIVER_DETECTION_DLIBINIT_H
