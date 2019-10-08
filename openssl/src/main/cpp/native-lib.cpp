#include <jni.h>
#include <string>
#include <openssl/aes.h>

extern "C" JNIEXPORT jstring JNICALL
Java_com_dds_gmssl_GmCipher_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";

    AES_KEY aes_key;
    AES_set_encrypt_key(NULL,0,&aes_key);
    return env->NewStringUTF(hello.c_str());
}
