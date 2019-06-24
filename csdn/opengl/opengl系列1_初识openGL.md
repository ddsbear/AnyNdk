## 徒手撸一个抖音APP(1) 初识openGL ES

闲来无事，徒手撸一个抖音App，因为是写的笔记，所以顺序会有些乱，但基本上分为下面这几个步骤

- 初识openGL的使用

- 显示摄像头
- 录制视频
- 添加功能：设置录制视频速度
- 添加功能：各种特效

这里是开篇

[TOC]


## 1. 什么是OpenGL？ 

**Open Graphics Library**

​	图形领域的工业标准，是一套跨编程语言、跨平台的、专业的图形编程(软件)接口。它用于二维、三维图像，是一个功能强大，调用方便的底层图形库。
​	与硬件无关。可以在不同的平台如Windows、Linux、Mac、Android、IOS之间进行移植。因此，支持OpenGL的软件具有很好的移植性，可以获得非常广泛的应用。

目前广泛支持的是2.0

```xml
<uses-feature android:glEsVersion="0x00020000" android:required="true"/>
```

**GLSurfaceView**
继承至SurfaceView，它内嵌的surface专门负责OpenGL渲染。
 - 管理Surface与EGL
 - 允许自定义渲染器(render)。
 - 让渲染器在独立的线程里运作，和UI线程分离。
 -  支持按需渲染(on-demand)和连续渲染(continuous)。



## 2. OpenGL的绘制流程

一句话概括就是将几何信息转换成一个个的栅格组成的图像的过程。

![openGL绘制流程](D:\github\AnyNdk\csdn\art\openGL绘制流程.jpg)



1. 获取顶点数据，然后将顶点数据传递给顶点着色器，确定图元顶点位置
2. 进行图元装配
3. 光栅化
4. 片元着色器处理光、阴影等对图像的影响，从而进行颜色填充，然后对每个片元进行处理，最后帧缓存。

## 3. OpenGL的坐标系

![openGL坐标系](D:\github\AnyNdk\csdn\art\openGL坐标系.png)





- OpenGL世界坐标，取值范围[-1,1], 在OpenGL中用来确定形状的，顶点位置坐标就是按照这个坐标来给的
- 纹理坐标，取值范围[0,1]，在OpenGL进行纹理的操作，是根据这个坐标来操作的。
- Android屏幕坐标，取值范围[0,1]，根据设备屏幕的坐标，来显示OpenGL中的内容。

## 4. Shader着色器

- 着色器（Shader）是运行在GPU上的小程序
- 顶点着色器
  \- 如何处理顶点、法线等数据的小程序
- 片元着色器
  \- 如何处理光、阴影、遮挡、环境等等对物体表面的影响，最终生成一副图像的小程序



## 5. GLSL

OpenGL着色语言（OpenGL Shading Language）

### 数据类型

float 浮点型
vec2 含有两个浮点型数据的向量
vec4 含有四个浮点型数据的向量
sampler2D 2D纹理采样器（代表一层纹理）

### 修饰符

attribute 属性变量。只能用于顶点着色器中。 一般用该变量来表示一些顶点数据，如：顶点坐标、纹理坐标、颜色等。
uniforms 一致变量。在着色器执行期间一致变量的值是不变的。与const常量不同的是，这个值在编译时期是未知的是由着色器外部初始化的。
varying 易变变量。是从顶点着色器传递到片元着色器的数据变量。

### 内建函数

texture2D (采样器 坐标) 采样指定位置的纹理
texture2D(采样器，采样点的坐标)

### 内建变量

顶点着色器
gl_Position vec4 顶点位置
片元着色器
gl_FragColor vec4 颜色

### 其他

precision lowp	低精度
precision mediump	中精度
precision highp	高精度





**例子：**

```c
// 把顶点坐标给这个变量， 确定要画画的形状
attribute vec4 vPosition;
//接收纹理坐标，接收采样器采样图片的坐标
attribute vec4 vCoord;
//变换矩阵， 需要将原本的vCoord（01,11,00,10） 与矩阵相乘 才能够得到 surfacetexure(特殊)的正确的采样坐标
uniform mat4 vMatrix;
//传给片元着色器 像素点
varying vec2 aCoord;
void main(){
    //内置变量 gl_Position ,我们把顶点数据赋值给这个变量 opengl就知道它要画什么形状了
    gl_Position = vPosition;
    // 进过测试 和设备有关
    aCoord = (vMatrix * vCoord).xy;
    //aCoord =  vec2((vCoord*vMatrix).x,(vCoord*vMatrix).y);
}
```



```c
#extension GL_OES_EGL_image_external : require
//SurfaceTexture比较特殊
//float数据是什么精度的
precision mediump float;
//采样点的坐标
varying vec2 aCoord;
//采样器
uniform samplerExternalOES vTexture;
void main(){
    //变量 接收像素值
    // texture2D：采样器 采集 aCoord的像素
    //赋值给 gl_FragColor 就可以了
    gl_FragColor = texture2D(vTexture,aCoord);
}
```

















