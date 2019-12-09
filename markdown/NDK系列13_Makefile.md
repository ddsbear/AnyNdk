## 【NDK系列13】Makefile Android.mk Application.mk 详解

我们在使用第三方库时，很多都提供makefile，我们需要读懂他们并且适当的修改他们，另外虽说现在google推荐使用cmake，但是如果遇见Android.mk还是需要能够读懂。

## 1. 什么是Makefile

​	无论是c、c++首先要把源文件编译成中间代码文件，在Windows下也就是 .obj 文件，UNIX下是 .o 文件，即 Object File，这个动作叫做``编译（compile）``，然后再把大量的Object File合成执行文件或者静动态库，这个动作叫作``链接（link）``。

一个工程中的源文件不计数，其按类型、功能、模块分别放在若干个目录中，makefile定义了一系列的规则来指定，哪些文件需要先编译，哪些文件需要后编译，如何进行链接等等操作。

makefile 就是“自动化编译”，告诉make命令如何编译和链接,即make工具的配置脚本。

默认的情况下，gun make命令会在当前目录下按顺序找寻文件

`"GNUmakefile”、“makefile”、“Makefile”`
最好不要用“GNUmakefile”，这个文件是GNU的make识别的（Windows Nmake就不识别）
​        

​	 当然，也可以使用别的文件名来书写Makefile，比如：“Make.Linux”，“Make.android”。这样在使用时候就需要 `` make -f XX 或者 make --file XX。``

## 2. Makefile规则

### 基本用法

在Makefile中的命令，必须要以[Tab]键开始。

```makefile
在Makefile中的命令，必须要以[Tab]键开始。

target : prerequisites ...(预备知识，先决条件)
	command（指令）
-----------------------------------------------------------------------------------------
      target也就是一个目标文件，可以是Object File，也可以是执行文件。还可以是一个标签。
      prerequisites 要生成那个target所需要的文件或是其他target。
      command也就是make需要执行的命令。（任意的Shell命令）
```

如下例子

```makefile
# g++ -o  指定生成可执行文件的名称
# 下面的方法就是将main.o和test.o编译成test可执行程序test
test:main.o test1.o
	g++ -o test main.o test1.o
```

当然也可以直接编译cpp

```makefile
# \ 是换行连接符 便于Makefile的易读,不用都挤在一行
test2:
	g++ -o test2 main.cpp \
test1.cpp
```

clean 

```makefile
clean:
	rm test main.o test.o
```

打印数据

```makefile
print:
	echo "hello world"
```

### 变量

如果比较复杂的情况，比如文件很多，target目标比较多，那么我们如果来修改，比如增加一个.cpp文件，

那可能需要在很多地方都写一下，也容易出错。为了易于维护，可以在makefile中使用变量。

```makefile
#声明变量
objects=main.o T1.o
#mac上自动编译 main.o
test:${objects}
	g++ -o test ${objects}

clean:
	rm test ${objects}
	
#======================================
# *.c 表示所有后缀为c的文件。
# 让通配符在变量中(当前目录下所有 .c 文件)
objects = $(wildcard *.c)
test : ${objects}
        gcc -o test ${objects}
clean :
        rm test ${obects}
```

### include

```makefile
include make.clean

#或者

mk=make.clean
include ${mk}
```

### 文件搜索

在一些大的工程中，有大量的源文件存放在不同的目录中,最好的方法是把一个路径告诉make，让make在自动去找。
Makefile文件中的特殊变量``VPATH``就是完成这个功能的

```makefile
#默认先查找当前目录再查找当前目录下的a、b、c目录
VPATH = a:b:c
OBJ=a.o b.o c.o main.o

test : $(OBJ)
        gcc -o  test  $(OBJ)
clean :
        rm test $(OBJ)
```

### 预定义变量

| 命令变量 | 含义                              |
| -------- | --------------------------------- |
| AR       | 函数库的打包程序，默认为"ar"      |
| AS       | 汇编语言编译程序,默认为"as"       |
| CC       | C语言编译程序,默认命令是"cc"      |
| CXX      | C++语言编译程序,默认命令是"g++"   |
| RM       | 文件删除程序的名称,默认值为 rm –f |
| ARFLAGS  | 库文件维护程序的选项,无默认值     |
| ASFLAGS  | 汇编程序的选项,无默认值           |
| CFLAGS   | C 编译器的选项,无默认值           |
| CPPFLAGS | C 预编译的选项,无默认值           |
| CXXFLAGS | C++编译器的选项,无默认值          |

### 自动变量

> $@  ``target的名字``  

