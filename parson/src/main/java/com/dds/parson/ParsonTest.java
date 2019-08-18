package com.dds.parson;

/**
 * Created by dds on 2019/8/18.
 * android_shuai@163.com
 */
public class ParsonTest {
    static {
        System.loadLibrary("parson-test");
    }

    public native String stringFromJNI();


}
