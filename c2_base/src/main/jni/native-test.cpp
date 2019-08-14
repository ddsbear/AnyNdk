#include <jni.h>
#include <string>
#include "log.h"


int ret = -1;

extern "C"
JNIEXPORT void JNICALL
Java_com_dds_c2_1base_Utils_test(JNIEnv *env, jclass type, jstring _str, jint a, jlong b) {
    const char *str = env->GetStringUTFChars(_str, JNI_FALSE);
    env->ReleaseStringUTFChars(_str, str);
}


extern "C"
JNIEXPORT void JNICALL
Java_com_dds_c2_1base_Utils_LogInit(JNIEnv *env, jobject instance, jstring logFilePath,
                                    jstring logName, jint logfileLevel, jint logScreenLevel) {
    if (ret != 0) {
        const char *path = env->GetStringUTFChars(logFilePath, JNI_FALSE);
        const char *name = env->GetStringUTFChars(logName, JNI_FALSE);
        int fileLevel = logfileLevel;
        int screenLevel = logScreenLevel;
        ret = _LogInit(path, name, fileLevel, screenLevel);
        env->ReleaseStringUTFChars(logFilePath, path);
        env->ReleaseStringUTFChars(logName, name);
    }

}

extern "C"
JNIEXPORT void JNICALL
Java_com_dds_c2_1base_Utils_log(JNIEnv *env, jclass type, jint _level, jstring _str) {
    if (ret != 0) {
        LOGE("log error! LogInit need");
        return;
    }
    const char *str = env->GetStringUTFChars(_str, JNI_FALSE);
    WriteTextLog(_level, str);
    env->ReleaseStringUTFChars(_str, str);
}


extern "C"
JNIEXPORT void JNICALL
Java_com_dds_c2_1base_Utils_logClose(JNIEnv *env, jclass type) {
    _LogClose();
    ret = -1;
}