```makefile
main.o:main.c
	gcc -c main.c -o main.o
#使用 $@ 代替 main.o
main.o:main.c
	gcc -c main.c -o $@
```

> $<  ``target依赖的第一个依赖文件名``

```makefile
main.o:main.c a.h b.h
	gcc -c main.c -o main.o
#使用 $< 代替 main.c
main.o:main.c a.h b.h
	gcc -c $< -o main.o
```

| 变量 | 说明                                                         |
| ---- | ------------------------------------------------------------ |
| $*   | 不包含扩展名的target文件名称                                 |
| $+   | 所有的依赖文件,以空格分开,并以出现的先后为序,可能包含 重复的依赖文件 |
| $?   | 所有时间戳比target文件晚的依赖文件,并以空格分开              |
| $^   | 所有不重复的依赖文件,以空格分开                              |

### 条件语句

```makefile

ifeq ($(CC),gcc)
        $(CC) -o foo $(objects) $(libs_for_gcc)
else
        $(CC) -o foo $(objects) $(normal_libs)
endif
```

### 输出信息

```makefile
AAA=123456
#输出变量AAA
$(warning $(AAA))
$(info $(AAA))
```

## 3. Android.mk

微小 GNU makefile 片段。

将源文件分组为*模块*。 模块是静态库、共享库或独立可执行文件。 可在每个 `Android.mk` 文件中定义一个或多个模块，也可在多个模块中使用同一个源文件。

```makefile
#源文件在的位置。宏函数 my-dir 返回当前目录
LOCAL_PATH := $(call my-dir)
#可为您清除变量
#不会清理 LOCAL_PATH 变量
include $(CLEAR_VARS)
#存储您要构建的模块的名称 每个模块名称必须唯一，且不含任何空格
#如果模块名称的开头已是 lib，则构建系统不会附加额外的前缀 lib；而是按原样采用模块名称，并添加 .so 扩展名。
LOCAL_MODULE := hello-jni
#包含要构建到模块中的 C 和/或 C++ 源文件列表 以空格分开
LOCAL_SRC_FILES := hello-jni.c
#构建动态库
include $(BUILD_SHARED_LIBRARY)
```

**变量和宏**

定义自己的任意变量。在定义变量时请注意，NDK 构建系统会预留以下变量名称：

- 以 `LOCAL_` 开头的名称，例如 `LOCAL_MODULE`。
- 以 `PRIVATE_`、`NDK_` 或 `APP` 开头的名称。构建系统在内部使用这些变量。
- 小写名称，例如 `my-dir`。构建系统也是在内部使用这些变量。

如果为了方便而需要在 `Android.mk` 文件中定义自己的变量，建议在名称前附加 `MY_`。

**常用内置变量**

| 变量名                  | 含义                       | 示例                               |
| ----------------------- | -------------------------- | ---------------------------------- |
| BUILD_STATIC_LIBRARY    | 构建静态库的Makefile脚本   | include $(BUILD_STATIC_LIBRARY)    |
| PREBUILT_SHARED_LIBRARY | 预编译共享库的Makeifle脚本 | include $(PREBUILT_SHARED_LIBRARY) |
| PREBUILT_STATIC_LIBRARY | 预编译静态库的Makeifle脚本 | include $(PREBUILT_STATIC_LIBRARY) |
| TARGET_PLATFORM         | Android API 级别号         | TARGET_PLATFORM := android-22      |
| TARGET_ARCH             | CUP架构                    | arm arm64 x86 x86_64               |
| TARGET_ARCH_ABI         | CPU架构                    | armeabi  armeabi-v7a  arm64-v8a    |

**模块描述变量**

| 变量名                       | 描述                                   | 例                                                     |
| ---------------------------- | -------------------------------------- | ------------------------------------------------------ |
| LOCAL_MODULE_FILENAME        | 覆盖构建系统默认用于其生成的文件的名称 | LOCAL_MODULE := foo LOCAL_MODULE_FILENAME := libnewfoo |
| LOCAL_CPP_FEATURES           | 特定 C++ 功能                          | 支持异常:LOCAL_CPP_FEATURES := exceptions              |
| LOCAL_C_INCLUDES             | 头文件目录查找路径                     | LOCAL_C_INCLUDES := $(LOCAL_PATH)/include              |
| LOCAL_CFLAGS                 | 构建 C *和* C++ 的编译参数             |                                                        |
| LOCAL_CPPFLAGS               | c++                                    |                                                        |
| LOCAL_STATIC_LIBRARIES       | 当前模块依赖的静态库模块列表           |                                                        |
| LOCAL_SHARED_LIBRARIES       |                                        |                                                        |
| LOCAL_WHOLE_STATIC_LIBRARIES | --whole-archive                        | 将未使用的函数符号也加入编译进入这个模块               |
| LOCAL_LDLIBS                 | 依赖 系统库                            | LOCAL_LDLIBS := -lz                                    |

