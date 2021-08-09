//
// Created by chao.ma on 2021/8/9.
//

#ifndef FFMPEGBASE_JAVACALL_H
#define FFMPEGBASE_JAVACALL_H
#include <jni.h>
class JavaCall {
public:
    JavaCall(JavaVM *vm, JNIEnv *env, jobject &_job);
    void callWH(int width, int height);

private:
    JavaVM *p_vm;
    JNIEnv *p_env;
    jobject obj;

};


#endif //FFMPEGBASE_JAVACALL_H
