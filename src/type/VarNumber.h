//
// Created by Dylan on 11/13/2022.
//

#ifndef MCPROXY_VARNUMBER_H
#define MCPROXY_VARNUMBER_H

#include <stdexcept>
#include <vector>

struct VarNumber {
protected:
    const static int SEGMENT_BITS = 0x7F;
    const static int CONTINUE_BIT = 0x80;
public:
    static std::vector<unsigned char> toBytes(int value, int maxBits);
    static int fromBytes(std::vector<unsigned char> byteArr, int maxBits);
};


#endif //MCPROXY_VARNUMBER_H
