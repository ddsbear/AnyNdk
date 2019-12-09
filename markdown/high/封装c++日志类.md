## 需求定位

我们一般用jni都是封装一些库供Android调用，其中有一项就是日志打印，需要控制台输出，还需要文件输出日志，以便于查看客户端使用情况，如果有bug，也可以快速定位

初步需求如下

1. 供c++代码调用
2. 控制台输出
3. 文件输出（可控制文件大小）
4. 可设置日志等级



## 定义方法和变量

定义日志输出等级

```c
enum {
    LOG_LEVEL_NONE = 0,
    LOG_LEVEL_ERR = 1,
    LOG_LEVEL_WARNING = 2,
    LOG_LEVEL_INFO = 3,
    LOG_LEVEL_DEBUG = 4
};
```



定义文件大小和日志大小

```c
#define LOG_TEXT_MAX_LENGTH        (1024)  //  单条日志大小
#define LOG_FILE_MAX_SIZE    (1024*1024*2) //  文件最大为2MB
```



对于Android来说，需要引入log并定义日志TAG

```c
#include <android/log.h>

#define  LOG_TAG    "dds_log"

#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,  LOG_TAG, __VA_ARGS__ )
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,  LOG_TAG, __VA_ARGS__ )
#define  LOGW(...)  __android_log_print(ANDROID_LOG_WARN,  LOG_TAG, __VA_ARGS__ )
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
```

log.h定义几个方法

```c
/**
 * 初始化日志选项
 * @param pFile
 * @param filename
 * @param logLevel
 * @param printScreen
 * @return
 */
int _LogInit(const char *pFile, const char *filename, int logLevel, int printScreen);

/**
 * 写日志
 * @param level
 * @param strFormat
 * @param ...
 */
void WriteTextLog(int level, const char *strFormat, ...);

/**
 * 向文件中写入日志
 * @param level
 * @param log
 */
void WriteTextLogBottom(int level, const char *log);

/**
 * 关闭日志库
 */
void _LogClose();
```

像第一条_LogInit需要传入日志路径和名称，已经日志等级，需要Android调用，于是先来几个jni的方法

```c
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

```

对应的java方法如下

```java
public class Utils {
    static {
        System.loadLibrary("native-test");
    }

    public enum LogLevel {
        LOG_LEVEL_NONE,
        LOG_LEVEL_ERR,
        LOG_LEVEL_WARNING,
        LOG_LEVEL_INFO,
        LOG_LEVEL_DEBUG
    }

    public static native void test(String str, int a, long b);

    // 日志类初始化
    public static native void LogInit(String logFilePath, String logName, int logfileLevel, int logScreenLevel);

    // 这里测试使用，所以将方法暴露出来，其实使用的时候是不需要暴露出来的
    public static native void log(int logLevel, String content);

    public static native void logClose();


}

```



## 编写代码

log.cpp详细代码

初始化

```c
int _LogInit(const char *pFile, const char *filename, int logLevel, int printScreen) {
    g_RollingPtr = 0;
    g_log_file_level = logLevel;
    g_log_screen_level = printScreen;
    if (filename != nullptr) {
        strcpy(LOG_FILE_NAME, filename);
    }
    if (pFile != nullptr) {
        g_logFilePath = std::string(pFile) + "/" + LOG_FILE_NAME;
    } else {
        g_logFilePath = LOG_FILE_NAME;
    }
    return 0;
}

```

打印日志

```c
void WriteTextLog(int level, const char *strFormat, ...) {
    if (level > g_log_file_level && level > g_log_screen_level) {
        return;
    }
    time_t now;
    char timeStr[20];
    char temBuf[LOG_TEXT_MAX_LENGTH];

    time(&now);
    strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", localtime(&now));

    va_list args;
    va_start(args, strFormat);
    vsnprintf(temBuf, sizeof(temBuf) - 1, strFormat, args);
    va_end(args);

    switch (level) {
        case LOG_LEVEL_DEBUG:
            LOGD("%s", g_log_info);
            sprintf(g_log_info, "%s [DEBUG] %s\n", timeStr, temBuf);
            break;

        case LOG_LEVEL_INFO:
            LOGI("%s", g_log_info);
            sprintf(g_log_info, "%s [INFO] %s\n", timeStr, temBuf);
            break;

        case LOG_LEVEL_WARNING:
            LOGW("%s", g_log_info);
            sprintf(g_log_info, "%s [WARN] %s\n", timeStr, temBuf);
            break;

        case LOG_LEVEL_ERR:
            LOGE("%s", g_log_info);
            sprintf(g_log_info, "%s [ERROR] %s\n", timeStr, temBuf);
            break;

        default:
            LOGI("%s", g_log_info);
            sprintf(g_log_info, "%s [NONE] %s\n", timeStr, temBuf);
            break;
    }

    if (level <= g_log_file_level && !g_logFilePath.empty()) {
        WriteTextLogBottom(level, g_log_info);
    }
}

```



打印日志到文件

```c
void WriteTextLogBottom(int level, const char *log) {
    if (level <= g_log_file_level) {
        FILE *fp;
        struct stat info{};
        if (stat(g_logFilePath.c_str(), &info) != 0) {
            g_RollingPtr = 0;
            fp = fopen(g_logFilePath.c_str(), "we");// create file
            if (fp == nullptr) {
                LOGE("%s, fopen(w) %s fail, err:%d", __func__, g_logFilePath.c_str(), errno);
                return;
            }
            fprintf(fp, "%s, stat fail create logfile, errno:%d", __func__, errno);
            fprintf(fp, "%s", log);
            fclose(fp);
            return;
        }

        if (info.st_size >= LOG_FILE_MAX_SIZE)// loop write
        {
            // 这里使用复写的方式，保证日志文件不会超过LOG_FILE_MAX_SIZE
            fp = fopen(g_logFilePath.c_str(), "r+");
            if (nullptr == fp) {
                LOGE("%s, fopen(r+) %s fail, size:%ld, err:%d", __func__, g_logFilePath.c_str(),
                     info.st_size, errno);
                return;
            }
            if (fseek(fp, g_RollingPtr, SEEK_SET) < 0) {
                fclose(fp);
                return;
            }
            g_RollingPtr += strlen(log);
            if (g_RollingPtr > info.st_size) {
                g_RollingPtr = 0;
            }
        } else {
            fp = fopen(g_logFilePath.c_str(), "a");
            if (fp == nullptr) {
                LOGE("%s, fopen(a) %s fail, size:%ld, err:%d", __func__, g_logFilePath.c_str(),
                     info.st_size, errno);
                return;
            }
        }
        fprintf(fp, "%s", log);
        fclose(fp);
    }
}

```



## 大功告成

具体代码收录到https://github.com/ddssingsong/AnyNdk    

c2_base测试代码里

谢谢关注

