cmake_minimum_required(VERSION 3.13)
project(Driver_Detection)

set(CMAKE_CXX_STANDARD 11)


add_executable(Driver_Detection main.cpp DlibInit.h DlibInit.cpp utils/ConfigManager.cpp utils/ConfigManager.h utilsFunction.cpp utilsFunction.h)

find_package(dlib REQUIRED)
target_link_libraries(${PROJECT_NAME} dlib::dlib)

find_package(OpenCV REQUIRED core highgui imgproc calib3d objdetect)
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})

SET(CMAKE_BUILD_TYPE "Release")
