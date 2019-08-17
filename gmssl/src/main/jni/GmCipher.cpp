#include <jni.h>
#include <cstring>
#include <climits>
#include "openssl/crypto.h"
#include "openssl/rand.h"
#include "Cipher.h"


JNIEXPORT void JNICALL
Java_com_dds_gmssl_GmCipher_LogInit(JNIEnv *env, jclass instance, jstring _dir, jint _fileLevel,
                                    jint _screenLevel) {
    const char *dir = env->GetStringUTFChars(_dir, JNI_FALSE);


    env->ReleaseStringUTFChars(_dir, dir);

}

extern "C" JNIEXPORT jbyteArray JNICALL
Java_com_dds_gmssl_GmCipher_generateRandom(JNIEnv *env, jobject instance, jint outLen) {
    Cipher *cipher = new Cipher();
    jbyteArray ret = 0;
    jbyte *outBuf = 0;

    cipher->generateRandom(outLen, (unsigned char *) outBuf);

    if (!outBuf) {
        env->NewByteArray(outLen);
        env->SetByteArrayRegion(ret, 0, outLen, outBuf);
        return ret;
    }

}
