cmake_minimum_required(VERSION 2.8)
project(Driver_Detection)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_PREFIX_PATH "/home/sujunhao/Qt/5.11.2/gcc_64")
set(CMAKE_INCLUDE_CURRENT_DIR ON) 
set(CMAKE_AUTOMOC ON) 
set(CMAKE_AUTOUIC ON) 
set(CMAKE_AUTORCC ON)

find_package(Qt5 REQUIRED Core Gui Widgets)
find_package(dlib REQUIRED)
find_package(OpenCV REQUIRED core highgui imgproc calib3d objdetect)

qt5_wrap_cpp(MOC mainwindow.h)
qt5_wrap_ui(UIC mainwindow.ui)
qt5_add_resources(QRC_FILES menu_icon.qrc )
add_executable(${PROJECT_NAME} main.cpp mainwindow.cpp mainwindow.h mainwindow.ui configmanager.h configmanager.cpp
    dlibinit.h dlibinit.cpp utilsfunction.h utilsfunction.cpp videoprocessor.cpp videoprocessor.h ${MOC} ${UIC} ${QRC_FILES})

target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Gui Qt5::Widgets)
target_link_libraries(${PROJECT_NAME} dlib::dlib)
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})

SET(CMAKE_BUILD_TYPE "Release")

