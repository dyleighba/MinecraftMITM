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
    static std::vector<unsigned char> toBytes(int value);
    static int fromBytes(std::vector<unsigned char> byteArr);
};


#endif //MCPROXY_VARINT_H
