package com.dds.anyndk;

import android.Manifest;
import android.os.Build;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.View;

import com.dds.anyndk.permision.Consumer;
import com.dds.anyndk.permision.Permissions;


public class MainActivity extends AppCompatActivity {


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
            Permissions.request(this, new String[]{
                    Manifest.permission.CAMERA,
                    Manifest.permission.RECORD_AUDIO,
                    Manifest.permission.WRITE_EXTERNAL_STORAGE,
                    Manifest.permission.READ_EXTERNAL_STORAGE}, new Consumer<Integer>() {
                @Override
                public void accept(Integer integer) {

                }
            });
        }


    }


    // c/c++基础测试
    public void onC1base(View view) {
//        C1Activity.openActivity(this);


    }

    // c/c++的工具
    public void onC2base(View view) {
//        Utils.test("my is string,", 12, 323L);
//
//        File file = new File(Environment.getExternalStorageDirectory(), "dds_log");
//        String fileName = "dds_log.txt";
//
//        if (!file.exists()) {
//            file.mkdirs();
//        }
//        Utils.LogInit(file.getAbsolutePath(), fileName, 4, 4);
//        Utils.log(4, "我是1");
//        Utils.log(4, "我是2");
//        Utils.log(4, "我是3");
//        Utils.log(4, "我是4");
//        Utils.log(4, "我是5");
//        Utils.log(4, "我是6");
//        Utils.log(4, "我是7");
//        Utils.log(4, "我是8");

    }

    // openGL显示摄像头
    public void onDouyinCamera(View view) {
//        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
//            Permissions.request(this, new String[]{Manifest.permission.CAMERA,
//                    Manifest.permission.RECORD_AUDIO,
//                    Manifest.permission.WRITE_EXTERNAL_STORAGE}, new Consumer<Integer>() {
//                @Override
//                public void accept(Integer integer) {
//                    if (integer == PackageManager.PERMISSION_GRANTED) {
//                        DouCameraActivity.openActivity(MainActivity.this);
//                    }
//
//                }
//            });
//        }


    }


    // openssl
    public void openssl(View view) {
        OpenSSLActivity.openActivity(this);

    }

    // parson
    public void parson(View view) {
        ParsonActivity.openActivity(this);
    }
}
