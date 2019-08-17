package com.dds.gmssl;

/**
 * Created by dds on 2019/8/16.
 * android_shuai@163.com
 */
public class GmCipher {


    public native void LogInit(String dir, int logFileLevel, int logScreenLevel);

    // 生成随机数
    public native byte[] generateRandom(int length);


    private static GmCipher gmCipher;

    public static GmCipher getInstance() {
        if (gmCipher == null) {
            gmCipher = new GmCipher();
        }
        return gmCipher;
    }
}
