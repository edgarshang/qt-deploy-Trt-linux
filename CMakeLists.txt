cmake_minimum_required(VERSION 3.5)

project(trtDeploy LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check http://doc.qt.io/qt-5/deployment-android.html for more information.
# They need to be set before the find_package(Qt5 ...) call.

#if(ANDROID)
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    if (ANDROID_ABI STREQUAL "armeabi-v7a")
#        set(ANDROID_EXTRA_LIBS
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so)
#    endif()
#endif()



find_package(Qt5 COMPONENTS Widgets REQUIRED)

set(OPENCV_INCLUE_DIR /usr/include/opencv4)
include_directories(${OPENCV_INCLUE_DIR})

find_package(OpenCV REQUIRED)
# 打印 OpenCV 信息（可选）
message(STATUS "OpenCV Version: ${OpenCV_VERSION}")
message(STATUS "OpenCV Include Dir: ${OpenCV_INCLUDE_DIRS}")
message(STATUS "OpenCV Libraries: ${OpenCV_LIBS}")

set(CUDA_INCLUDE_DIR /usr/local/cuda-11.4/targets/x86_64-linux/include)
set(CUDA_LIBRARY_DIR /usr/local/cuda-11.4/targets/x86_64-linux/lib)
link_directories(${CUDA_LIBRARY_DIR})
include_directories(${CUDA_INCLUDE_DIR})
link_libraries(cuda  cufft cudart  cublas curand  cudnn nvinfer nvonnxparser nvinfer_plugin  nvparsers)

set(TRT_INCLUDE_DIR /home/add/TensorRT/TensorRT-8.6.1.6/include)
set(TRT_LIBRARY_DIR /home/add/TensorRT/TensorRT-8.6.1.6/lib)
link_directories(${TRT_LIBRARY_DIR})
include_directories(${TRT_INCLUDE_DIR})


if(ANDROID)
  add_library(trtDeploy SHARED
    main.cpp
    QDeployUi.cpp
    QDeployUi.h
    QDeployUi.ui
  )
else()
  add_executable(trtDeploy
    main.cpp
    QDeployUi.cpp
    QDeployUi.h
    QDeployUi.ui
    Interface.h
    FFmpeg.cpp
    FFmpeg.h
    OpenCV.cpp
    OpenCV.h
    MangerClass.cpp
    MangerClass.h
    Yolov5.cpp
    Yolov5.h
    CommonAPI.cpp
    CommonAPI.h
  )
endif()

target_link_libraries(trtDeploy PRIVATE Qt5::Widgets ${OpenCV_LIBS})
