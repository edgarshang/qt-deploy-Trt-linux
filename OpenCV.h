#ifndef OPENCV_H
#define OPENCV_H

#include <Interface.h>
#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>

#include <QObject>



class OpenCV: public VideoDecode
{
    Q_OBJECT
public:
    explicit OpenCV(const QString &path);
    virtual void DecodeImage() override;

    ~OpenCV();

    void startOpenVideo();


signals:
    void videoOpenResult(bool success, const QString &errorMsg = "");
    void openVideoInThread(const QString &path);

private slots:
    void onOpenVideo(const QString &path);

private:
    cv::VideoCapture m_cap;
    bool m_isOpend = false;
    QThread *m_workerThread = nullptr;
    void cvMatToQImage(const cv::Mat &mat, QImage &qimg);

};

#endif // OPENCV_H
