#include <jni.h>
#include <string>
#include <android/log.h>

#define  LOG_TAG    "dds_native4"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,  LOG_TAG, __VA_ARGS__ )


// 动态注册
void dynamicNative1(JNIEnv *env, jobject jobj) {
    LOGD("dynamicNative1 动态注册");
}

jstring dynamicNative2(JNIEnv *env, jobject jobj, jint i) {
    return env->NewStringUTF("我是动态注册的dynamicNative2方法");
}

//需要动态注册的方法数组
static const JNINativeMethod mMethods[] = {
        {"dynamicNative", "()V",                   (void *) dynamicNative1},
        {"dynamicNative", "(I)Ljava/lang/String;", (jstring *) dynamicNative2}

};

//需要动态注册native方法的类名
static const char* mClassName = "com/dds/anyndk/AnyNdk";

jint JNI_OnLoad(JavaVM* vm, void* reserved){
    JNIEnv* env = NULL;
    //获得 JniEnv
    int r = vm->GetEnv((void**) &env, JNI_VERSION_1_4);
    if( r != JNI_OK){
        return -1;
    }
    jclass mainActivityCls = env->FindClass( mClassName);
    // 注册 如果小于0则注册失败
    r = env->RegisterNatives(mainActivityCls,mMethods,2);
    if(r  != JNI_OK )
    {
        return -1;
    }
    return JNI_VERSION_1_4;
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_dds_anyndk_AnyNdk_native11(JNIEnv *env, jclass type, jint a, jstring str_, jfloat f) {
    // 获得c字符串
    const char *str = env->GetStringUTFChars(str_, JNI_FALSE);

    char returnStr[100];
    //格式化字符串
    sprintf(returnStr, "C++ string:%d,%s,%f", a, str, f);

    //释放掉内存
    env->ReleaseStringUTFChars(str_, str);

    return env->NewStringUTF(returnStr);
}


extern "C"
JNIEXPORT jstring JNICALL
Java_com_dds_anyndk_AnyNdk_native11_12(JNIEnv *env, jclass type, jobjectArray strs,
                                       jintArray ints_) {

    //1、 获得字符串数组
    int32_t str_length = env->GetArrayLength(strs);
    LOGD("字符串 数组长度:%d", str_length);
    for (int i = 0; i < str_length; ++i) {
        jstring str = jstring(env->GetObjectArrayElement(strs, i));
        const char *c_str = env->GetStringUTFChars(str, JNI_FALSE);
        LOGD("字符串有:%s", c_str);
        //使用完释放
        env->ReleaseStringUTFChars(str, c_str);
    }

    //2、获得基本数据类型数组
    int32_t int_length = env->GetArrayLength(ints_);
    LOGD("int 数组长度:%d", int_length);
    jint *ints = env->GetIntArrayElements(ints_, nullptr);
    for (int i = 0; i < int_length; i++) {
        LOGD("int 数据有:%d", ints[i]);
    }

    env->ReleaseIntArrayElements(ints_, ints, 0);

    return env->NewStringUTF("hello");
}


extern "C"
JNIEXPORT jstring JNICALL
Java_com_dds_anyndk_AnyNdk_native11_13(JNIEnv *env, jclass type) {
    jclass class_helper = env->FindClass("com/dds/anyndk/JavaHelper");

    // 反射调用静态方法
    jmethodID method_staticMethod = env->GetStaticMethodID(class_helper, "staticMethod",
                                                           "(Ljava/lang/String;IZ)V");
    jstring staticStr = env->NewStringUTF("C++调用静态方法");
    env->CallStaticVoidMethod(class_helper, method_staticMethod, staticStr, 1, true);


    // 反射调用
    jmethodID constructMethod = env->GetMethodID(class_helper, "<init>", "()V");

    jobject helper = env->NewObject(class_helper, constructMethod);
    jmethodID instanceMethod = env->GetMethodID(class_helper, "instanceMethod",
                                                "(Ljava/lang/String;IZ)V");
    jstring instanceStr = env->NewStringUTF("C++调用实例方法");
    env->CallVoidMethod(helper, instanceMethod, instanceStr, 2, 0);
    // 释放资源
    env->DeleteLocalRef(class_helper);
    env->DeleteLocalRef(staticStr);
    env->DeleteLocalRef(instanceStr);
    env->DeleteLocalRef(helper);

    return env->NewStringUTF("dds");
}

extern "C"
JNIEXPORT void JNICALL
Java_com_dds_anyndk_AnyNdk_native11_14(JNIEnv *env, jclass type, jobject javaHelper) {

    jclass clazz = env->GetObjectClass(javaHelper);

    //获得int a的标示
    jfieldID a = env->GetFieldID(clazz, "a", "I");

    int value = env->GetIntField(javaHelper, a);
    LOGD("获得java属性a:%d", value);

    env->SetIntField(javaHelper, a, 100);

    // 获取String b 的内容
    jfieldID b = env->GetStaticFieldID(clazz, "b", "Ljava.lang.String;");
    auto bStr = jstring(env->GetStaticObjectField(clazz, b));
    const char *bc_str = env->GetStringUTFChars(bStr, JNI_FALSE);
    LOGD("获得java属性b:%s", bc_str);

    jstring new_str = env->NewStringUTF("C++字符串");
    env->SetStaticObjectField(clazz, b, new_str);

    env->ReleaseStringUTFChars(bStr, bc_str);
    env->DeleteLocalRef(new_str);
    env->DeleteLocalRef(clazz);

}


