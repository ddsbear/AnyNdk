package com.dds.opengl.filter.impl;

import android.content.Context;
import android.opengl.GLES20;
import android.util.Log;

import com.dds.opengl.R;
import com.dds.opengl.utils.OpenUtils;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;
import java.util.Arrays;


/**
 * 负责往屏幕上渲染
 */
public class OseFilter {
    private static final String TAG = "dds_Filter";

    protected int mProgram;    // 程序句柄

    protected int mHPosition;  // 顶点坐标句柄
    protected int mHCoord;     // 纹理坐标句柄
    protected int mHMatrix;    // 总变换矩阵句柄

    protected int mHTexture;   // 默认纹理贴图句柄 片元句柄


    protected FloatBuffer mVertexBuffer; // 顶点坐标Buffer
    protected FloatBuffer mTextureBuffer; // 纹理坐标Buffer


    //顶点坐标
    private float pos[] = {
            -1.0f, 1.0f,
            -1.0f, -1.0f,
            1.0f, 1.0f,
            1.0f, -1.0f,
    };

    //纹理坐标
    private float[] coord = {
            0.0f, 0.0f,
            0.0f, 1.0f,
            1.0f, 0.0f,
            1.0f, 1.0f,
    };

    //单位矩阵
    public static final float[] OM = new float[]{
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1
    };


    public OseFilter(Context context) {
        // 读出顶点着色器:vertexSource 和 片源着色器:fragmentSource
        String vertexSource = OpenUtils.readRawTextFile(context, R.raw.camera_vertex);
        String fragmentSource = OpenUtils.readRawTextFile(context, R.raw.camera_frag);

        mProgram = createOpenGLProgram(vertexSource, fragmentSource);
        if (mProgram != 0) {
            // 顶点
            mHPosition = GLES20.glGetAttribLocation(mProgram, "vPosition");
            mHCoord = GLES20.glGetAttribLocation(mProgram, "vCoord");
            mHMatrix = GLES20.glGetUniformLocation(mProgram, "vMatrix");
            // 片元
            mHTexture = GLES20.glGetUniformLocation(mProgram, "vTexture");

            //创建一个数据缓冲区
            //4个点 每个点两个数据(x,y) 数据类型float
            mVertexBuffer = ByteBuffer.allocateDirect(4 * 2 * 4)
                    .order(ByteOrder.nativeOrder())
                    .asFloatBuffer();

            mVertexBuffer.put(pos);
            mVertexBuffer.position(0);

            // 纹理 buffer
            mTextureBuffer = ByteBuffer.allocateDirect(4 * 2 * 4)
                    .order(ByteOrder.nativeOrder())
                    .asFloatBuffer();
            mTextureBuffer.clear();
            mTextureBuffer.put(coord);
            mTextureBuffer.position(0);

        }


    }

    private int textureId = 0;
    private float[] matrix = Arrays.copyOf(OM, 16);

    public final void setTextureId(int textureId) {
        this.textureId = textureId;
    }

    public void setMatrix(float[] matrix) {
        this.matrix = matrix;
    }

    public void draw() {
        onClear();

        // 使用着色器程序
        GLES20.glUseProgram(mProgram);

        // 设置其他扩展数据
        onSetExpandData();

        // 绑定默认纹理
        onBindTexture();

        onDraw();


    }

    /**
     * 清除画布
     */
    protected void onClear() {
        GLES20.glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        GLES20.glClear(GLES20.GL_COLOR_BUFFER_BIT | GLES20.GL_DEPTH_BUFFER_BIT);
    }

    /**
     * 设置其他扩展数据
     */
    protected void onSetExpandData() {
        //3、变换矩阵
        GLES20.glUniformMatrix4fv(mHMatrix, 1, false, matrix, 0);
    }

    /**
     * 绑定默认纹理
     */
    protected void onBindTexture() {
        // 激活图层
        GLES20.glActiveTexture(GLES20.GL_TEXTURE0);
        // 图像数据
        GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, textureId);
        //传递参数 0：需要和纹理层GL_TEXTURE0对应
        GLES20.glUniform1i(mHTexture, 0);
    }

    /**
     * 启用顶点坐标和纹理坐标进行绘制
     */
    protected void onDraw() {
        // 1、将顶点数据传入，确定形状
        GLES20.glVertexAttribPointer(mHPosition, 2, GLES20.GL_FLOAT, false, 0, mVertexBuffer);
        //传了数据之后 激活
        GLES20.glEnableVertexAttribArray(mHPosition);


        //2、将纹理坐标传入，采样坐标
        GLES20.glVertexAttribPointer(mHCoord, 2, GLES20.GL_FLOAT, false, 0, mTextureBuffer);
        GLES20.glEnableVertexAttribArray(mHCoord);

        //参数传完了 通知opengl 画画 从第0点开始 共4个点
        GLES20.glDrawArrays(GLES20.GL_TRIANGLE_STRIP, 0, 4);


        GLES20.glDisableVertexAttribArray(mHPosition);
        GLES20.glDisableVertexAttribArray(mHCoord);
    }

    /**
     * 生成OpenGL Program
     *
     * @param vertexSource   顶点着色器代码
     * @param fragmentSource 片元着色器代码
     * @return 生成的OpenGL Program，如果为0，则表示创建失败
     */
    private static int createOpenGLProgram(String vertexSource, String fragmentSource) {
        // 1. 创建顶点着色器
        int vertex = loadShader(GLES20.GL_VERTEX_SHADER, vertexSource);
        if (vertex == 0) {
            Log.e(TAG, "loadShader vertex failed");
            return 0;
        }
        // 2.创建片元着色器
        int fragment = loadShader(GLES20.GL_FRAGMENT_SHADER, fragmentSource);
        if (fragment == 0) {
            Log.e(TAG, "loadShader fragment failed");
            return 0;
        }
        // 3、创建着色器程序
        int program = GLES20.glCreateProgram();
        if (program != 0) {
            // 把着色器塞到程序当中
            GLES20.glAttachShader(program, vertex);
            GLES20.glAttachShader(program, fragment);
            // 链接着色器
            GLES20.glLinkProgram(program);
            int[] linkStatus = new int[1];
            // 获得程序是否配置成功
            GLES20.glGetProgramiv(program, GLES20.GL_LINK_STATUS, linkStatus, 0);
            if (linkStatus[0] != GLES20.GL_TRUE) {
                Log.e(TAG, "Could not link program:" + GLES20.glGetProgramInfoLog(program));
                GLES20.glDeleteProgram(program);
                program = 0;
            }
        }
        //因为已经塞到着色器程序中了，所以删了没关系
        GLES20.glDeleteShader(vertex);
        GLES20.glDeleteShader(fragment);
        return program;
    }

    /**
     * 加载着色器
     *
     * @param type       加载着色器类型
     * @param shaderCode 加载着色器的代码
     */
    private static int loadShader(int type, String shaderCode) {
        //1. 根据type创建顶点着色器或者片元着色器
        int shader = GLES20.glCreateShader(type);
        //2. 将着色器的代码加入到着色器中
        GLES20.glShaderSource(shader, shaderCode);
        //3. 编译着色器
        GLES20.glCompileShader(shader);
        return shader;
    }


}
