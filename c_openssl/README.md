## 使用clang编译openssl1.1.1d 

openssl下载地址

https://www.openssl.org/source/openssl-1.1.1d.tar.gz

ndk要使用r19以上

ndk各个 版本的下载地址

https://blog.csdn.net/u011077027/article/details/102706283



使用clang编译 要设置ANDROID_API>=21，配置ndk的路径，如果需要编译其他平台的，只需要修改architecture的值



shell脚本

```shell
#!/bin/bash
set -e
set -x

# Set directory
SCRIPTPATH=/root/openssl
export ANDROID_NDK_HOME=/root/android/android-ndk-r20
export OPENSSL_DIR=/root/openssl/openssl-1.1.1d

# Find the toolchain for your build machine
toolchains_path=/root/android/android-ndk-r20/toolchains/llvm/prebuilt/linux-x86_64

# Configure the OpenSSL environment, refer to NOTES.ANDROID in OPENSSL_DIR
# Set compiler clang, instead of gcc by default
CC=clang

# Add toolchains bin directory to PATH
PATH=$toolchains_path/bin:$PATH

# Set the Android API levels
ANDROID_API=21

# Set the target architecture
# Can be android-arm, android-arm64, android-x86, android-x86 etc

architecture=android-arm

# Create the make file
cd ${OPENSSL_DIR}
#./Configure ${architecture} -D__ANDROID_API__=$ANDROID_API -I

./config no-ssl2 no-ssl3 no-comp no-hw no-engine \
--openssldir=$(pwd)/../out/$ANDROID_API \
--prefix=$(pwd)/../out/$ANDROID_API

# Build
make
# Copy the outputs
OUTPUT_INCLUDE=$SCRIPTPATH/output/include
OUTPUT_LIB=$SCRIPTPATH/output/lib/${architecture}
mkdir -p $OUTPUT_INCLUDE
mkdir -p $OUTPUT_LIB
cp -RL include/openssl $OUTPUT_INCLUDE
cp libcrypto.so $OUTPUT_LIB
cp libcrypto.a $OUTPUT_LIB
cp libssl.so $OUTPUT_LIB
cp libssl.a $OUTPUT_LIB
```



