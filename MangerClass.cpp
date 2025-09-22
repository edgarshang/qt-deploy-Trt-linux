#include "MangerClass.h"

MangerClass::MangerClass(QObject *parent) : QObject(parent)
{
    opencv = new OpenCV("/home/add/project/trt-model-deploy/pic/lxyy.mp4");
    connect(opencv, &OpenCV::frameReady, &m_ui, &QDeployUi::onFrameReady, Qt::QueuedConnection);
    opencv->DecodeImage();
}

void MangerClass::show()
{
    m_ui.show();
}
