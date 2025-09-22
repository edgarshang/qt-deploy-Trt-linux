#ifndef FFMPEG_H
#define FFMPEG_H
#include <Interface.h>

class FFmpeg:public VideoDecode
{
public:
    FFmpeg();
    virtual void DecodeImage() override;
};

#endif // FFMPEG_H
