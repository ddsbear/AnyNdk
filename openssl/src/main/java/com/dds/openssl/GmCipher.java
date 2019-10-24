package com.dds.openssl;

/**
 * Created by dds on 2019/8/18.
 * android_shuai@163.com
 */
public class GmCipher {
    static {
        System.loadLibrary("native-cipher");
    }


    public native String AesEncode(String content, String key);

    public native String AesDecode(String content, String key);



    private static GmCipher gmCipher;

    public static GmCipher getInstance() {
        if (gmCipher == null) {
            gmCipher = new GmCipher();
        }
        return gmCipher;
    }
}
