#include <jni.h>
#include <string>
#include <openssl/aes.h>
#include "cipher.h"
#include "macro.h"
#include "utils.h"


extern "C"
JNIEXPORT jstring JNICALL
Java_com_dds_openssl_OpenCipher_AesEncode(JNIEnv *env, jobject thiz,
                                          jstring _content,
                                          jstring _key) {
    LOGD("------------encrypt----------------");
    const char *content = env->GetStringUTFChars(_content, JNI_FALSE);
    int contentLen = env->GetStringLength(_content);
    const char *key = env->GetStringUTFChars(_key, JNI_FALSE);
    int keyLen = env->GetStringLength(_key);
    LOGD("content:%s", content);
    LOGD("key:%s", key);
    int encLen = contentLen + AES_BLOCK_SIZE - contentLen % AES_BLOCK_SIZE;

    unsigned char *srcData = (unsigned char *) malloc(encLen);
    memset(srcData, 0, encLen);
    memcpy(srcData, content, strlen(content));
    utils::Padding(srcData, contentLen, encLen);
    LOGD("encLength:%d", encLen);

    unsigned char *dstData = (unsigned char *) malloc(encLen);
    memset(dstData, 0, encLen);

    if (cipher::aes_encrypt(srcData, (unsigned char *) key, keyLen, dstData) <= 0) {
        free(srcData);
        free(dstData);
        env->ReleaseStringUTFChars(_content, content);
        env->ReleaseStringUTFChars(_key, key);
        return NULL;
    }
    LOGD("enc aes hex :%s", utils::char2HexStr(dstData, encLen).c_str());
    char *encB64str = cipher::Base64encode(dstData, encLen, NULL, false);
    LOGD("enc base64:%s", encB64str);

    jstring jStr = env->NewStringUTF((const char *) encB64str);
    // 释放资源
    free(srcData);
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
    LOGD("------------encrypt----------------");
    const char *content = env->GetStringUTFChars(_content, JNI_FALSE);
    int contentLen = env->GetStringLength(_content);
    const char *key = env->GetStringUTFChars(_key, JNI_FALSE);
    int keyLen = env->GetStringLength(_key);
    LOGD("dec content:%s", content);
    LOGD("dec key:%s", key);
    int encLen = 0;
    unsigned char *pEncData = (unsigned char *) malloc(contentLen);
    // base64解密
    cipher::Base64decode(content, pEncData, &encLen, false);
    LOGD("dec base64 hex :%s", utils::char2HexStr(pEncData, encLen).c_str());
    int dataLen = encLen + 1;
    unsigned char *pData = (unsigned char *) malloc(dataLen);
    memset(pData, 0, dataLen);
    // ase 解密
    if (cipher::aes_decrypt(pEncData, (unsigned char *) key, keyLen, pData) <= 0) {
        free(pEncData);
        free(pData);
        env->ReleaseStringUTFChars(_content, content);
        env->ReleaseStringUTFChars(_key, key);
        return NULL;
    }
    pData[dataLen] = 0;
    LOGD("dec aes:%s", pData);
    jstring jStr = env->NewStringUTF((const char *) (pData));
    free(pEncData);
    free(pData);
    env->ReleaseStringUTFChars(_content, content);
    env->ReleaseStringUTFChars(_key, key);
    return jStr;
}


