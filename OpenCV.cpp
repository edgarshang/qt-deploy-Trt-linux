#include "OpenCV.h"
#include <QDebug>


OpenCV::OpenCV(const QString &path): VideoDecode(path)
{
    m_workerThread = new QThread();
    this->moveToThread(m_workerThread);
    connect(m_workerThread, &QThread::finished, this, &QObject::deleteLater);
    connect(this, &OpenCV::openVideoInThread, this, &OpenCV::onOpenVideo,
            Qt::QueuedConnection);

     m_workerThread->start();
}


OpenCV::~OpenCV()
{
    m_workerThread->quit();
    m_workerThread->wait();
    delete m_workerThread;
    m_cap.release();
    qDebug() << "~OpenCV()";
}

void OpenCV::DecodeImage()
{
//    Q_UNUSED
    qDebug() << "DecodeImage";
    qDebug() << "DecodeImage 线程ID: " << QThread::currentThreadId();
    emit openVideoInThread(VideoPath);
//    startOpenVideo();
//    start();
}

void OpenCV::startOpenVideo()
{
    qDebug() << "开始异步打开视频 子线程ID: " << QThread::currentThreadId();

}

// 转换函数：通过引用传参，直接修改qimg
void OpenCV::cvMatToQImage(const cv::Mat &mat, QImage &qimg)
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

void OpenCV::onOpenVideo(const QString &path)
{
     qDebug() << "正在子线程打开视频 (子线程ID:" << QThread::currentThreadId() << ")";
    m_cap.open(path.toStdString());

    if(m_cap.isOpened())
    {
        m_isOpend = true;
//        emit videoOpenResult(true); // 发送成功信号
        qDebug() << "视频打开成功，路径:" << path;
        // 获取视频属性
          int frame_width = static_cast<int>(m_cap.get(cv::CAP_PROP_FRAME_WIDTH));
          int frame_height = static_cast<int>(m_cap.get(cv::CAP_PROP_FRAME_HEIGHT));
          double fps = m_cap.get(cv::CAP_PROP_FPS);
          int total_frames = static_cast<int>(m_cap.get(cv::CAP_PROP_FRAME_COUNT));

          std::cout << "视频信息:" << std::endl;
          std::cout << "宽度: " << frame_width << " 高度: " << frame_height << std::endl;
          std::cout << "帧率: " << fps << " FPS" << std::endl;
          std::cout << "总帧数: " << total_frames << std::endl;

          cv::Mat frame; // 存储每一帧的图像
          m_cap >> frame; // 读取一

          while (!frame.empty())
          {
              imageInfer->imageHandleInference(frame);

              // call the GPU handling the image
              emit frameReady(frame);
              double fps = m_cap.get(cv::CAP_PROP_FPS);
              int delay = static_cast<int>(1000 / fps);
              QThread::msleep(delay); // 毫秒级延时（1.5秒）
              m_cap >> frame;
           }

    }else
    {
        m_isOpend = false;
        qDebug() << "视频打开失败，路径:" << path;
    }
}
