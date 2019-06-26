package com.dds.anyndk;

import android.Manifest;
import android.content.pm.PackageManager;
import android.os.Build;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.View;

import com.dds.opengl.DouCameraActivity;
import com.dds.opengl.permision.Consumer;
import com.dds.opengl.permision.Permissions;

public class MainActivity extends AppCompatActivity {


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);


    }

    public void onC1base(View view) {
        C1Activity.openActivity(this);


    }

    // openGL显示摄像头
    public void onDouyinCamera(View view) {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
            Permissions.request(this, new String[]{Manifest.permission.CAMERA,
                    Manifest.permission.RECORD_AUDIO,
                    Manifest.permission.WRITE_EXTERNAL_STORAGE}, new Consumer<Integer>() {
                @Override
                public void accept(Integer integer) {
                    if (integer == PackageManager.PERMISSION_GRANTED) {
                        DouCameraActivity.openActivity(MainActivity.this);
                    }

                }
            });
        }


    }


}
