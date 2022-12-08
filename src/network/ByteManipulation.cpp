//
// Created by Dylan on 11/30/2022.
//

#include "ByteManipulation.h"

template<typename T>
T swapEndianT(T value) {
    union {
        T value;
        unsigned char bytes[sizeof(T)];
    } data{value};
    std::reverse(std::begin(data.bytes), std::end(data.bytes));
    return data.value;
}

double swapEndian(double value) {
    return swapEndianT(value);
}

float swapEndian(float value) {
    return swapEndianT(value);
}
