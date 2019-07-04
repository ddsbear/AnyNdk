package com.dds.opengl;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.widget.RadioGroup;
import android.widget.RelativeLayout;

import com.dds.opengl.wedget.CaptureView;
import com.dds.opengl.wedget.RecordButton;

public class DouCameraActivity extends AppCompatActivity {

    RelativeLayout captureContainer;
    CaptureView douyinView;

    public static void openActivity(Activity activity) {
        Intent intent = new Intent(activity, DouCameraActivity.class);
        activity.startActivity(intent);

    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_dou_camera);
        initView();
        initListener();


    }

    @Override
    protected void onResume() {
        super.onResume();
        if (douyinView != null) {
            douyinView.onResume();
        }
    }

    @Override
    protected void onPause() {
        super.onPause();
        if (douyinView != null) {
            douyinView.onPause();
        }
    }

    private void initView() {
        captureContainer = findViewById(R.id.capture_container);
        //douyinView = new CaptureView(this);
        //  douyinView.setLayoutParams(new RelativeLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT,
        //  ViewGroup.LayoutParams.WRAP_CONTENT));
        //  captureContainer.addView(douyinView, 0);


    }

    private void initListener() {
        // 录制
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


        // 选择速度
        RadioGroup radioGroup = findViewById(R.id.rg_speed);
        radioGroup.setOnCheckedChangeListener(new RadioGroup.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(RadioGroup group, int checkedId) {
                if (checkedId == R.id.rb_extra_slow) {
                    douyinView.setSpeed(CaptureView.Speed.MODE_EXTRA_SLOW);
                } else if (checkedId == R.id.rb_slow) {
                    douyinView.setSpeed(CaptureView.Speed.MODE_SLOW);
                } else if (checkedId == R.id.rb_normal) {
                    douyinView.setSpeed(CaptureView.Speed.MODE_NORMAL);
                } else if (checkedId == R.id.rb_fast) {
                    douyinView.setSpeed(CaptureView.Speed.MODE_FAST);
                } else if (checkedId == R.id.rb_extra_fast) {
                    douyinView.setSpeed(CaptureView.Speed.MODE_EXTRA_FAST);
                }
            }
        });
    }

}

