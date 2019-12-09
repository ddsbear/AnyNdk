package com.dds.anyndk;

import android.Manifest;
import android.os.Build;
import android.os.Bundle;
import android.view.View;

import androidx.appcompat.app.AppCompatActivity;

import com.dds.anyndk.permision.Consumer;
import com.dds.anyndk.permision.Permissions;


public class MainActivity extends AppCompatActivity {


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
            Permissions.request(this, new String[]{
                    Manifest.permission.WRITE_EXTERNAL_STORAGE,
                    Manifest.permission.READ_EXTERNAL_STORAGE}, new Consumer<Integer>() {
                @Override
                public void accept(Integer integer) {

                }
            });
        }


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
