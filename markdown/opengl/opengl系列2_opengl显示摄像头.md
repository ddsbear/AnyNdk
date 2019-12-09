## 徒手撸一个抖音App(2) openGL显示摄像头

下面我们开始动手撸一个抖音APP，根据实际需求，会分为下面几步

- openGL基础
- 显示摄像头
- 录制视频
- 添加功能：设置录制视频速度
- 添加功能：各种特效

这里是第二部分，openGL显示摄像头



## 1. 权限

```xml
<!--openGL版本-->
    <uses-feature
        android:glEsVersion="0x00020000"
        android:required="true" />
    <!--摄像头权限-->
    <uses-permission android:name="android.permission.CAMERA" />
    <!--录音权限-->
    <uses-permission android:name="android.permission.RECORD_AUDIO" />
    <!--写入文件权限-->
    <uses-permission android:name="android.permission.WRITE_EXTERNAL_STORAGE" />
    <uses-permission android:name="android.permission.READ_EXTERNAL_STORAGE" />
```

## 2. 初始化画布	

```java
public class CaptureView extends GLSurfaceView implements GLSurfaceView.Renderer {
    // Render类
    private GLCameraDrawer glCameraDrawer;
    // 相机控制类
    private CameraHelper cameraHelper;
    // 要打开的摄像头的ID
    public static int cameraId = Camera.CameraInfo.CAMERA_FACING_BACK;

    public CaptureView(Context context) {
        super(context);
        init();
    }

    public CaptureView(Context context, AttributeSet attrs) {
        super(context, attrs);
        init();
    }

    private void init() {
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
    //=============================Render回调=========================================

    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        // 初始化render
        glCameraDrawer.onSurfaceCreated(gl, config);
        // 打开摄像头
        cameraHelper.open(cameraId);
        // 获取预览宽高
        Point point = cameraHelper.getPreviewSize();
        glCameraDrawer.setDataSize(point.x, point.y);
        // 设置预览控件
        cameraHelper.setPreviewTexture(glCameraDrawer.getSurfaceTexture());
        // 渲染回调
        glCameraDrawer.getSurfaceTexture().setOnFrameAvailableListener(new SurfaceTexture.OnFrameAvailableListener() {
            @Override
            public void onFrameAvailable(SurfaceTexture surfaceTexture) {
                requestRender();
            }
        });
        // 开始预览画面
        cameraHelper.preview();

    }

    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height) {
        // 界面改变
        GLES20.glViewport(0, 0, width, height);
        glCameraDrawer.setViewSize(width, height);
    }

    @Override
    public void onDrawFrame(GL10 gl) {
        // 画出界面
        glCameraDrawer.onDrawFrame(gl);

    }

    @Override
    public void onPause() {
        super.onPause();
        // 关闭摄像头预览
        cameraHelper.close();
    }


```

## 3. 初始化相机控制类

