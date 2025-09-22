#ifndef MANGERCLASS_H
#define MANGERCLASS_H

#include <QObject>
#include "QDeployUi.h"
#include "OpenCV.h"

class MangerClass : public QObject
{
    Q_OBJECT
public:
    explicit MangerClass(QObject *parent = nullptr);
    void show();
signals:


private:
    QDeployUi m_ui;
    VideoDecode *opencv = nullptr;
};

#endif // MANGERCLASS_H
