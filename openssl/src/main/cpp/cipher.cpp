//
// Created by dds on 2019/10/24.
//

#include <openssl/aes.h>
#include <cstring>
#include "cipher.h"

int cipher::aes_encrypt(char *in, char *key, char *out) {
    AES_KEY aes;
    int len = strlen(in), en_len = 0;
    if (!in || !key || !out) return 0;
    if (AES_set_encrypt_key((unsigned char*)key, 128, &aes) < 0){
        return 0;
    }



    return 0;
}

int cipher::aes_decrypt(char *in, char *key, char *out) {
    return 0;
}
