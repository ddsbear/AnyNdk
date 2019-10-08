//
// Created by dds on 2019/10/8.
//

#ifndef MOBISHIELDNEW_CIPHER_H
#define MOBISHIELDNEW_CIPHER_H


#include "include/openssl/stack.h"

class cipher {
public:
    static char *base64Encode(const void *pin, int inLen, char *pout, char bNewLine = 1);

    static void base64Decode(const char *pin, void *pout, int *poutLen, char bNewLine = 1);

};


#endif //MOBISHIELDNEW_CIPHER_H
