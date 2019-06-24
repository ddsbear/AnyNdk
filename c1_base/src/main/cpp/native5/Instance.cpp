//
// Created by dds on 2019/6/22.
//

#include "Instance.h"
#include <android/log.h>

#define  LOG_TAG    "dds_native5"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,  LOG_TAG, __VA_ARGS__ )


Instance* Instance::instance = 0;

Instance *Instance::getInstance() {
    //C++11以后，编译器保证内部静态变量的线程安全性
    if (!instance) {
        instance = new Instance;
    }
    return instance;

}

void Instance::printHello() {
    LOGD("printHello");


}
