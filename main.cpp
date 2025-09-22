#include "QDeployUi.h"

#include <QApplication>
#include <Interface.h>
#include <OpenCV.h>
#include <memory>
#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include "MangerClass.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qRegisterMetaType<cv::Mat>("cv::Mat");

    int ret;
    MangerClass manger;

    manger.show();

    ret = a.exec();
    return ret;
}
