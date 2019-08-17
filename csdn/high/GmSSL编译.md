## GmSSL Android 交叉编译

最近没事就编译了一下openssl的国密支持项目GmSSL，将其移植到Android端

## 环境准备

gcc , g++，make，**python**

这些必须都有的，ndk的版本也是有限制的，之前我使用ndk17编译的版本某些方法会崩溃，也不知是什么原因

所以我们选择ndk版本比较低的，比如，**ndk-r10e-linux-x86_64**，我暂时使用的是**android-ndk--r14b**,好像也没啥问题

我使用的ubuntu18.04的系统，用户是非root用户



## 开始编译

首先下载源码

```java
git clone https://github.com/guanzhi/GmSSL
```

进入项目，编写build_armv7.sh，这里是仿照官网的方式进行，方便理解

有几点需要注意

1. 不要在windows上写好传到linux，要直接复制文字到linux里，不然会有格式问题
2. python必须要
3. TOOLCHAIN_PATH这个东西可以给个随便的位置

**build_armv7.sh**

```shell
#!/bin/bash

# 这个路径就只是ndk的父目录而已
ANDROID_PATH=/home/dds/Android

PLATFORM_VERSION=22

MAKE_TOOLCHAIN=$ANDROID_PATH/android-ndk-r14b/build/tools/make-standalone-toolchain.sh
export TOOLCHAIN_PATH=$ANDROID_PATH/android-toolchain-arm
$MAKE_TOOLCHAIN --arch=arm --platform=android-$PLATFORM_VERSION --install-dir=$TOOLCHAIN_PATH

export MACHINE=armv7
export SYSTEM=android
export ARCH=arm
export CROSS_SYSROOT=$TOOLCHAIN_PATH/sysroot
export TOOL_BASENAME=$TOOLCHAIN_PATH/bin/arm-linux-androideabi
export CC=$TOOL_BASENAME-gcc
export CXX=$TOOL_BASENAME-g++
export LD=$TOOL_BASENAME-ld
export LINK=$CXX
export AR=$TOOL_BASENAME-ar
export RANLIB=$TOOL_BASENAME-ranlib
export STRIP=$TOOL_BASENAME-strip

./config
make
```

执行完了之后执行下面操作，继续之后别的arm的编译

```shell
make clean 
```

**build_armv8.sh**

```shell
ANDROID_PATH=/home/dds/Android

PLATFORM_VERSION=22

MAKE_TOOLCHAIN=$ANDROID_PATH/android-ndk-r14b/build/tools/make-standalone-toolchain.sh
export TOOLCHAIN_PATH=$ANDROID_PATH/aarch64-linux-android
$MAKE_TOOLCHAIN --arch=aarch64 --platform=android-$PLATFORM_VERSION --install-dir=$TOOLCHAIN_PATH

export MACHINE=armv8
export SYSTEM=android64-aarch64
export ARCH=aarch64
export CROSS_SYSROOT=$TOOLCHAIN_PATH/sysroot
export TOOL_BASENAME=$TOOLCHAIN_PATH/bin/aarch64-linux-android
export CC=$TOOL_BASENAME-gcc
export CXX=$TOOL_BASENAME-g++
export LD=$TOOL_BASENAME-ld
export LINK=$CXX
export AR=$TOOL_BASENAME-ar
export RANLIB=$TOOL_BASENAME-ranlib
export STRIP=$TOOL_BASENAME-strip

./config
make
```



**build_x86.sh**

```shell
ANDROID_PATH=/home/dds/Android

PLATFORM_VERSION=22

MAKE_TOOLCHAIN=$ANDROID_PATH/android-ndk-r14b/build/tools/make-standalone-toolchain.sh
export TOOLCHAIN_PATH=$ANDROID_PATH/aarch64-linux-android
$MAKE_TOOLCHAIN --arch=aarch64 --platform=android-$PLATFORM_VERSION --install-dir=$TOOLCHAIN_PATH

export MACHINE=x86
export SYSTEM=android-x86
export ARCH=x86
export CROSS_SYSROOT=$TOOLCHAIN_PATH/sysroot
export TOOL_BASENAME=$TOOLCHAIN_PATH/bin/i686-linux-android
export CC=$TOOL_BASENAME-gcc
export CXX=$TOOL_BASENAME-g++
export LD=$TOOL_BASENAME-ld
export LINK=$CXX
export AR=$TOOL_BASENAME-ar
export RANLIB=$TOOL_BASENAME-ranlib
export STRIP=$TOOL_BASENAME-strip

./config
make
```





```shell
ANDROID_PATH=/home/dds/Android

PLATFORM_VERSION=22

MAKE_TOOLCHAIN=$ANDROID_PATH/android-ndk-r14b/build/tools/make-standalone-toolchain.sh
export TOOLCHAIN_PATH=$ANDROID_PATH/x86_64
$MAKE_TOOLCHAIN --arch=x86_64 --platform=android-$PLATFORM_VERSION --install-dir=$TOOLCHAIN_PATH

export MACHINE=x86_64
export SYSTEM=android64
export ARCH=x86_64
export CROSS_SYSROOT=$TOOLCHAIN_PATH/sysroot
export TOOL_BASENAME=$TOOLCHAIN_PATH/bin/x86_64-linux-android
export CC=$TOOL_BASENAME-gcc
export CXX=$TOOL_BASENAME-g++
export LD=$TOOL_BASENAME-ld
export LINK=$CXX
export AR=$TOOL_BASENAME-ar
export RANLIB=$TOOL_BASENAME-ranlib
export STRIP=$TOOL_BASENAME-strip

./config
make
```



## 收录代码

代码和编译好的库收录在

[https://github.com/ddssingsong/AnyNdk](https://github.com/ddssingsong/AnyNdk)





