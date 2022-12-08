//
// Created by Dylan on 11/13/2022.
//

#ifndef MCPROXY_VARINT_H
#define MCPROXY_VARINT_H

#include <stdexcept>
#include <vector>
#include "VarNumber.h"

struct VarInt {
private:
    const static int maxBits = 32;
public:
    [[nodiscard]] std::vector<unsigned char> bytes() const;
    static VarInt fromBytes(const std::vector<unsigned char>& byteArr);
    int value;
    operator int() const { return value; };
};

inline VarInt operator+(const VarInt lhs, const VarInt & rhs) { return VarInt{lhs.value+rhs.value}; };
inline VarInt operator+(const VarInt lhs, const int rhs) { return VarInt{lhs.value+rhs}; };
inline int operator+(int lhs, const VarInt & rhs) { return lhs + rhs.value; };

#endif //MCPROXY_VARINT_H
