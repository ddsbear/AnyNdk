#include <jni.h>
#include <cstring>
#include "log.h"

JNIEXPORT void JNICALL
Java_com_dds_gmssl_GmCipher_LogInit(JNIEnv *env, jclass instance, jstring _dir, jint _fileLevel,
                                    jint _screenLevel) {
    const char *dir = env->GetStringUTFChars(_dir, JNI_FALSE);

    LOGD("LogInit..._fileLevel:%d..._screenLevel:%d", _fileLevel, _screenLevel);

    env->ReleaseStringUTFChars(_dir, dir);

}