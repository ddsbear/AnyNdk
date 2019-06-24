package com.trustmobi.anyndk;

import android.util.Log;

/**
 * Created by dds on 2019/6/20.
 * android_shuai@163.com
 */
public class JavaHelper {


    private static final String TAG = "dds_native11";

    //private和public 对jni开发来说没任何区别 都能反射调用
    public void instanceMethod(String a, int b, boolean c) {
        Log.e(TAG, "instanceMethod a=" + a + " b=" + b + " c=" + c);
    }

    public static void staticMethod(String a, int b, boolean c) {
        Log.e(TAG, "staticMethod a=" + a + " b=" + b + " c=" + c);
    }

     int a = 10;
     static String b = "java字符串";

    public void testReflect(JavaHelper javaHelper) {
        Log.e(TAG, "修改前 ： a = " + a + " b=" + b);
        AnyNdk.native11_4(javaHelper);
        Log.e(TAG, "修改后 ： a = " + a + " b=" + b);
    }


}
