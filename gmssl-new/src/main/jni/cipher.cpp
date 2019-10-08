//
// Created by dds on 2019/10/8.
//
#include <string>
#include <openssl/ossl_typ.h>
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/buffer.h>
#include "cipher.h"

char *cipher::base64Encode(const void *pin, int inLen, char *pout, char bNewLine) {
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

void cipher::base64Decode(const char *pin, void *pout, int *poutLen, char bNewLine) {
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
