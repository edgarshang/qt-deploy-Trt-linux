#ifndef INTERFACE_H
#define INTERFACE_H

#include <QLabel>
#include <QString>
#include <QDebug>
#include <QFile>
#include <QThread>
#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>



class Show
{
public:
    virtual void showImage(QImage &img) = 0;
};


class VideoDecode:public QObject
{
    Q_OBJECT
public:
    virtual void DecodeImage() = 0;

    VideoDecode(const QString &videoPath):VideoPath(videoPath)
    {
        qDebug() << "videoDecode init";
        isVideoExist =  QFile::exists(VideoPath);
        if(isVideoExist)
        {
            qDebug() << "video file is exist";
        }else
        {
            qDebug() << "video file is not exist";
        }
    }
    ~VideoDecode() = default;
    bool isVideoExist = false;
    QString VideoPath;
signals:
    // 信号：发送视频帧给主线程（参数为处理后的视频帧）
    void frameReady(const cv::Mat& frame); // cv::Mat 可直接作为信号参数（Qt 支持）


};

#endif // INTERFACE_H
