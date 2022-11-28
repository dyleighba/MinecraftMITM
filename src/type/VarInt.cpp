//
// Created by Dylan on 11/15/2022.
//

#include "VarInt.h"

std::vector<unsigned char> VarInt::toBytes(int value) {
    return VarNumber::toBytes(value, VarInt::maxBits);
}

int VarInt::fromBytes(std::vector<unsigned char> byteArr) {
    if (byteArr.empty()) throw std::out_of_range("VarInt::fromBytes received an empty array");
    return VarNumber::fromBytes(byteArr, VarInt::maxBits);
}