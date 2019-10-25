//
// Created by dds on 2019/10/24.
//

#ifndef ANYNDK_CIPHER_H
#define ANYNDK_CIPHER_H

class cipher {
private:

public:
    static int aes_encrypt(char *in, unsigned char *key, int ketLen, unsigned char *out);

    static int aes_decrypt(char *in, unsigned char *key, int keyLen, char *out);

    static char *Base64encode(const void *pin, int inlen, char *pout, char bNewLine = 1);

    static void Base64decode(const char *pin, void *pout, int *poutLen, char bNewLine = 1);
};


#endif //ANYNDK_CIPHER_H
