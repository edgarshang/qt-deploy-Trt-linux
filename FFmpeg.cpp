#include "FFmpeg.h"
#include <QDebug>
#include <libavformat/avformat.h>
FFmpeg::FFmpeg():VideoDecode("tset.mp4")
{
    qDebug() << "FFmpeg init";
//    av_register_all();
//    avformat_network_init();
}

void FFmpeg::DecodeImage()
{
//    Q_UNUSED
    qDebug() << "DecodeImage";
}
