package com.dds.opengl.record;

import android.content.Context;
import android.opengl.EGLContext;

/**
 * 视频录制类
 * Created by dds on 2019/6/26.
 * android_shuai@163.com
 */
public class MediaRecorder {

    private final Context mContext;
    private final String mPath;
    private final int mWidth;
    private final int mHeight;
    private final EGLContext mEglContext;

    /**
     * @param context 上下文
     * @param path    保存视频的地址
     * @param width   视频宽
     * @param height  视频高
     *                还可以让人家传递帧率 fps、码率等参数
     */
    public MediaRecorder(Context context, String path, int width, int height, EGLContext eglContext) {
        mContext = context.getApplicationContext();
        mPath = path;
        mWidth = width;
        mHeight = height;
        mEglContext = eglContext;
    }

    private float mSpeed;

    public void start(float speed) {
        mSpeed = speed;

    }


    public static class Builder {
        private Context context;
        private String path;
        private int mWidth;
        private int mHeight;
        private EGLContext mEglContext;

        public Builder(Context context, EGLContext eglContext) {
            this.context = context;
            mEglContext = eglContext;
        }

        public Builder setOutPutPath(String path) {
            this.path = path;
            return this;
        }

        public Builder setOutputWidth(int width) {
            this.mWidth = width;
            return this;
        }

        public Builder setOutputHeight(int height) {
            this.mHeight = height;
            return this;
        }

        public MediaRecorder build() {
            return new MediaRecorder(context, path, mWidth, mHeight, mEglContext);
        }


    }


}
