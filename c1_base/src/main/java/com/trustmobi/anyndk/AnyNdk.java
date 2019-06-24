package com.trustmobi.anyndk;

/**
 * Created by dds on 2019/6/18.
 * android_shuai@163.com
 */
public class AnyNdk {
    static {
        System.loadLibrary("native-lib");
    }

    public static native String stringFromJNI();


    public static native void native3();

    public static native void native4();

    public static native void native5();


    public static native String native11(int a, String str, float f);


    public static native String native11_2(String[] strs, int[] ints);

    // 反射调用java的方法
    public static native String native11_3();

    // 反射修改变量值
    public static native void native11_4(JavaHelper javaHelper);


    // 线程中调用java
    public static native void native11_5(JavaHelper javaHelper);


    public static native void dynamicNative();

    public static native String dynamicNative(int i);





}