```java
public class CameraHelper {
    private static final String TAG = "dds_CameraHelper";
    private Config mConfig;
    private Camera mCamera;
    private CameraSizeComparator sizeComparator;
    private Point mPreSize;

    public CameraHelper() {
        this.mConfig = new Config();
        mConfig.minPreviewWidth = 720;
        mConfig.minPictureWidth = 720;
        mConfig.rate = 1.778f;
        sizeComparator = new CameraSizeComparator();
    }

    // 打开相机
    public boolean open(int cameraId) {
        mCamera = Camera.open(cameraId);
        if (mCamera != null) {
            // 获取合适的图片尺寸和预览尺寸
            Camera.Parameters param = mCamera.getParameters();
            Camera.Size picSize = getPropPictureSize(param.getSupportedPictureSizes(), mConfig.rate, mConfig.minPictureWidth);
            Camera.Size preSize = getPropPreviewSize(param.getSupportedPreviewSizes(), mConfig.rate, mConfig.minPreviewWidth);
            param.setPictureSize(picSize.width, picSize.height);
            param.setPreviewSize(preSize.width, preSize.height);
            mCamera.setParameters(param);


            Camera.Size pre = param.getPreviewSize();
            Log.d(TAG, "preview height:" + pre.height + ",width:" + pre.width);
            // 注意，这里写反的话会导致界面被压缩
            mPreSize = new Point(pre.height, pre.width);
            return true;
        }
        return false;
    }

    // 开始预览
    public boolean preview() {
        if (mCamera != null) {
            mCamera.startPreview();
        }
        return true;
    }

    // 关闭摄像头
    public boolean close() {
        if (mCamera != null) {
            try {
                mCamera.stopPreview();
                mCamera.release();
            } catch (Exception e) {
                e.printStackTrace();
                return false;
            }
        }
        return true;
    }

    // 设置渲染的控件SurfaceTexture
    public void setPreviewTexture(SurfaceTexture texture) {
        if (mCamera != null) {
            try {
                mCamera.setPreviewTexture(texture);
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }

    // 获取预览的宽高
    public Point getPreviewSize() {
        return mPreSize;
    }

    private Camera.Size getPropPreviewSize(List<Camera.Size> list, float th, int minWidth) {
        Collections.sort(list, sizeComparator);

        int i = 0;
        for (Camera.Size s : list) {
            if ((s.height >= minWidth) && equalRate(s, th)) {
                break;
            }
            i++;
        }
        if (i == list.size()) {
            i = 0;
        }
        return list.get(i);
    }

    private Camera.Size getPropPictureSize(List<Camera.Size> list, float th, int minWidth) {
        Collections.sort(list, sizeComparator);
        int i = 0;
        for (Camera.Size s : list) {
            if ((s.height >= minWidth) && equalRate(s, th)) {
                break;
            }
            i++;
        }
        if (i == list.size()) {
            i = 0;
        }
        return list.get(i);
    }

    private boolean equalRate(Camera.Size s, float rate) {
        float r = (float) (s.width) / (float) (s.height);
        if (Math.abs(r - rate) <= 0.03) {
            return true;
        } else {
            return false;
        }
    }


    private class CameraSizeComparator implements Comparator<Camera.Size> {
        public int compare(Camera.Size lhs, Camera.Size rhs) {
            if (lhs.height == rhs.height) {
                return 0;
            } else if (lhs.height > rhs.height) {
                return 1;
            } else {
                return -1;
            }
        }
    }

    class Config {
        float rate; //宽高比
        int minPreviewWidth;
        int minPictureWidth;
    }
}

```

## 4. 初始化GLCameraDrawer

```java
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
         // 创建纹理，生成你要操作的纹理对象的索引
        GLES20.glGenTextures(texture.length, texture, 0); 
        // 绑定纹理，告诉OpenGL下面代码中对2D纹理的任何设置都是针对索引为0的纹理的
        GLES20.glBindTexture(GLES11Ext.GL_TEXTURE_EXTERNAL_OES, texture[0]); 

        // glTexParameterF 是设置纹理贴图的参数属性
        GLES20.glTexParameterf(GLES11Ext.GL_TEXTURE_EXTERNAL_OES, GL10.GL_TEXTURE_MIN_FILTER, GL10.GL_LINEAR);
        GLES20.glTexParameterf(GLES11Ext.GL_TEXTURE_EXTERNAL_OES, GL10.GL_TEXTURE_MAG_FILTER, GL10.GL_LINEAR);

        // OpenGL——纹理过滤函数glTexParameterI() 确定如何把纹理象素映射成像素.
        GLES20.glTexParameteri(GLES11Ext.GL_TEXTURE_EXTERNAL_OES, GL10.GL_TEXTURE_WRAP_S, GL10.GL_CLAMP_TO_EDGE);
        GLES20.glTexParameteri(GLES11Ext.GL_TEXTURE_EXTERNAL_OES, GL10.GL_TEXTURE_WRAP_T, GL10.GL_CLAMP_TO_EDGE);
        return texture[0];

    }

}
```

## 5. 开始编写OseFilter

