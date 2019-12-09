//
// Created by dds on 2019/10/28.
//

#include "utils.h"

std::string utils::char2HexStr(unsigned char *bytes, int byteLength) {
    std::string str("");
    std::string str2("0123456789abcdef");
    for (int i = 0; i < byteLength; i++) {
        int b;
        b = 0x0f & (bytes[i] >> 4);
        char s1 = str2.at(b);
        str.append(1, str2.at(b));
        b = 0x0f & bytes[i];
        str.append(1, str2.at(b));
        char s2 = str2.at(b);
    }
    return str;

}

void utils::Padding(unsigned char *pSrc, int nSrcLen, int kCodeLen) {
    if (nSrcLen < kCodeLen) {
        unsigned char ucPad = (unsigned char) (kCodeLen - nSrcLen);
        for (int nID = kCodeLen; nID > nSrcLen; --nID) {
            pSrc[nID - 1] = ucPad;
        }
    }
}
