#include <jni.h>
#include <string>
#include <android/log.h>

#ifdef __cplusplus
extern "C" {
#endif

#define  LOG_TAG    "dds_native4"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,  LOG_TAG, __VA_ARGS__ )

// 动态注册

void dynamicNative1(JNIEnv *env, jobject jobj) {
    LOGD("dynamicNative1 动态注册");
}

jstring dynamicNative2(JNIEnv *env, jobject jobj, jint i) {
    char returnStr[100];
    sprintf(returnStr, "我是动态注册的dynamicNative2方法:%d", i);

//    jstring str = env->NewStringUTF(returnStr);
//    const char *c_str = env->GetStringUTFChars(str, JNI_FALSE);

    LOGD("%s", returnStr);
    return env->NewStringUTF(returnStr);
}

//需要动态注册的方法数组
static const JNINativeMethod mMethods[] = {
        {"dynamicNative", "()V",                   (void *) dynamicNative1},
        {"dynamicNative", "(I)Ljava/lang/String;", (jstring *) dynamicNative2}

};

//需要动态注册native方法的类名
static const char *mClassName = "com/dds/anyndk/AnyNdk";

jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv *env = NULL;
    //获得 JniEnv
    int r = vm->GetEnv((void **) &env, JNI_VERSION_1_4);
    if (r != JNI_OK) {
        return -1;
    }
    jclass activityCls = env->FindClass(mClassName);
    // 注册 如果小于0则注册失败
    r = env->RegisterNatives(activityCls, mMethods, 2);
    if (r != JNI_OK) {
        return -1;
    }
    return JNI_VERSION_1_4;
}


#ifdef __cplusplus
}
#endif
