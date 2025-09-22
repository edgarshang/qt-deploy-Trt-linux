#ifndef QDEPLOYUI_H
#define QDEPLOYUI_H

#include <QWidget>
#include <Interface.h>
#include <opencv2/opencv.hpp>

QT_BEGIN_NAMESPACE
namespace Ui { class QDeployUi; }
QT_END_NAMESPACE

class QDeployUi : public QWidget
{
    Q_OBJECT

public:
    QDeployUi(QWidget *parent = nullptr);
    void cvMatToQImage(const cv::Mat &mat, QImage &qimg);
//    void cvMatToQImage(const cv::Mat &mat, QImage &qimg);
    ~QDeployUi();

private:
    Ui::QDeployUi *ui;

public slots:
    // 主线程槽函数：接收子线程的帧数据并更新 UI
    void onFrameReady(const cv::Mat& frame);
};
#endif // QDEPLOYUI_H
