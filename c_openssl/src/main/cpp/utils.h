//
// Created by dds on 2019/10/28.
//

#ifndef ANYNDK_UTILS_H
#define ANYNDK_UTILS_H

#include <sstream>

class utils {
public:
    static std::string char2HexStr(unsigned char *bytes, int byteLength);

    static void Padding(unsigned char *pSrc, int nSrcLen, int kCodeLen);

};


#endif //ANYNDK_UTILS_H
