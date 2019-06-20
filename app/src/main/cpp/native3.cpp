#include <jni.h>
#include <string>
#include <android/log.h>

#define  LOG_TAG    "dds_native3"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,  LOG_TAG, __VA_ARGS__ )

typedef struct {
    char name[50];
    int age;
} Student;

struct Struct1 {
    short a;
    int b;
    short c;
};
struct Struct2 {
    short a;
    short c;
    int b;
};

union Data {
    int i;
    short j;
};


extern "C"
JNIEXPORT void JNICALL
Java_com_dds_anyndk_AnyNdk_native3(JNIEnv *env, jclass type) {

    // 1. 声明和使用结构体
//    Student *s = (Student *) malloc(sizeof(Student));
//    memset(s, 0, sizeof(Student));
//    s->age = 100;
//
//    LOGD("%d\n", s->age);

    // 2. 字节对齐问题
//    LOGD("%ld", sizeof(Struct1));
//    LOGD("%ld", sizeof(Struct2));

    // 3. 共用体问题
//    union Data data{};
//    data.i = 1;
//    //i的数据损坏
//    data.j = 1.1f;
//
//    LOGD("%d", data.i);
//    LOGD("%d", data.j);


}