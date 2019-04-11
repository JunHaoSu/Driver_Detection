//
// Created by sujunhao on 19-3-24.
//

#include "DlibInit.h"
#include "utils/ConfigManager.h"
#include "utilsFunction.h"
using namespace dlib;
using namespace cv;

/**
 *  构造函数,对象初始化时将需要的变量进行初始化
 */
DlibInit::DlibInit()
{
    //从配置文件调取6点模型文件路径
    ConfigManager *config = ConfigManager::GetInstance();
    std::string predictor_face_model_path = config->get_date("face_model_path").c_str();
    dlib::deserialize(predictor_face_model_path) >> predictor;
    detector = dlib::get_frontal_face_detector();
    init_cam_intrinsics();
    init_object_pts();
}

/**
 * 析构函数
 */
DlibInit::~DlibInit() {
//这个对象长期存在,不销毁
}

/**
 * 初始化3D模型
 */
void DlibInit::init_object_pts()
{
    object_pts.push_back(cv::Point3d(6.825897, 6.760612, 4.402142));     //#33 left brow left corner
    object_pts.push_back(cv::Point3d(1.330353, 7.122144, 6.903745));     //#29 left brow right corner
    object_pts.push_back(cv::Point3d(-1.330353, 7.122144, 6.903745));    //#34 right brow left corner
    object_pts.push_back(cv::Point3d(-6.825897, 6.760612, 4.402142));    //#38 right brow right corner
    object_pts.push_back(cv::Point3d(5.311432, 5.485328, 3.987654));     //#13 left eye left corner
    object_pts.push_back(cv::Point3d(1.789930, 5.393625, 4.413414));     //#17 left eye right corner
    object_pts.push_back(cv::Point3d(-1.789930, 5.393625, 4.413414));    //#25 right eye left corner
    object_pts.push_back(cv::Point3d(-5.311432, 5.485328, 3.987654));    //#21 right eye right corner
    object_pts.push_back(cv::Point3d(2.005628, 1.409845, 6.165652));     //#55 nose left corner
    object_pts.push_back(cv::Point3d(-2.005628, 1.409845, 6.165652));    //#49 nose right corner
    object_pts.push_back(cv::Point3d(2.774015, -2.080775, 5.048531));    //#43 mouth left corner
    object_pts.push_back(cv::Point3d(-2.774015, -2.080775, 5.048531));   //#39 mouth right corner
    object_pts.push_back(cv::Point3d(0.000000, -3.116408, 6.097667));    //#45 mouth central bottom corner
    object_pts.push_back(cv::Point3d(0.000000, -7.415691, 4.070434));    //#6 chin corner

}

/**
 * 填写相机内联函数和失真系数
 */
void DlibInit::init_cam_intrinsics()
{
//相机内参数矩阵
    static double K[9] = { 6.5308391993466671e+002, 0.0, 3.1950000000000000e+002,
             0.0, 6.5308391993466671e+002, 2.3950000000000000e+002,
             0.0, 0.0, 1.0 };
    //畸变系数
    static  double D[5] = { 7.0834633684407095e-002, 6.9140193737175351e-002, 0.0,
             0.0, -1.3073460323689292e+000 };

    cam_parameter1.cam_matrix = cv::Mat(3, 3, CV_64FC1, K);//相机内参数矩阵
    cam_parameter1.dist_coeffs = cv::Mat(5, 1, CV_64FC1, D);//畸变系数

}

/**
 * 图像预处理函数
 * @param img
 */
void  DlibInit::image_processing(cv::Mat &img)
{
    cv::cvtColor(img,img,CV_BGR2GRAY);//转化成灰度图
}

/**
 * 68个特征点提取函数
 * @param src_img
 */
void DlibInit::find_68_point(cv::Mat &src_img)
{
//图像初始化
    image_processing(src_img);
    //将opencv图像转化成dlib图像
    dlib::cv_image<uchar> cimg(src_img);
    //使用人脸检测器检测人脸
    std::vector<dlib::rectangle> faces = detector(cimg);
    //std::vector<cv::Point2d> image_pts;
    if (faces.size() > 0) {
        //使用关键点检测器检测人脸的68点
        dlib::full_object_detection shape = predictor(cimg, faces[0]);
        for (int i = 0; i < 68; i++) {
            face_68_point.push_back(cv::Point2d(shape.part(i).x(), shape.part(i).y()));
        }
    }
}

/**
 * 头部姿态估计,并且计算三个欧拉角
 */
