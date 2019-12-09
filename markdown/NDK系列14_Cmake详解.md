# N【NDK系列14】Cmake详解 CMakeLists.txt详细配置

## 1. CMake简介

CMake是一个比make更高级的编译配置工具，它可以根据不同平台、不同的编译器，生成相应的Makefile或者vcproj项目。从而达到跨平台的目的。

Android Studio利用CMake生成的是ninja，ninja是一个小型的关注速度的构建系统。我们不需要关心ninja的脚本，知道怎么配置cmake就可以了。从而可以看出cmake其实是一个跨平台的支持产出各种不同的构建脚本的一个工具。

在android studio 2.2及以上，构建原生库的默认工具是 CMake。

## 2. 基本命令

```cmake
#cmake最低版本
cmake_minimum_required(VERSION 3.6.0)

#指定项目
project(Main)

add_executable(demo demo.cpp) # 生成可执行文件
add_library(common STATIC util.cpp) # 生成静态库
add_library(common SHARED util.cpp) # 生成动态库或共享库
```

**搜索c/cpp文件**

```cmake
# 搜索当前目录下的所有.cpp文件， 但不能查找子目录
aux_source_directory(. SRC_LIST) 
add_library(demo ${SRC_LIST})

#也可以 
file(GLOB DIR_SRCS *.c)
add_executable(main ${DIR_SRCS})

```

**使用子目录中的CMakeList**

```cmake
# 添加 child 子目录下的cmakelist
add_subdirectory(child)
# 指定生成目标 
add_library(main ${DIR_SRCS})
# 添加链接库
target_link_libraries(main child)

# ===========================================
# child中设置
aux_source_directory(. DIR_LIB_SRCS)
# 生成链接库 默认生成静态库
add_library (child ${DIR_LIB_SRCS})
#指定编译为静态库
add_library (child STATIC ${DIR_LIB_SRCS})
```

**设置预编译宏**

```cmake
#设置预编译宏 cflag和cxxflag
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DTEST"  )
set(CMAKE_Cxx_FLAGS "${CMAKE_Cxx_FLAGS} -DTEST"  )  
```

**引入头文件**

```cmake
#这样就可以使用 #include <xx> 引入 
#否则需要使用 #include "path/xx" 
include_directories( imported-lib/include/ )
```

**逻辑判断语句**

```cmake
#逻辑判断 计较字符串
set(ANDROID_ABI "areambi-v7a")
if(${ANDROID_ABI} STREQUAL "areambi")
  	message("armv5")
elseif(${ANDROID_ABI} STREQUAL "areambi-v7a")
	message("armv7a")
else()
	
endif()
```

## 3. build.gradle配置

```groovy
//还可以在gradle中使用 arguments  设置一些配置
externalNativeBuild {
      cmake {
        arguments "-DANDROID_TOOLCHAIN=clang",	//使用的编译器clang/gcc
                  "-DANDROID_STL=gnustl_static" //cmake默认就是 gnustl_static
        cFlags "" //这里也可以指定cflag和cxxflag,效果和之前的cmakelist里使用一样
        cppFlags "" 
      }
    }	
```

## 4. 注意事项

1. ​	使用Android.mk在 >=6.0 设备上不能再使用预编译动态库(静态库没问题)：

   ```cmake
   cmake_minimum_required(VERSION 3.4.1)
   
   file(GLOB SOURCE *.c )
   add_library(
                hello-jni
                SHARED
               ${SOURCE} )
               
   #这段配置在6.0依然没问题 
   set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -L[SO所在目录]")
   
   #这段配置只能在6.0以下使用 
   #add_library(Test SHARED IMPORTED)
   #set_target_properties(Test PROPERTIES IMPORTED_LOCATION [SO绝对地址])
   
   target_link_libraries(  hello-jni Test )
   ```

   

   2. 存在两个动态库` libhello-jni.so` 与 `libTest.so`。

      libhello-jni.so`依赖于`libTest.so` (使用NDK下的`ndk-depends`可查看依赖关系)，则：

      ```cmake
      //<=5.0:
      	System.loadLibrary("Test");
      	System.loadLibrary("hello-jni");
      //>=6.0:
      	System.loadLibrary("hello-jni");
      ```

      

   





