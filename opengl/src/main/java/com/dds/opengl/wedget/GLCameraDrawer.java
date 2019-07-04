package com.dds.opengl.wedget;

import android.content.Context;
import android.graphics.SurfaceTexture;
import android.opengl.GLES11Ext;
import android.opengl.GLES20;
import android.opengl.GLSurfaceView;
import android.opengl.Matrix;

import com.dds.opengl.filter.impl.OseFilter;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class GLCameraDrawer implements GLSurfaceView.Renderer {
    private SurfaceTexture mSurfaceTexture;
    // OseFilter 使用openGL渲染画面
    private OseFilter baseFilter;
    private float[] mtx = new float[16];
    private Context mContext;

    public GLCameraDrawer(Context context) {
        baseFilter = new OseFilter();
        mContext = context;
    }


    public SurfaceTexture getSurfaceTexture() {
        return mSurfaceTexture;
    }


    //------------------------------------------------------------------------------
    private int width, height;
    private int dataWidth, dataHeight;

    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        // 通过openGL创建一个纹理id
        int texture = createTextureID();
        // 初始化SurfaceTexture
        mSurfaceTexture = new SurfaceTexture(texture);
        // 注意：必须在gl线程操作openGL
        baseFilter.create(mContext);
        baseFilter.setTextureId(texture);
    }

    /**
     * 画布发生了改变
     */
    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height) {
        setViewSize(width, height);
        // baseFilter.setMatrix(mtx);
    }


    /**
     * 开始画画吧
     */
    @Override
    public void onDrawFrame(GL10 gl) {
        if (mSurfaceTexture != null) {
            // 更新纹理图像为从图像流中提取的最近一帧
            mSurfaceTexture.updateTexImage();
        }
        // 绘制纹理
        baseFilter.draw();
    }


    //------------------------------------------------------------------------------

    public void setViewSize(int width, int height) {
        this.width = width;
        this.height = height;
        calculateMatrix();
    }

    public void setDataSize(int dataWidth, int dataHeight) {
        this.dataWidth = dataWidth;
        this.dataHeight = dataHeight;
        calculateMatrix();
    }

    private void calculateMatrix() {
        getShowMatrix(mtx, this.dataWidth, this.dataHeight, this.width, this.height);

        if (CaptureView.cameraId == 1) {
            // 前置摄像头旋转90度并
            flip(mtx, true, false);
            rotate(mtx, 90);
        } else {
            // 前置旋转270度
            rotate(mtx, 270);
        }
        baseFilter.setMatrix(mtx);
    }

    private static void getShowMatrix(float[] matrix, int imgWidth, int imgHeight, int viewWidth, int viewHeight) {
        if (imgHeight > 0 && imgWidth > 0 && viewWidth > 0 && viewHeight > 0) {
            float sWhView = (float) viewWidth / viewHeight;
            float sWhImg = (float) imgWidth / imgHeight;
            float[] projection = new float[16];
            float[] camera = new float[16];
            if (sWhImg > sWhView) {
                Matrix.orthoM(projection, 0, -sWhView / sWhImg, sWhView / sWhImg, -1, 1, 1, 3);
            } else {
                Matrix.orthoM(projection, 0, -1, 1, -sWhImg / sWhView, sWhImg / sWhView, 1, 3);
            }
            Matrix.setLookAtM(camera, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0);
            Matrix.multiplyMM(matrix, 0, projection, 0, camera, 0);
        }
    }

    private static float[] rotate(float[] m, float angle) {
        Matrix.rotateM(m, 0, angle, 0, 0, 1);
        return m;
    }

    private static float[] flip(float[] m, boolean x, boolean y) {
        if (x || y) {
            Matrix.scaleM(m, 0, x ? -1 : 1, y ? -1 : 1, 1);
        }
        return m;
    }

    //通过openGL创建一个纹理id
    private int createTextureID() {
        int[] texture = new int[1];
        GLES20.glGenTextures(texture.length, texture, 0);  // 创建纹理，生成你要操作的纹理对象的索引
        GLES20.glBindTexture(GLES11Ext.GL_TEXTURE_EXTERNAL_OES, texture[0]); // 绑定纹理，告诉OpenGL下面代码中对2D纹理的任何设置都是针对索引为0的纹理的

        // glTexParameterF 是设置纹理贴图的参数属性
        GLES20.glTexParameterf(GLES11Ext.GL_TEXTURE_EXTERNAL_OES, GL10.GL_TEXTURE_MIN_FILTER, GL10.GL_LINEAR);
        GLES20.glTexParameterf(GLES11Ext.GL_TEXTURE_EXTERNAL_OES, GL10.GL_TEXTURE_MAG_FILTER, GL10.GL_LINEAR);

        // OpenGL——纹理过滤函数glTexParameterI() 确定如何把纹理象素映射成像素.
        GLES20.glTexParameteri(GLES11Ext.GL_TEXTURE_EXTERNAL_OES, GL10.GL_TEXTURE_WRAP_S, GL10.GL_CLAMP_TO_EDGE);
        GLES20.glTexParameteri(GLES11Ext.GL_TEXTURE_EXTERNAL_OES, GL10.GL_TEXTURE_WRAP_T, GL10.GL_CLAMP_TO_EDGE);
        return texture[0];

    }



}
