//
// Created by Dylan on 11/15/2022.
//

#include "VarInt.h"

VarInt VarInt::fromBytes(const std::vector<unsigned char>& byteArr) {
    if (byteArr.empty()) throw std::out_of_range("VarInt::fromBytes received an empty array");
    return VarInt{VarNumber::fromBytes(byteArr, VarInt::maxBits)};
}

std::vector<unsigned char> VarInt::bytes() const {
    return VarNumber::toBytes(value, VarInt::maxBits);
}
