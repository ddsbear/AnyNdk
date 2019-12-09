#!/bin/bash
set -e
set -x

# Set directory
SCRIPTPATH=/root/gmssl
export ANDROID_NDK_HOME=/root/android/android-ndk-r20
export OPENSSL_DIR=/root/gmssl/GmSSL-master

# Find the toolchain for your build machine
toolchains_path=${ANDROID_NDK_HOME}/toolchains/llvm/prebuilt/linux-x86_64

# Configure the OpenSSL environment, refer to NOTES.ANDROID in OPENSSL_DIR
# Set compiler clang, instead of gcc by default
CC=clang

# Add toolchains bin directory to PATH
PATH=$toolchains_path/bin:$PATH

# Set the Android API levels
ANDROID_API=21

# Set the target architecture
# Can be android-arm, android-arm64, android-x86, android-x86 etc

outdir=x86
architecture=android-x86

# Create the make file
cd ${OPENSSL_DIR}


./Configure ${architecture} -D__ANDROID_API__=$ANDROID_API no-asm no-unit-test no-serpent


#./config no-ssl2 no-ssl3 no-comp no-hw no-engine \
#--openssldir=$(pwd)/../out/$ANDROID_API \
#--prefix=$(pwd)/../out/$ANDROID_API


# Build
make

# Copy the outputs
OUTPUT_INCLUDE=$SCRIPTPATH/output/include
OUTPUT_LIB=$SCRIPTPATH/output/lib/${outdir}
mkdir -p $OUTPUT_INCLUDE
mkdir -p $OUTPUT_LIB
cp -RL include/openssl $OUTPUT_INCLUDE
cp libcrypto.so $OUTPUT_LIB
cp libcrypto.a $OUTPUT_LIB
cp libssl.so $OUTPUT_LIB
cp libssl.a $OUTPUT_LIB

