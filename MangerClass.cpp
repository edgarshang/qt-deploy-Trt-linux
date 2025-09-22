#include "MangerClass.h"

MangerClass::MangerClass(QObject *parent) : QObject(parent)
{
    opencv = new OpenCV("/home/add/project/trt-model-deploy/pic/lxyy.mp4");
    connect(opencv, &OpenCV::frameReady, &m_ui, &QDeployUi::onFrameReady, Qt::QueuedConnection);

    infer = new Yolov5("/home/add/project/trt-model-deploy/model/yolov5s.engine", "/home/add/project/trt-model-deploy/model/classes.txt");
    opencv->setImageModelInfer(infer);
    opencv->DecodeImage();

}

void MangerClass::show()
{
    m_ui.show();
}

MangerClass::~MangerClass()
{
//    delete opencv;
//    delete infer;
}
