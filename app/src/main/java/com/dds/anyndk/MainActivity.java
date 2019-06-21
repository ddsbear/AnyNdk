package com.dds.anyndk;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Example of a call to a native method
        TextView tv = findViewById(R.id.sample_text);
//        tv.setText(AnyNdk.stringFromJNI());

        // 测试3
        //  AnyNdk.native3();

        // 测试4
        //AnyNdk.native4();


        // 测试11

        // 普通测试
//        String hello = AnyNdk.native11(100, "hello", 100.123f);
//        tv.setText(hello);

        // 数组测试
//        AnyNdk.native11_2(new String[]{"test1", "test2", "test3"}, new int[]{112, 113, 114});

        // c/++反射调用java方法
        // AnyNdk.native11_3();

        // c/c++反射修改变量值
//        JavaHelper javaHelper = new JavaHelper();
//        javaHelper.testReflect(javaHelper);

        AnyNdk.dynamicNative();
        AnyNdk.dynamicNative(12);

    }


}
