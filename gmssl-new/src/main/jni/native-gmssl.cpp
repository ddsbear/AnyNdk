#include <jni.h>
#include <string>
#include <android/log.h>
#include "cipher.h"

#define  LOG_TAG    "dds_log"

#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,  LOG_TAG, __VA_ARGS__ )


extern "C" JNIEXPORT jstring JNICALL
Java_com_trustmobi_mobishield_GmCipher_test(JNIEnv *env, jclass clazz) {
    LOGD("test");
}

