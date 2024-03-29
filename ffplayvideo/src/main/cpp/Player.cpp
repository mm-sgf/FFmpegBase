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
        frame_rate = avCodecContext->framerate.num / avCodecContext->framerate.den;
        frame_time = 1000000 / frame_rate;
        LOGE("video width :%d height : %d  frame_rate: %d  frame_time:%d " , p_width, p_height, frame_rate, frame_time);
        p_javaCall->callWH(p_width, p_height);
    }

    return 0;
}

int Player::play(JNIEnv *env, jobject surface) {
    // 获取上面传下来的surface
    nativeWindow = ANativeWindow_fromSurface(env, surface);

    if (nativeWindow == nullptr) {
        LOGE("Couldn't get native window from surface.\n");
        return -1;
    }

    // 创建三个容器
    //ffmpeg实例化  avFrame   实例化他的大小
    avFrame = av_frame_alloc();
    //ffmpeg实例化  avFrame
    avPacket = av_packet_alloc();
    //实例化    rgb
    rgbFrame = av_frame_alloc();



    // 获取指定宽高大小，一帧图片size
    int numBytes = av_image_get_buffer_size(AV_PIX_FMT_RGBA, p_width, p_height, 1);

    LOGE("1 frame size: %d" , numBytes);

    // 申请输出buffer 内存， numBytes * sizeof(uint8_t)
    outBuffer = static_cast<uint8_t *>(av_malloc(numBytes * sizeof(uint8_t)));

    // 缓冲区设置给 rgbframe
    av_image_fill_arrays(rgbFrame->data, rgbFrame->linesize, outBuffer,
                         AV_PIX_FMT_RGBA, p_width, p_height, 1);

    // 获取转换的上下文
    swsContext = sws_getContext(p_width, p_height, avCodecContext->pix_fmt,
                                p_width, p_height, AV_PIX_FMT_RGBA,
                                SWS_BICUBIC, NULL, NULL, NULL);

    int nativeWindowCode = ANativeWindow_setBuffersGeometry(nativeWindow, p_width, p_height, WINDOW_FORMAT_RGBA_8888);
    if (0 > nativeWindowCode) {
        LOGE("Couldn't set buffers geometry.\n");
        ANativeWindow_release(nativeWindow);
        return -2;
    }
    LOGE("ANativeWindow_setBuffersGeometry成功\n");

    while (av_read_frame(avFormatContext, avPacket) >=0) {// 读出一帧数据
        // 读出来的数据是否是视频数据
        if (avPacket->stream_index == videoTrackIndex) {
            int ret = avcodec_send_packet(avCodecContext, avPacket);// 解码数据
            if(ret < 0 && ret != AVERROR(EAGAIN) && ret != AVERROR_EOF) {
                LOGE("解码出错");
                return -3;
            }

            ret = avcodec_receive_frame(avCodecContext, avFrame);// 获取解码数据
            if (ret == AVERROR(EAGAIN)) {
                continue;
            } else if (ret < 0) {
                break;
            }

            // 将解码数据进行转换，转换surface可播放的数据
            sws_scale(swsContext,
                      avFrame->data, avFrame->linesize, 0, avCodecContext->height,
                      rgbFrame->data,rgbFrame->linesize );

            if (ANativeWindow_lock(nativeWindow, &windowBuffer, NULL) < 0) {
                LOGE("cannot lock window");
            } else {
                uint8_t *dst = static_cast<uint8_t *>(windowBuffer.bits);
                for (int h  = 0; h < p_height; h++ ) {
                    memcpy(dst + h * windowBuffer.stride * 4,
                           outBuffer + h * rgbFrame->linesize[0],
                           rgbFrame->linesize[0]);
                    switch(avFrame->pict_type){
                        case AV_PICTURE_TYPE_I:
                            LOGE("I");
                            break;
                        case AV_PICTURE_TYPE_P:
                            LOGE("P");
                            break;
                        case AV_PICTURE_TYPE_B:
                            LOGE("B");
                            break;
                        default:
                            ;break;
                    }
                }

                av_usleep(frame_time);
                ANativeWindow_unlockAndPost(nativeWindow);
            }
        }
    }
    avformat_free_context(avFormatContext);

    return 0;
}

Player::~Player() {

}

void Player::close() {

}

const char *Player::getEncoderInfo() {
    return avcodec_configuration();
}
