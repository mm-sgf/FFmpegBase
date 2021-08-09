//
// Created by chao.ma on 2021/8/9.
//

#include "JavaCall.h"
#include <jni.h>


JavaCall::JavaCall(JavaVM *vm, JNIEnv *env, jobject &_job) : p_vm(vm), p_env(env), obj(_job) {

}
void JavaCall::callWH(int width, int height) {
    jclass clazz = p_env->GetObjectClass(obj);
    jmethodID callId = p_env->GetMethodID(clazz,"videoWH", "(II)V");
    p_env->CallVoidMethod(obj, callId, width, height);
}




