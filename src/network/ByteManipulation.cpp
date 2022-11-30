//
// Created by Dylan on 11/30/2022.
//

#include "ByteManipulation.h"

double swapEndian(double value) {
    auto valueBytes = reinterpret_cast<unsigned char *>(&value);
    std::span valueBytesSpan(valueBytes, sizeof(double));
    std::reverse(valueBytesSpan.begin(), valueBytesSpan.end());
    return *reinterpret_cast<double *>(valueBytesSpan.data());
}
