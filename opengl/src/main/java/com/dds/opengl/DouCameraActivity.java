package com.dds.opengl;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.widget.RadioGroup;

import com.dds.opengl.wedget.DouyinView;
import com.dds.opengl.wedget.RecordButton;

public class DouCameraActivity extends AppCompatActivity {
    DouyinView douyinView;

    public static void openActivity(Activity activity) {
        Intent intent = new Intent(activity, DouCameraActivity.class);
        activity.startActivity(intent);

    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_dou_camera);
        douyinView = findViewById(R.id.douyinView);
        RecordButton recordButton = findViewById(R.id.btn_record);
        recordButton.setOnRecordListener(new RecordButton.OnRecordListener() {
            /**
             * 开始录制
             */
            @Override
            public void onRecordStart() {
                douyinView.startRecord();
            }

            /**
             * 停止录制
             */
            @Override
            public void onRecordStop() {
                douyinView.stopRecord();
            }
        });
        RadioGroup radioGroup = findViewById(R.id.rg_speed);
        radioGroup.setOnCheckedChangeListener(new RadioGroup.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(RadioGroup group, int checkedId) {
                if (checkedId == R.id.rb_extra_slow) {
                    douyinView.setSpeed(DouyinView.Speed.MODE_EXTRA_SLOW);
                } else if (checkedId == R.id.rb_slow) {
                    douyinView.setSpeed(DouyinView.Speed.MODE_SLOW);
                } else if (checkedId == R.id.rb_normal) {
                    douyinView.setSpeed(DouyinView.Speed.MODE_NORMAL);
                } else if (checkedId == R.id.rb_fast) {
                    douyinView.setSpeed(DouyinView.Speed.MODE_FAST);
                } else if (checkedId == R.id.rb_extra_fast) {
                    douyinView.setSpeed(DouyinView.Speed.MODE_EXTRA_FAST);
                }
            }
        });
    }

}

