#include "utilsfunction.h"
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
 * 计算张口度
 * @param mouth
 * @return
 */
double cal_mouth(std::vector<cv::Point2d> &mouth)
{
    double inside_width = getDistance(mouth[12],mouth[16]);
    double inside_high = getDistance(mouth[14],mouth[18]);
    double outside_width = getDistance(mouth[0],mouth[6]);
    double outside_high = getDistance(mouth[3],mouth[9]);
    double open_mouth_inside = inside_high / inside_width;
    double open_mouth_outside = outside_high / outside_width;
    return (open_mouth_inside + open_mouth_outside) / 2;
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


/**
 * @brief comp_mouth 计算vector中张口的帧数
 * @param frame
 * @return
 */
bool comp_mouth(one_frame_data frame)
{
    return frame.mouth_status;
}


/**
 * @brief comp_eye 计算vector中闭眼的帧数
 * @param frame
 * @return
 */
bool comp_eye(one_frame_data frame)
{
    return frame.eye_status;
}



/**
 * @brief comp_head_pose 通过vector计算是否有点头行为
 * @param frame_vector 窗口
 * @param head_pose_init 初始相机角度
 * @param window_size //窗口大小
 * @return
 */
bool comp_head_pose(std::vector<one_frame_data> &frame_vector)
{
     QSettings *configIniRead = new QSettings("config.ini", QSettings::IniFormat);
     double head_pose_init = configIniRead->value("/vedio/head_pose_init").toDouble();//取出相应的初始角,直接在改变
     delete configIniRead;//释放内存
     double sum = std::accumulate(std::begin(frame_vector), std::end(frame_vector), 0.0,AddFun());
     //sum值在仰头时快速下降到负数.在低头时快速上升到正数.
     double sum2 = sum - frame_vector.size() * head_pose_init;
     //减去摄像头位置的初始角度
     //std::cout<<sum<<" "<<sum2<<" "<<frame_vector.size()<<" "<<head_pose_init<<std::endl;
     //如果和值在去除摄像头的初始值后依然超过3度,判定低头
     if(sum2 > 4.0*frame_vector.size()){
         return false;
     }else{
         return true;
     }
}
