//
// Created by chao.ma on 2021/8/9.
//

#include "Player.h"
#include "clog.h"

Player::Player(const char *path, JavaCall *javaCall) : p_path(path), p_javaCall(javaCall){
}

int Player::prepare() {
    // 注册所有组件，这个api在新的版本上面可以不写
//    avcodec_register_all();

    //实例化 format 上下文
    avFormatContext = avformat_alloc_context();

    // 打开视频流
    int openCode = avformat_open_input(&avFormatContext, p_path, nullptr, nullptr);
    if (openCode != 0) {
        LOGE("open file fail code : %d" , openCode);
        return -1;
    }

    LOGE("open  file  success ....openCode :%d", openCode);

    // 查找文件流信息
    int streamInfoCode = avformat_find_stream_info(avFormatContext, nullptr);
    if (streamInfoCode < 0 ) {
        LOGE("find stream info fail : %d" , streamInfoCode);
        return -2;
    }

    LOGE("find stream info success stream info code : %d", streamInfoCode);

    // 获取 track
    LOGE("media stream cont : %d",  avFormatContext->nb_streams);
    for (int i = 0; i < avFormatContext->nb_streams; i++) {
        /**
         * enum AVMediaType {
            AVMEDIA_TYPE_UNKNOWN = -1,  ///< Usually treated as AVMEDIA_TYPE_DATA
            AVMEDIA_TYPE_VIDEO,
            AVMEDIA_TYPE_AUDIO,
            AVMEDIA_TYPE_DATA,          ///< Opaque data information usually continuous
            AVMEDIA_TYPE_SUBTITLE,
            AVMEDIA_TYPE_ATTACHMENT,    ///< Opaque data information usually sparse
            AVMEDIA_TYPE_NB
            };
         */
        if (avFormatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            videoTrackIndex = i;
        } else if (avFormatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
            audioTrackIndex = i;
        }
    }

    if (videoTrackIndex >= 0) {
        avCodecContext = avFormatContext->streams[videoTrackIndex]->codec;
//        avCodec = avcodec_find_decoder(avCodecContext->codec_id);
        LOGE("open codec 111 codec_id : %d" , avCodecContext->codec_id);
        // 获取当前媒体 stream 参数
        avCodecParameters = avFormatContext->streams[videoTrackIndex]->codecpar;
        // 通过 codec_id 获取
        avCodec = avcodec_find_decoder(avCodecParameters->codec_id);
        LOGE("open codec 222 codec_id : %d" , avCodecParameters->codec_id);

        int openCodecCode = avcodec_open2(avCodecContext, avCodec, nullptr);
        if (openCodecCode < 0) {
            LOGE("open codec code : %d" , openCodecCode);
            return -3;
        }
        LOGE("open codec success code : %d" , openCodecCode);
        p_width = avCodecParameters->width;
        p_height = avCodecParameters->height;
        LOGE("video width :%d height : %d" , p_width, p_height);
        p_javaCall->callWH(p_width, p_height);
    }

    return 0;
}

Player::~Player() {

}

void Player::close() {

}

const char *Player::getEncoderInfo() {
    return avcodec_configuration();
}
