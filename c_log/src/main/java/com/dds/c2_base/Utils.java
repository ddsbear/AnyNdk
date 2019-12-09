package com.dds.c2_base;

/**
 * Created by dds on 2019/8/13.
 * android_shuai@163.com
 */
public class Utils {
    static {
        System.loadLibrary("native-log");
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

    public static native void log(int logLevel, String content);

    public static native void logClose();


}
