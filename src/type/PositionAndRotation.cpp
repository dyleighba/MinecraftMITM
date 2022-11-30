//
// Created by Dylan on 11/28/2022.
//

#include "PositionAndRotation.h"

std::vector<unsigned char> PositionAndRotation::bytes() {
    printf("WARNING: PositionAndRotation::bytes() not yet implemented, but was used!");
    return std::vector<unsigned char>{};
}

PositionAndRotation PositionAndRotation::parse(std::vector<unsigned char> byteArr) {
    std::reverse(byteArr.begin(), byteArr.end());
    PositionAndRotation pos {
            *(double*)(byteArr.data()+25),
            *(double*)(byteArr.data()+17),
            *(double*)(byteArr.data()+9),
            *(float*)(byteArr.data()+5),
            *(float*)(byteArr.data()+1),
            *(bool*)(byteArr.data())
    };
    return pos;
}
