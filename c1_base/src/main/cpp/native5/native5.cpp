#include <jni.h>
#include <string>
#include <android/log.h>
#include "Instance.h"

#define  LOG_TAG    "dds_native5"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,  LOG_TAG, __VA_ARGS__ )


extern "C"
JNIEXPORT void JNICALL
Java_com_dds_anyndk_AnyNdk_native5(JNIEnv *env, jclass type) {

    Instance *instance = Instance::getInstance();
    instance->printHello();


}

