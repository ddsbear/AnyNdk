package com.trustmobi.mobishield;

/**
 * Created by dds on 2019/8/13.
 * android_shuai@163.com
 */
public class MobiShield {

    static {
        System.loadLibrary("mobishield");
    }

    public static native String test();



}
