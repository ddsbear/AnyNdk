#include <jni.h>
#include <string>
#include <android/log.h>
#include <pthread.h>

#define  LOG_TAG    "dds_native4"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,  LOG_TAG, __VA_ARGS__ )


JavaVM *_vm = 0;
jobject _instance = 0;

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
    _vm = vm;
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


void *task(void *args) {
    JNIEnv *env;
    _vm->AttachCurrentThread(&env, 0);

    jclass clazz = env->GetObjectClass(_instance);
    jmethodID methodId = env->GetStaticMethodID(clazz, "staticMethod", "(Ljava/lang/String;IZ)V");
    jstring staticStr = env->NewStringUTF("C++调用静态方法");
    env->CallStaticVoidMethod(clazz, methodId, staticStr, 1, true);
    env->DeleteLocalRef(clazz);
    env->DeleteLocalRef(staticStr);

    _vm->DetachCurrentThread();
    return 0;


}

extern "C"
JNIEXPORT void JNICALL
Java_com_dds_anyndk_AnyNdk_native11_15(JNIEnv *env, jclass type, jobject javaHelper) {
    pthread_t pid;
    _instance = env->NewGlobalRef(javaHelper);
    pthread_create(&pid, 0, task, 0);


}



