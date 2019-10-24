//
// Created by dds on 2019/10/24.
//

#ifndef ANYNDK_CIPHER_H
#define ANYNDK_CIPHER_H


class cipher {
private:


public:
    static int aes_encrypt(char* in, char* key, char* out);
    static int aes_decrypt(char* in, char* key, char* out);

};


#endif //ANYNDK_CIPHER_H
