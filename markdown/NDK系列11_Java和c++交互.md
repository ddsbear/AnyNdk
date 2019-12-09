# 【NDK系列11】Java和c++交互  Jni初识

[TOC]

JNI是一种本地编程接口。它允许运行在JAVA虚拟机中的JAVA代码和用其他编程语言，诸如C语言、C++、汇编，应用和库之间的交互操作。 不只是Android特有的东西

## 1. Java调用c++方法

静态加载so

```java
 static {
        System.loadLibrary("native-lib");
}
```

c++中和java中对应方法

```java
  public static native void native11();
```

```c
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
```

CmakeLists.txt配置

```c
cmake_minimum_required(VERSION 3.4.1)

add_library(
        native-lib
        SHARED
        native11.cpp)

target_link_libraries(
        native-lib
        log)
```

## 2. JNI数据类型

JNIEXPORT 和 JNICALL，定义在`jni.h`头文件中。

JNIEXPORT：

​	在 Windows 中,定义为`__declspec(dllexport)`。因为Windows编译 dll 动态库规定，如果动态库中的函数要被外部调用，需要在函数声明中添加此标识，表示将该函数导出在外部可以调用。

​	在 Linux/Unix/Mac os/Android 这种 Like Unix系统中，定义为`__attribute__ ((visibility ("default")))`

JNICALL:

​	在类Unix中无定义，在Windows中定义为：`_stdcall  ` ，一种函数调用约定

类Unix系统中这两个宏可以省略不加。



| Java类型  | 本地类型      | 描述                                     |
| --------- | ------------- | ---------------------------------------- |
| boolean   | jboolean      | C/C++8位整型                             |
| byte      | jbyte         | C/C++带符号的8位整型                     |
| char      | jchar         | C/C++无符号的16位整型                    |
| short     | jshort        | C/C++带符号的16位整型                    |
| int       | jint          | C/C++带符号的32位整型                    |
| long      | jlong         | C/C++带符号的64位整型                    |
| float     | jfloat        | C/C++32位浮点型                          |
| double    | jdouble       | C/C++64位浮点型                          |
| Object    | jobject       | 任何Java对象，或者没有对应java类型的对象 |
| Class     | jclass        | Class对象                                |
| String    | jstring       | 字符串对象                               |
| Object[]  | jobjectArray  | 任何对象的数组                           |
| boolean[] | jbooleanArray | 布尔型数组                               |
| byte[]    | jbyteArray    | 比特型数组                               |
| char[]    | jcharArray    | 字符型数组                               |
| short[]   | jshortArray   | 短整型数组                               |
| int[]     | jintArray     | 整型数组                                 |
| long[]    | jlongArray    | 长整型数组                               |
| float[]   | jfloatArray   | 浮点型数组                               |
| double[]  | jdoubleArray  | 双浮点型数组                             |

**获取数组类型的数据**

```c
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
```



## 3. C/C++反射Java

在C/C++中反射创建Java的对象，调用Java的方法

基本数据类型的签名采用一系列大写字母来表示, 如下表所示:

