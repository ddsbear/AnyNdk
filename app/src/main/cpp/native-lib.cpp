#include <jni.h>
#include <string>
#include <android/log.h>
#include <cstdarg>

#define  LOG_TAG    "dds_log"
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,  LOG_TAG, __VA_ARGS__ )

int addR(int num, ...);

void println(char *buffer);

void say(void(*p)(char *), char *buffer);

typedef void (*Callback)(char *);

void test(Callback callback);

void callback(char *msg);

extern "C"
JNIEXPORT jstring JNICALL
Java_com_dds_anyndk_AnyNdk_stringFromJNI(JNIEnv *env, jclass type) {


//    LOGD("int --- %ld", sizeof(int));
//    LOGD("long --- %ld", sizeof(long));
//    LOGD("long --- %ld", sizeof(long long));
//
//    char str[100];
//    sprintf(str, "img/png_%d.png", 1);
//    printf("%s", str);
//
//   //使用 0 补到3个字符
//    sprintf(str, "img/png_%03d.png", 1);
//    printf("%s", str);

    //声明一个整型变量
//    int i = 10;
//    //将i的地址使用取地址符给p指针
//    int *p = &i;

    //输出 0xffff 16进制地址
//    LOGD("%#x\n", &i);
//    LOGD("%#x\n", &p);
//    int i = 10;
//    int *p = &i;
//    int pv = *p;
//    *p = 100;
//
//    LOGD("%d\n", i);
//    LOGD("%d\n", *p);
//    LOGD("%d\n", pv);


//    int i1[] = {11, 22, 33, 44, 55};
//    int *p1 = i1;
//
//    for (size_t i = 0; i < 5; i++) {
//        LOGE("%d\n", *p1++);
//    }


//    int i1[] = {11, 22, 33, 44, 55};

    //直接输出数组名会得到数组首元素的地址
//    LOGE("%#x\n", i1);
//    LOGE("%#x\n", &i1);
//    //解引用
//    LOGE("%d\n", *i1);


//    int array[2][3] = {{11, 22, 33},
//                       {44, 55, 66}};
//
//    //array1 就是一个 int[3] 类型的指针
//    int (*array1)[3] = array;
//    //通过解引用
//    int i = *(*(array1 + 1) + 1);
//
//    LOGE("%d\n", i);

//    char str[] = "hello";
//    const char *p = str;
//    str[0] = 'c'; //正确
//    // p[0] = 'c';   //错误 不能通过指针修改 const char
//    p = "012345";
//
//    LOGE("%#x\n", *p);
//
//    char * const p2 = str;
//    p2[0] = 'd';  //正确
    //  p2 = "12345"; //错误



    // 可变参数
//   int sum =  addR(3, 21, 122, 32);
//   LOGD("sum :%d\n", sum);



//    void (*p)(char *) = println;
//    p("hello");
//
//    //传递参数
//    say(println, "hello");

    //创建别名 由编译器执行解释
//    typedef void(*Fun)(char *);
//    Fun fun = println;
//    fun("hello");
//    say(fun, "hello");





    void (*p)(char *) = callback;
    test(p);
//    test(callback);




    std::string hello = "Hello from C++";

    return env->NewStringUTF(hello.c_str());
}


void test(Callback callback) {
    callback("成功");
    callback("失败");
}

void callback(char *msg) {
    LOGD("%s\n", msg);
}

//函数指针
void println(char *buffer) {
    LOGD("%s\n", buffer);
}

//接受一个函数作为参数
void say(void(*p)(char *), char *buffer) {
    p(buffer);
}

int addR(int num, ...) {
    va_list valist;
    int sum = 0;
    // 初始化
    va_start(valist, num);
    for (size_t i = 0; i < num; i++) {
        //访问
        int j = va_arg(valist, int);
        LOGD("%d\n", j);
        sum += j;
    }
    //清理
    va_end(valist);
    return sum;
}
