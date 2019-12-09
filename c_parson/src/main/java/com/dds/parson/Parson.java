package com.dds.parson;

/**
 * Created by dds on 2019/8/18.
 * android_shuai@163.com
 */
public class Parson {
    static {
        System.loadLibrary("parson-test");
    }

    public static native String test();

    public static native User parseJson(String json);


}
