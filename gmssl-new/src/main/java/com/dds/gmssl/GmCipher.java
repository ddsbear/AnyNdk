package com.dds.gmssl;

/**
 * Created by dds on 2019/8/13.
 * android_shuai@163.com
 */
public class GmCipher {

    static {
        System.loadLibrary("native-gmssl");
    }


    public native String sm4Encode(String content, String key);

    public native String sm4Decode(String content, String key);



    private static GmCipher gmCipher;

    public static GmCipher getInstance() {
        if (gmCipher == null) {
            gmCipher = new GmCipher();
        }
        return gmCipher;
    }

}
