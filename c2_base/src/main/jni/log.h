#ifndef __LOG_H__
#define __LOG_H__

#include <stdio.h>
#include <android/log.h>
#include <errno.h>

#define  LOG_TAG    "dds_log"

#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,  LOG_TAG, __VA_ARGS__ )
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,  LOG_TAG, __VA_ARGS__ )
#define  LOGW(...)  __android_log_print(ANDROID_LOG_WARN,  LOG_TAG, __VA_ARGS__ )
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

#define LOG_TEXT_MAX_LENGTH        (1024)  //  单条日志大小
#define LOG_FILE_MAX_SIZE    (1024*1024*2) //  文件最大为2MB

enum {
    LOG_LEVEL_NONE = 0,
    LOG_LEVEL_ERR = 1,
    LOG_LEVEL_WARNING = 2,
    LOG_LEVEL_INFO = 3,
    LOG_LEVEL_DEBUG = 4
};

#ifdef  __cplusplus
extern "C" {
#endif
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

#ifdef __cplusplus
}
#endif


#endif
