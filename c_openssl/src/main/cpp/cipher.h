//
// Created by dds on 2019/10/24.
//

#ifndef ANYNDK_CIPHER_H
#define ANYNDK_CIPHER_H

class cipher {
private:

public:
    static int aes_encrypt(unsigned char *in, unsigned char *key, int ketLen, unsigned char *out);

    static int aes_decrypt(unsigned char *in, unsigned char *key, int keyLen, unsigned char *out);

    static char *Base64encode(const void *pin, int inlen, char *pout, char bNewLine = 1);

    static void Base64decode(const char *pin, void *pout, int *poutLen, char bNewLine = 1);

    static void rsa_genkey();

    static void rsa_encrypt();

    static void rsa_decrypt();
};


#endif //ANYNDK_CIPHER_H
