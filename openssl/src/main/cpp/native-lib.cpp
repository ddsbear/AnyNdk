#include <jni.h>
#include <string>
#include <openssl/aes.h>
#include "cipher.h"

#define LEN 512

extern "C"
JNIEXPORT jstring JNICALL
Java_com_dds_openssl_GmCipher_AesEncode(JNIEnv *env, jobject thiz, jstring _content, jstring _key) {

    const char *content = env->GetStringUTFChars(_content, JNI_FALSE);
    const char *key = env->GetStringUTFChars(_key, JNI_FALSE);
    char source[LEN];
    char dst[LEN];
    memset((char *) source, 0, LEN);
    memset((char *) dst, 0, LEN);

    strcpy(source, content);

    if (!cipher::aes_encrypt(source, (char *) (key), dst))//(in,key,out)//加密
    {
        printf("encrypt error\n");
    }


    env->ReleaseStringUTFChars(_content, content);
    env->ReleaseStringUTFChars(_key, key);


    return NULL;


}


extern "C"
JNIEXPORT jstring JNICALL
Java_com_dds_openssl_GmCipher_AesDecode(JNIEnv *env, jobject thiz, jstring content, jstring key) {


    return NULL;
}


