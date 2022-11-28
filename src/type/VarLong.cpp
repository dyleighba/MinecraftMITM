//
// Created by Dylan on 11/15/2022.
//

#include "VarLong.h"

std::vector<unsigned char> VarLong::toBytes(int value) {
    return VarNumber::toBytes(value, VarLong::maxBits);
}

int VarLong::fromBytes(std::vector<unsigned char> byteArr) {
    if (byteArr.empty()) throw std::out_of_range("VarLong::fromBytes received an empty array");
    return VarNumber::fromBytes(byteArr, VarLong::maxBits);
}