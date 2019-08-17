#include <cstring>
#include <climits>
#include <openssl/rand.h>
#include <openssl/crypto.h>
#include "Cipher.h"

Cipher::Cipher() {

}

void Cipher::generateRandom(int outLen, unsigned char *buf) {

    if (outLen <= 0 || outLen >= INT_MAX) {
        return;
    }
    OPENSSL_malloc(outLen);
    if (!RAND_bytes(buf, outLen)) {
        return;
    }
}


