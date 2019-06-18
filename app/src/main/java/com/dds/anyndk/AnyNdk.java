package com.dds.anyndk;

/**
 * Created by dds on 2019/6/18.
 * android_shuai@163.com
 */
public class AnyNdk {
    static {
        System.loadLibrary("native-lib");
    }

    public static native String stringFromJNI();

}
