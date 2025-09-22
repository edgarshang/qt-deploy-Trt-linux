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
//    cv::Mat copyFrame = frame.clone();
////    qDebug() << "showing  (子线程ID:" << QThread::currentThreadId() << ")";
//        QImage dst(copyFrame.data, copyFrame.cols, copyFrame.rows, static_cast<int>(copyFrame.step), QImage::Format::Format_RGB888);
//        ui->ImageLable->setPixmap(QPixmap::fromImage(dst.rgbSwapped()));


        if (frame.empty()) return; // 避免空图像处理

           // 1. 克隆图像并转换为QImage（保持原始比例）
           cv::Mat copyFrame = frame.clone();
           QImage qImg(copyFrame.data, copyFrame.cols, copyFrame.rows,
                       static_cast<int>(copyFrame.step), QImage::Format_RGB888);
           // OpenCV默认BGR格式，转换为RGB以便Qt正确显示
           qImg = qImg.rgbSwapped();

           // 2. 获取QLabel的当前实际大小（考虑布局和窗口缩放）
           QSize labelSize = ui->ImageLable->size();

           // 3. 缩放图像以适应QLabel，保持宽高比（避免拉伸变形）
           QImage scaledImg = qImg.scaled(labelSize,
                                         Qt::KeepAspectRatio,  // 保持比例
                                         Qt::SmoothTransformation); // 平滑缩放，画质更好

           // 4. 设置缩放后的图像到QLabel
           ui->ImageLable->setPixmap(QPixmap::fromImage(scaledImg));
}