导出给引入模块的模块使用

LOCAL_EXPORT_CFLAGS

LOCAL_EXPORT_CPPFLAGS

LOCAL_EXPORT_C_INCLUDES

LOCAL_EXPORT_LDLIBS

**引入其他模块**

```makefile
#将一个新的路径加入NDK_MODULE_PATH变量
#NDK_MODULE_PATH 变量是系统环境变量
$(call import-add-path,$(LOCAL_PATH)/platform/third_party/android/prebuilt)

#包含CocosDenshion/android目录下的mk文件
$(call import-module,CocosDenshion/android)

#这里即为 我需要引入 CocosDenshion/android 下面的Android.mk
#CocosDenshion/android 的路径会从 $(LOCAL_PATH)/platform/third_party/android/prebuilt 去查找
```



## 4. Application.mk

### 配置

同样是GNU Makefile 片段,在Application.mk中定义一些全局(整个项目)的配置

**APP_OPTIM**

将此可选变量定义为 `release` 或 `debug`。在构建应用的模块时可使用它来更改优化级别。发行模式是默认模式，可生成高度优化的二进制文件。调试模式会生成未优化的二进制文件，更容易调试。

**APP_CFLAGS**

为任何模块编译任何 C 或 C++ 源代码时传递到编译器的一组 C 编译器标志

**APP_CPPFLAGS**

构建 C++ 源文件时传递到编译器的一组 C++ 编译器标志。

**APP_ABI**

需要生成的cpu架构(ndk r17之上 只支持：armeabi-v7a, arm64-v8a, x86, x86_64)

**APP_PLATFORM**

此变量包含目标 Android 平台的名称。例如，`android-3` 指定 Android 1.5 系统映像

**APP_STL**

默认情况下，NDK 构建系统为 Android 系统提供的最小 C++ 运行时库 (`system/lib/libstdc++.so`) 提供 C++ 功能。 

### APP_ABI架构

不同 Android 手机使用不同的 CPU，因此支持不同的指令集。

**armeabi**

此 ABI 适用于基于 ARM、至少支持 ARMv5TE 指令集的 CPU。此 ABI 不支持硬件辅助的浮点计算。 相反，所有浮点运算都使用编译器 `libgcc.a` 静态库中的软件帮助程序函数。

**armeabi-v7a**

`armeabi-v7a` ABI 使用 `-mfloat-abi=softfp` 开关强制实施规则，要求编译器在函数调用时必须传递核心寄存器对中的所有双精度值，而不是专用浮点值。 系统可以使用 FP 寄存器执行所有内部计算。 这样可极大地加速计算。

如果要以 armeabi-v7a ABI 为目标，则必须设置下列标志：

```makefile
CFLAGS= -march=armv7-a -mfloat-abi=softfp -mfpu=vfpv3-d16
```

**arm64-v8a**

此 ABI 适用于基于 ARMv8、支持 AArch64 的 CPU。它还包含 NEON 和 VFPv4 指令集。

**x86**

此 ABI 适用于支持通常称为“x86”或“IA-32”的指令集的 CPU。设置的标志如：

```makefile
-march=i686 -mtune=intel -mssse3 -mfpmath=sse -m32
```

**x86_64**

```makefile
-march=x86-64 -msse4.2 -mpopcnt -m64 -mtune=intel
```

现在手机主要是armeabi-v7a。查看手机cpu：

```
adb shell cat /proc/cpuinfo
adb shell getprop ro.product.cpu.abi
```



apk在安装的时候，如果手机是armeabi-v7a的，则会首先查看apk中是否存在armeabi-v7a目录，如果没有就会查找armeabi。

**保证cpu目录下so数量一致。**

	如果目标是armeabi-v7a，但是拥有一个armeabi的，也可以把它放到armeabi-v7a目录下。但是反过来不行

| ABI(横 so)/CPU(竖 手机) | armeabi | armeabi-v7a | arm64-v8a | x86  | x86_64 |
| ----------------------- | ------- | ----------- | --------- | ---- | ------ |
| ARMV5                   | 支持    |             |           |      |        |
| ARMV7                   | 支持    | 支持        |           |      |        |
| ARMV8                   | 支持    | 支持        | 支持      |      |        |
| X86                     |         |             |           | 支持 |        |
| X86_64                  |         |             |           | 支持 | 支持   |





