package com.dds.anyndk;

import android.app.Application;

import com.squareup.leakcanary.LeakCanary;

/**
 * Created by dds on 2019/6/22.
 * android_shuai@163.com
 */
public class App extends Application {

    @Override
    public void onCreate() {
        super.onCreate();

        if (LeakCanary.isInAnalyzerProcess(this)) {
            return;
        }

        LeakCanary.install(this);


    }
}
