#include "QDeployUi.h"
#include "./ui_QDeployUi.h"
#include <QDebug>


QDeployUi::QDeployUi(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::QDeployUi)
{
    ui->setupUi(this);
    ui->ImageLable->setText("hello, world");
}

void QDeployUi::cvMatToQImage(const cv::Mat &mat, QImage &qimg)
{
    if (mat.empty()) {
        qimg = QImage();  // 空图像处理
        return;
    }

    // OpenCV默认是BGR格式，需要转换为RGB
    cv::Mat rgbMat;
    if (mat.type() == CV_8UC3) {
        cv::cvtColor(mat, rgbMat, cv::COLOR_BGR2RGB);
        // 直接使用mat的数据创建QImage（不拷贝数据，提高效率）
        qimg = QImage(rgbMat.data,
                      rgbMat.cols,
                      rgbMat.rows,
                      rgbMat.step,  // 每行字节数（自动计算）
                      QImage::Format_RGB888);
    }
    // 灰度图处理
    else if (mat.type() == CV_8UC1) {
        qimg = QImage(mat.data,
                      mat.cols,
                      mat.rows,
                      mat.step,
                      QImage::Format_Grayscale8);
    }
}

QDeployUi::~QDeployUi()
{
    delete ui;
}

void QDeployUi::onFrameReady(const cv::Mat& frame)
{
        QImage dst(frame.data, frame.cols, frame.rows, static_cast<int>(frame.step), QImage::Format::Format_RGB888);
        ui->ImageLable->setPixmap(QPixmap::fromImage(dst.rgbSwapped()));
}