```java
public class OseFilter {
    private static final String TAG = "dds_Filter";

    protected int mProgram;    // 程序句柄

    protected int mHPosition;  // 顶点坐标句柄
    protected int mHCoord;     // 纹理坐标句柄
    protected int mHMatrix;    // 总变换矩阵句柄

    protected int mHTexture;   // 默认纹理贴图句柄 片元句柄


    protected FloatBuffer mVertexBuffer; // 顶点坐标Buffer
    protected FloatBuffer mTextureBuffer; // 纹理坐标Buffer


    //单位矩阵
    public static final float[] OM = new float[]{
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1
    };

    private int textureId = 0;
    private float[] matrix = Arrays.copyOf(OM, 16);

    private int mHCoordMatrix;
    private float[] mCoordMatrix = Arrays.copyOf(OM, 16);


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


    public OseFilter() {
        //创建一个数据缓冲区
        //4个点 每个点两个数据(x,y) 数据类型float
        mVertexBuffer = ByteBuffer.allocateDirect(4 * 2 * 4)
                .order(ByteOrder.nativeOrder())
                .asFloatBuffer();
        mVertexBuffer.clear();
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


    public final void setTextureId(int textureId) {
        this.textureId = textureId;
    }

    public void setMatrix(float[] matrix) {
        this.matrix = matrix;
    }

    public final void create(Context context) {
        // 读出顶点着色器:vertexSource 和 片源着色器:fragmentSource
        String vertexSource = OpenUtils.readRawTextFile(context, R.raw.camera_vertex);
        String fragmentSource = OpenUtils.readRawTextFile(context, R.raw.camera_frag);

        mProgram = createOpenGLProgram(vertexSource, fragmentSource);
        // 顶点
        mHPosition = GLES20.glGetAttribLocation(mProgram, "vPosition");
        mHCoord = GLES20.glGetAttribLocation(mProgram, "vCoord");
        mHMatrix = GLES20.glGetUniformLocation(mProgram, "vMatrix");
        // 片元
        mHTexture = GLES20.glGetUniformLocation(mProgram, "vTexture");

        //自定义
        mHCoordMatrix = GLES20.glGetUniformLocation(mProgram, "vCoordMatrix");


    }


    public void draw() {
        onClear();

        // 使用着色器程序
        onUseProgram();

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
     * 使用着色器程序
     */
    protected void onUseProgram() {
        GLES20.glUseProgram(mProgram);
    }

    /**
     * 设置其他扩展数据
     */
    protected void onSetExpandData() {
        //3、变换矩阵
        GLES20.glUniformMatrix4fv(mHMatrix, 1, false, matrix, 0);

        GLES20.glUniformMatrix4fv(mHCoordMatrix, 1, false, mCoordMatrix, 0);
    }

    /**
     * 绑定默认纹理
     */
    protected void onBindTexture() {
        // 激活图层
//        GLES20.glActiveTexture(GLES20.GL_TEXTURE0);
//        // 图像数据
//        GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, textureId);
//        //传递参数 0：需要和纹理层GL_TEXTURE0对应
//        GLES20.glUniform1i(mHTexture, 0);


        GLES20.glActiveTexture(GLES20.GL_TEXTURE0);
        GLES20.glBindTexture(GLES11Ext.GL_TEXTURE_EXTERNAL_OES, textureId);
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

```

其中**顶点着色器**和**片元着色器**如下

camera_vertex

```c
attribute vec4 vPosition;
attribute vec2 vCoord;
uniform mat4 vMatrix;
uniform mat4 vCoordMatrix;
varying vec2 textureCoordinate;
void main(){
    gl_Position = vMatrix*vPosition;
    textureCoordinate = (vCoordMatrix*vec4(vCoord,0,1)).xy;
}
```

camera_frag

```c
#extension GL_OES_EGL_image_external : require
precision mediump float;
varying vec2 textureCoordinate;
uniform samplerExternalOES vTexture;

void main(){
    gl_FragColor = texture2D(vTexture,textureCoordinate);
}
```

读取文件

```java
 public static String readRawTextFile(Context context, int rawId) {
        InputStream is = context.getResources().openRawResource(rawId);
        BufferedReader br = new BufferedReader(new InputStreamReader(is));
        String line;
        StringBuilder sb = new StringBuilder();
        try {
            while ((line = br.readLine()) != null) {
                sb.append(line);
                sb.append("\n");
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
        try {
            br.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
        return sb.toString();
    }
```



## 代码

本教程已发布到release，可下载相应的release的代码

https://github.com/ddssingsong/AnyNdk