cv::Mat DlibInit::cal_detect_angle()
{
    //decomposeProjectionMatrix()函数的temp buf
    cv::Mat out_intrinsics = cv::Mat(3, 3, CV_64FC1);
    cv::Mat out_rotation = cv::Mat(3, 3, CV_64FC1);
    cv::Mat out_translation = cv::Mat(3, 1, CV_64FC1);
    //text on screen

    //result
    cv::Mat rotation_vec;//旋转向量
    cv::Mat translation_vec;//平移向量
    cv::Mat rotation_mat;//旋转矩阵
    cv::Mat pose_mat = cv::Mat(3, 4, CV_64FC1);     //3 x 4 R | T
    cv::Mat euler_angle = cv::Mat(3, 1, CV_64FC1, {90,90,90});//初始化欧拉角为90,如果显示90代表未检测到人脸


    //拿到所有的68个点的位置

    if(face_68_point.empty())
        return euler_angle;
    //拿到定义计算姿态的2D点集
    std::vector<cv::Point2d> img_pts_need;
    img_pts_need.push_back(face_68_point[17]); //#17 left brow left corner
    img_pts_need.push_back(face_68_point[21]); //#21 left brow right corner
    img_pts_need.push_back(face_68_point[22]); //#22 right brow left corner
    img_pts_need.push_back(face_68_point[26]); //#26 right brow right corner
    img_pts_need.push_back(face_68_point[36]); //#36 left eye left corner
    img_pts_need.push_back(face_68_point[39]); //#39 left eye right corner
    img_pts_need.push_back(face_68_point[42]); //#42 right eye left corner
    img_pts_need.push_back(face_68_point[45]); //#45 right eye right corner
    img_pts_need.push_back(face_68_point[31]); //#31 nose left corner
    img_pts_need.push_back(face_68_point[35]); //#35 nose right corner
    img_pts_need.push_back(face_68_point[48]); //#48 mouth left corner
    img_pts_need.push_back(face_68_point[54]); //#54 mouth right corner
    img_pts_need.push_back(face_68_point[57]); //#57 mouth central bottom corner
    img_pts_need.push_back(face_68_point[8]);   //#8 chin corner
    /*solvePnp函数参数如下
    object_pts 3D集合
    img_pts_need 2D集合
    cam_matrix 相机内参数矩阵
    dist_coeffs 畸变系数
    rotation_vec 得到的结果 旋转矩阵
    translation_vec 得到的结果 平移矩阵
     */
    cv::solvePnP(object_pts, img_pts_need, cam_parameter1.cam_matrix, cam_parameter1.dist_coeffs, rotation_vec, translation_vec);
    //求出欧拉角
    //罗德里格斯变换

    cv::Rodrigues(rotation_vec, rotation_mat);
    //垂直拼接 pose_mat = [ rataton_mat translation_vec ]
    cv::hconcat(rotation_mat, translation_vec, pose_mat);
    // pose_mat 输入3*4投影矩阵
    // out_intrinsics 输出3*3 相机矩阵
    // out_rotation 输出3*3 外部旋转矩阵
    // out_translation 输出4*1 平移向量
    // eulerAngles 包含三个以度为单位的欧拉旋转角
    cv::decomposeProjectionMatrix(pose_mat, out_intrinsics, out_rotation, out_translation, cv::noArray(), cv::noArray(), cv::noArray(), euler_angle);

    return euler_angle;
}

/**
 *  ear值算法 左上右下+右上左下的距离除以左右眼角的两倍
    大于0.3代表眼睛睁开
    :param eye:眼睛特征点坐标数组
    :return:ear值*/
double DlibInit::eye_aspect_ratio()
{
    if (!face_68_point.empty()){
        std::vector<cv::Point2d> right_eye(&face_68_point[36], &face_68_point[42]);
        std::vector<cv::Point2d> left_eye(&face_68_point[42], &face_68_point[48]);
        double left_ear = cal_ear(left_eye);
        double right_ear = cal_ear(right_eye);
        double ear = (left_ear + right_ear)/2.0;
        return ear;
    }else{
        return 0.0;
    }
}


double DlibInit::mouth_aspect_ratio()
{
    if(!face_68_point.empty()){
        std::vector<cv::Point2d> mouth_point(&face_68_point[48],&face_68_point[68]);
        return cal_mouth(mouth_point);
    }else{
        return 0;
    }
}


/**
 * 读取下一帧视频时将上一帧的68点数据清空*/
void DlibInit::clear_68_point()
{
    face_68_point.clear();
}