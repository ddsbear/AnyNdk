#include <jni.h>
#include <string>
#include <openssl/aes.h>
#include "cipher.h"


extern "C"
JNIEXPORT jstring JNICALL
Java_com_dds_openssl_OpenCipher_AesEncode(JNIEnv *env, jobject thiz,
                                          jstring _content,
                                          jstring _key) {

    const char *content = env->GetStringUTFChars(_content, JNI_FALSE);
    int contentLen = env->GetStringLength(_content);
    const char *key = env->GetStringUTFChars(_key, JNI_FALSE);
    int keyLen = env->GetStringLength(_key);

    int encLen = contentLen + AES_BLOCK_SIZE + 1;
    unsigned char *dstData = (unsigned char *) malloc(encLen);

    if (cipher::aes_encrypt((char *) content, (unsigned char *) key, keyLen,
                            dstData) <= 0) {
        free(dstData);
        env->ReleaseStringUTFChars(_content, content);
        env->ReleaseStringUTFChars(_key, key);
        return NULL;
    }
    dstData[encLen] = 0;
    char *encB64str = cipher::Base64encode(dstData, encLen, NULL, false);
    jstring jStr = env->NewStringUTF((const char *) encB64str);
    // 释放资源
    free(dstData);
    env->ReleaseStringUTFChars(_content, content);
    env->ReleaseStringUTFChars(_key, key);

    return jStr;
}


extern "C"
JNIEXPORT jstring JNICALL
Java_com_dds_openssl_OpenCipher_AesDecode(JNIEnv *env, jobject thiz,
                                          jstring _content,
                                          jstring _key) {
    const char *content = env->GetStringUTFChars(_content, JNI_FALSE);
    int contentLen = env->GetStringLength(_content);
    const char *key = env->GetStringUTFChars(_key, JNI_FALSE);
    int keyLen = env->GetStringLength(_key);

    int encLen = 0;
    char *pEncData = (char *) malloc(contentLen);
    // base64解密
    cipher::Base64decode(content, pEncData, &encLen, false);

    int dataLen = encLen + 1;
    char *pData = (char *) malloc(dataLen);
    // ase 解密
    if (cipher::aes_decrypt((char *) content, (unsigned char *) key, keyLen, pData) <= 0) {
        free(pEncData);
        free(pData);
        env->ReleaseStringUTFChars(_content, content);
        env->ReleaseStringUTFChars(_key, key);
        return NULL;
    }

    pData[dataLen] = 0;

    jstring jStr = env->NewStringUTF((const char *) (pData));
    free(pEncData);
    free(pData);
    env->ReleaseStringUTFChars(_content, content);
    env->ReleaseStringUTFChars(_key, key);
    return jStr;
}


