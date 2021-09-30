//
// Created by chao.ma on 2021/8/9.
//

#ifndef FFMPEGBASE_PLAYER_H
#define FFMPEGBASE_PLAYER_H

#include "JavaCall.h"
#include <android/native_window_jni.h>
extern "C" {
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavutil/imgutils.h"
#include "libswscale/swscale.h"
#include <libavutil/time.h>
}


class Player {
public:

    Player(const char *path, JavaCall *javaCall);

    const char * getEncoderInfo();
    int prepare();
    int play(JNIEnv *env, jobject surface);
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

    ANativeWindow* nativeWindow;

    AVPacket *avPacket;
    AVFrame *avFrame , *rgbFrame;

    uint8_t  *outBuffer;

    SwsContext *swsContext;

    int videoTrackIndex = -1;
    int audioTrackIndex = -1;

    int p_width = 0;
    int p_height = 0;

};


#endif //FFMPEGBASE_PLAYER_H
