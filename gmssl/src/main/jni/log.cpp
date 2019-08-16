#include <cstdio>
#include <cstdarg>
#include <ctime>
#include <sys/stat.h>
#include <cstring>
#include <sys/types.h>
#include <cassert>
#include <string>
#include <sstream>
#include <vector>
#include <iostream>
#include <algorithm>
#include <sys/time.h>
#include <cstring>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>

#include "log.h"

char LOG_FILE_NAME[100] = "dds_log.log";   //日志默认名称
// 日志级别
int g_log_file_level = LOG_LEVEL_NONE;
int g_log_screen_level = LOG_LEVEL_NONE;

long g_RollingPtr = 0;

// 文件路径
static std::string g_logFilePath;

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

char g_log_info[LOG_TEXT_MAX_LENGTH + 100];

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

void _LogClose() {
    g_log_file_level = LOG_LEVEL_NONE;
    g_log_screen_level = LOG_LEVEL_NONE;
}
