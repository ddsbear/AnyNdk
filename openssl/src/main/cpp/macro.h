//
// Created by dds on 2019/10/24.
//

#ifndef ANYNDK_MACRO_H
#define ANYNDK_MACRO_H

#include <android/log.h>

#define  LOG_TAG    "dds_openssl"
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)

//宏函数
#define DELETE(obj) if(obj){ delete obj; obj = 0; }


#endif //ANYNDK_MACRO_H
