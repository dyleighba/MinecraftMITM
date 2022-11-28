//
// Created by Dylan on 11/13/2022.
//

#ifndef MCPROXY_VARLONG_H
#define MCPROXY_VARLONG_H

#include <stdexcept>
#include <vector>
#include "VarNumber.h"

class VarLong {
private:
    const static int maxBits = 64;
public:
    static std::vector<unsigned char> toBytes(int value);
    static int fromBytes(std::vector<unsigned char> byteArr);
};


#endif //MCPROXY_VARLONG_H
