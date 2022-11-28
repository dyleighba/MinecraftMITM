//
// Created by Dylan on 11/13/2022.
//

#include "VarNumber.h"

std::vector<unsigned char> VarNumber::toBytes(int value, int maxBits) {
    std::vector<unsigned char> byteArr;
    while (true) {
        if ((value & ~SEGMENT_BITS) == 0) {
            byteArr.push_back(value);
            break;
        }

        byteArr.push_back((value & SEGMENT_BITS) | CONTINUE_BIT);

        // Note: >>> means that the sign bit is shifted with the rest of the number rather than being left alone
        value >>= 7;
    }
    return byteArr;
}

int VarNumber::fromBytes(std::vector<unsigned char> byteArr, int maxBits) {
    if (maxBits <= 0) throw std::runtime_error("VarNumber is not to be used directly");
    int value = 0;
    int position = 0;
    unsigned char currentByte;
    int currentByteIndex = 0;

    while (true) {
        currentByte = byteArr.at(currentByteIndex);
        value |= (currentByte & SEGMENT_BITS) << position;

        if ((currentByte & CONTINUE_BIT) == 0) break;

        position += 7;
        if (position >= maxBits) throw std::runtime_error("VarInt is too big");
        currentByteIndex++;
    }

    return value;
}
