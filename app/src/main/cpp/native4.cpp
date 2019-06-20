#include <jni.h>
#include <string>
#include <iostream>
#include <android/log.h>


#ifdef __cplusplus
extern "C" {
#endif

using namespace std;

#define  LOG_TAG    "dds_native4"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,  LOG_TAG, __VA_ARGS__ )

void change(int &i) {
    i = 10;
}
JNIEXPORT void JNICALL
Java_com_dds_anyndk_AnyNdk_native4(JNIEnv *env, jclass type) {

    // 1. c++输出
//    int time = 8;
//    cout << "dds:" << time << "点," << "hello world" << endl;

    // 2. c++引用
//    int i = 1;
//    change(i);
//    LOGD("%d\n", i); //i == 10


    // 3. c++ string
    string str1 = "Hello";
    string str2 = "World";
    str1.append(",");
    str1.append(str2);
    const char *s1 = str1.c_str();
    LOGD("%s", s1);
    LOGD("%ld", str1.size());
    LOGD("%d", str1.empty());


}


#ifdef __cplusplus
}
#endif


