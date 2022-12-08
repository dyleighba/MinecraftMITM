//
// Created by Dylan on 11/15/2022.
//

#include "VarLong.h"

VarLong VarLong::fromBytes(const std::vector<unsigned char>& byteArr) {
    if (byteArr.empty()) throw std::out_of_range("VarLong::fromBytes received an empty array");
    return VarLong{VarNumber::fromBytes(byteArr, VarLong::maxBits)};
}

std::vector<unsigned char> VarLong::bytes() const {
    return VarNumber::toBytes(value, VarLong::maxBits);
}