| Java类型 | 签名             |
| -------- | ---------------- |
| boolean  | Z                |
| short    | S                |
| float    | F                |
| byte     | B                |
| int      | I                |
| double   | D                |
| char     | C                |
| long     | J                |
| void     | V                |
| 引用类型 | L + 全限定名 + ; |
| 数组     | [+类型签名       |



需要反射的方法

```java
public class JavaHelper {
    private static final String TAG = "dds_native11";

    //private和public 对jni开发来说没任何区别 都能反射调用
    public void instanceMethod(String a, int b, boolean c) {
        Log.e(TAG, "instanceMethod a=" + a + " b=" + b + " c=" + c);
    }

    public static void staticMethod(String a, int b, boolean c) {
        Log.e(TAG, "staticMethod a=" + a + " b=" + b + " c=" + c);
    }
}

```



### 反射调用方法

```c
extern "C"
JNIEXPORT jstring JNICALL
Java_com_dds_anyndk_AnyNdk_native11_13(JNIEnv *env, jclass type) {
    jclass class_helper = env->FindClass("com/dds/anyndk/JavaHelper");

    // 反射调用静态方法
    jmethodID method_staticMethod = env->GetStaticMethodID(class_helper, "staticMethod",
                                                           "(Ljava/lang/String;IZ)V");
    jstring staticStr = env->NewStringUTF("C++调用静态方法");
    env->CallStaticVoidMethod(class_helper, method_staticMethod, staticStr, 1, true);


    // 反射调用构造方法
    jmethodID constructMethod = env->GetMethodID(class_helper,"<init>","()V");

    jobject  helper = env->NewObject(class_helper,constructMethod);
    jmethodID instanceMethod = env->GetMethodID(class_helper,"instanceMethod","(Ljava/lang/String;IZ)V");
    jstring instanceStr= env->NewStringUTF("C++调用实例方法");
    env->CallVoidMethod(helper,instanceMethod,instanceStr,2,0);
    // 释放资源
    env->DeleteLocalRef(class_helper);
    env->DeleteLocalRef(staticStr);
    env->DeleteLocalRef(instanceStr);
    env->DeleteLocalRef(helper);

    return env->NewStringUTF("dds");
}
```

可以使用javap来获取反射方法时的签名

```shell
javap -s com.dds.anyndk.JavaHelper
```



### 反射修改变量

需要反射的方法

```java
public class JavaHelper {
     int a = 10;
     static String b = "java字符串";

    public void testReflect(JavaHelper javaHelper) {
        Log.e(TAG, "修改前 ： a = " + a + " b=" + b);
        AnyNdk.native11_4(javaHelper);
        Log.e(TAG, "修改后 ： a = " + a + " b=" + b);
    }
}
```

反射

```c
#define  LOG_TAG    "dds_native4"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,  LOG_TAG, __VA_ARGS__ )

extern "C"
JNIEXPORT void JNICALL
Java_com_dds_anyndk_AnyNdk_native11_14(JNIEnv *env, jclass type, jobject javaHelper) {

    jclass clazz = env->GetObjectClass(javaHelper);

    //获得int a的标示
    jfieldID a = env->GetFieldID(clazz, "a", "I");
	// 获取a的值
    int value = env->GetIntField(javaHelper, a);
    LOGD("获得java属性a:%d", value);

    env->SetIntField(javaHelper, a, 100);

    // 获取String b 的标示
    jfieldID b = env->GetStaticFieldID(clazz, "b", "Ljava.lang.String;");
    // 获取静态变量的值
    auto bStr = jstring(env->GetStaticObjectField(clazz, b));
    
    const char *bc_str = env->GetStringUTFChars(bStr, JNI_FALSE);
    LOGD("获得java属性b:%s", bc_str);

    jstring new_str = env->NewStringUTF("C++字符串");
    env->SetStaticObjectField(clazz, b, new_str);

    // 释放资源
    env->ReleaseStringUTFChars(bStr, bc_str);
    env->DeleteLocalRef(new_str);
    env->DeleteLocalRef(clazz);

}

```

## 4. JNI_OnLoad

调用System.loadLibrary()函数时， 内部就会去查找so中的 JNI_OnLoad 函数，如果存在此函数则调用。

JNI_OnLoad会：

告诉 VM 此 native 组件使用的 JNI 版本。

​	对应了Java版本，android中只支持JNI_VERSION_1_2 、JNI_VERSION_1_4、JNI_VERSION_1_6 

​	在JDK1.8有 JNI_VERSION_1_8。

```c
jint JNI_OnLoad(JavaVM* vm, void* reserved){
    // 2、4、6都可以
    return JNI_VERSION_1_4;
}
```

**动态注册**

在此之前我们一直在jni中使用的 Java_PACKAGENAME_CLASSNAME_METHODNAME 来进行与java方法的匹配，这种方式我们称之为静态注册。

而动态注册则意味着方法名可以不用这么长了，在android aosp源码中就大量的使用了动态注册的形式

```c++

//Java：
native void dynamicNative();
native String dynamicNative(int i);

//C++：
void  dynamicNative1(JNIEnv *env, jobject jobj){
    LOGE("dynamicNative1 动态注册");
}
jstring  dynamicNative2(JNIEnv *env, jobject jobj,jint i){
    return env->NewStringUTF("我是动态注册的dynamicNative2方法");
}

//需要动态注册的方法数组
static const JNINativeMethod mMethods[] = {
        {"dynamicNative","()V", (void *)dynamicNative1},
        {"dynamicNative", "(I)Ljava/lang/String;", (jstring *)dynamicNative2}

};
//需要动态注册native方法的类名
static const char* mClassName = "com/dds/anyndk/AnyNdk";
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
```





## 5. c++线程中调用Java

native调用java需要使用JNIEnv这个结构体，而JNIEnv是由Jvm传入与线程相关的变量。

但是可以通过JavaVM的AttachCurrentThread方法来获取到当前线程中的JNIEnv指针。

```c
JavaVM* _vm = 0;
jobject  _instance = 0;
jint JNI_OnLoad(JavaVM* vm, void* reserved){
    _vm = vm;
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
    // 开启线程
    pthread_create(&pid, 0, task, 0);


}



```


## 代码

https://github.com/ddssingsong/AnyNdk










