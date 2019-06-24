## 徒手撸一个抖音App(1) openGL显示摄像头

下面我们开始动手撸一个抖音APP，根据实际需求，会分为下面几步

- 显示摄像头

- 录制视频

- 添加功能：设置录制视频速度
- 添加功能：各种特效

这里是第一部分，openGL显示摄像头



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
public class CaptureView extends GLSurfaceView {
    private CaptureRenderer captureRenderer;
    public CaptureView(Context context) {
        this(context,null);
    }
    public CaptureView(Context context, AttributeSet attrs) {
        super(context, attrs);
        // 初始化Render
        captureRenderer = new CaptureRenderer(this);
        // 设置EGL版本
        setEGLContextClientVersion(2);
        // 设置Render
        setRenderer(douyinRenderer);
        /**
         * 1. 连续渲染
         * 2. 按需渲染
         * 這裏设置按需渲染
         */
        setRenderMode(RENDERMODE_WHEN_DIRTY);
    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {
        super.surfaceDestroyed(holder);
        captureRenderer.onSurfaceDestroyed();
    }
}

```

## 3. 初始化captureRenderer









