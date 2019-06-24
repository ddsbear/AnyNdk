package com.dds.opengl.wedget;

import android.content.Context;
import android.graphics.Point;
import android.graphics.SurfaceTexture;
import android.hardware.Camera;
import android.opengl.GLES20;
import android.opengl.GLSurfaceView;

import com.dds.opengl.utils.CameraHelper;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class CaptureView extends GLSurfaceView implements GLSurfaceView.Renderer {


    private GLCameraDrawer glCameraDrawer;
    private CameraHelper cameraHelper;
    public static int cameraId = 1;  // 要打开的摄像头的ID

    public CaptureView(Context context) {
        super(context);
        // 初始化
        glCameraDrawer = new GLCameraDrawer(getContext());
        // 创建摄像头资源
        cameraHelper = new CameraHelper();
        //设置EGL版本
        setEGLContextClientVersion(2);
        // 设置Render
        setRenderer(this);
        /**
         * 1. 连续渲染
         * 2. 按需渲染
         * 這裏设置按需渲染
         */
        setRenderMode(RENDERMODE_WHEN_DIRTY);
    }


    // ===========================設置录制速度=========================================
    //默认正常速度
    private Speed mSpeed = Speed.MODE_NORMAL;


    public enum Speed {
        MODE_EXTRA_SLOW, MODE_SLOW, MODE_NORMAL, MODE_FAST, MODE_EXTRA_FAST
    }

    public void setSpeed(Speed speed) {
        mSpeed = speed;
    }

    public void startRecord() {
        float speed = 1.f;
        switch (mSpeed) {
            case MODE_EXTRA_SLOW:
                speed = 0.3f;
                break;
            case MODE_SLOW:
                speed = 0.5f;
                break;
            case MODE_NORMAL:
                speed = 1.f;
                break;
            case MODE_FAST:
                speed = 1.5f;
                break;
            case MODE_EXTRA_FAST:
                speed = 3.f;
                break;
        }
        glCameraDrawer.startRecord(speed);
    }

    public void stopRecord() {
        glCameraDrawer.stopRecord();
    }

    //=============================Render回调=========================================

    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        glCameraDrawer.onSurfaceCreated(gl, config);

        cameraHelper.open(Camera.CameraInfo.CAMERA_FACING_BACK);
        Point point = cameraHelper.getPreviewSize();
        glCameraDrawer.setDataSize(point.x, point.y);
        cameraHelper.setPreviewTexture(glCameraDrawer.getSurfaceTexture());
        glCameraDrawer.getSurfaceTexture().setOnFrameAvailableListener(new SurfaceTexture.OnFrameAvailableListener() {
            @Override
            public void onFrameAvailable(SurfaceTexture surfaceTexture) {
                requestRender();
            }
        });
        cameraHelper.preview();

    }

    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height) {
        GLES20.glViewport(0, 0, width, height);
        glCameraDrawer.setViewSize(width, height);
    }

    @Override
    public void onDrawFrame(GL10 gl) {
        glCameraDrawer.onDrawFrame(gl);

    }

    @Override
    public void onPause() {
        super.onPause();
        cameraHelper.close();
    }

}
