//
// Created by chao.ma on 2021/8/9.
//

#ifndef FFMPEGBASE_PLAYER_H
#define FFMPEGBASE_PLAYER_H

#include "JavaCall.h"

extern "C" {
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
}


class Player {
public:

    Player(const char *path, JavaCall *javaCall);

    const char * getEncoderInfo();
    int prepare();
    void close();
//    void play()

    ~Player();
private:
    const char *p_path;
    JavaCall *p_javaCall;
    AVFormatContext *avFormatContext;
    AVCodecContext  *avCodecContext;
    AVCodecParameters *avCodecParameters;
    AVCodec *avCodec;

    int videoTrackIndex = -1;
    int audioTrackIndex = -1;

    int p_width = 0;
    int p_height = 0;

};


#endif //FFMPEGBASE_PLAYER_H
