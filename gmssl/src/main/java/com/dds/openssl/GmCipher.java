package com.dds.openssl;

/**
 * Created by dds on 2019/8/18.
 * android_shuai@163.com
 */
public class GmCipher {
    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }
    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();


    private static GmCipher gmCipher;

    public static GmCipher getInstance() {
        if (gmCipher == null) {
            gmCipher = new GmCipher();
        }
        return gmCipher;
    }
}
