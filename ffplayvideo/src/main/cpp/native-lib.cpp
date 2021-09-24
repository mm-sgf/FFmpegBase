#include <jni.h>
#include <string>
#include "Player.h"
#include "clog.h"

Player *p_player;
JavaCall *p_javaCall;
JavaVM *jvm;

//RTMPPacket释放
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
    jvm = vm;
    LOGE("保存虚拟机的引用");
    return JNI_VERSION_1_4;
}

extern "C"
JNIEXPORT jint JNICALL
Java_com_cfox_ffplayvideo_PlayManager_native_1prepare(JNIEnv *env, jobject thiz, jstring path) {

    const char *c_path = env->GetStringUTFChars(path, 0);
    if (p_player) {
        p_player->close();
        delete p_player;
    }

    p_javaCall = new JavaCall(jvm, env, thiz);
    p_player = new Player(c_path, p_javaCall);
    p_player->prepare();
    env->ReleaseStringUTFChars(path, c_path);

    LOGE("prepare success handler id ");

    return reinterpret_cast<jint>(p_player);
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_cfox_ffplayvideo_PlayManager_natvie_1get_1encoder_1info(JNIEnv *env, jobject thiz) {
    if (p_player) {
        std::string info = p_player->getEncoderInfo();
        return env->NewStringUTF(info.c_str());
    }
    return nullptr;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_cfox_ffplayvideo_PlayManager_native_1close(JNIEnv *env, jobject thiz) {
    if (p_player) {
        p_player->close();
        delete p_player;
        p_player = nullptr;
    }

    delete p_javaCall;
    p_javaCall = nullptr;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_cfox_ffplayvideo_PlayManager_native_1play(JNIEnv *env, jobject thiz, jobject surface) {


}