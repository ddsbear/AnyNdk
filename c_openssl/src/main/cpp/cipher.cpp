//
// Created by dds on 2019/10/24.
//

#include <openssl/aes.h>
#include <cstring>
#include <openssl/ossl_typ.h>
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/buffer.h>
#include "cipher.h"
#include "macro.h"


int cipher::aes_encrypt(unsigned char *in,unsigned char *key, int keyLen,unsigned char *out) {
    AES_KEY aes;
    if (!in || !key || !out) return 0;
    // 检查key长度
    if (keyLen != 16 && keyLen != 24 && keyLen != 32) {
        LOGE("aes_encrypt key length is invalid");
        return -1;
    }
    // 设置key
    if (AES_set_encrypt_key(key, keyLen << 3, &aes) < 0) { // keyLen*8
        LOGE("aes_encrypt AES_set_encrypt_key error");
        return -2;
    }
    AES_ecb_encrypt(in, out, &aes, AES_ENCRYPT);
    return 1;
}

int cipher::aes_decrypt(unsigned char *in, unsigned char *key, int keyLen, unsigned char *out) {
    AES_KEY aes;
    if (!in || !key || !out) return 0;

    // 检查key长度
    if (keyLen != 16 && keyLen != 24 && keyLen != 32) {
        LOGE("aes_decrypt key length is invalid");
        return -1;
    }
    // 设置key
    if (AES_set_decrypt_key(key, keyLen << 3, &aes) != 0) { // keyLen*8
        LOGE("aes_decrypt AES_set_decrypt_key error");
        return -2;
    }

    AES_ecb_encrypt(in, out, &aes, AES_DECRYPT);
    return 1;
}

char *cipher::Base64encode(const void *pin, int inLen, char *pout, char bNewLine) {
    BIO *bMem, *b64;
    BUF_MEM *bptr;

    bMem = BIO_new(BIO_s_mem());
    b64 = BIO_new(BIO_f_base64());
    if (!bNewLine) {
        BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL); //one line file
    }
    b64 = BIO_push(b64, bMem);

    BIO_write(b64, pin, inLen);
    BIO_flush(b64);
    BIO_get_mem_ptr(b64, &bptr);

    if (pout == NULL) {
        pout = (char *) malloc(bptr->length + 1);
    }
    memcpy(pout, bptr->data, bptr->length);
    pout[bptr->length] = 0;

    BIO_free_all(b64);
    return pout;


}

void cipher::Base64decode(const char *pin, void *pout, int *poutLen, char bNewLine) {
    int inLen = strlen(pin);
    BIO *b64 = BIO_new(BIO_f_base64());
    if (!bNewLine) {
        BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL); //one line file
    }
    BIO *bMem = BIO_new_mem_buf((void *) pin, inLen);
    bMem = BIO_push(b64, bMem);

    int readBytes = 0;
    char msg[1024] = {0};

    if (poutLen)
        *poutLen = 0;
    while ((readBytes = BIO_read(bMem, msg, sizeof(msg))) > 0) {
        memcpy(pout, msg, readBytes);
        pout = (char *) pout + readBytes;
        if (poutLen)
            *poutLen += readBytes;
        memset(msg, 0, sizeof(msg));
    }
    BIO_free_all(bMem);

}




