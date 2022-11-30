//
// Created by Dylan on 11/26/2022.
//

#ifndef MCPROXY_POSITIONANDROTATION_H
#define MCPROXY_POSITIONANDROTATION_H

#include <vector>
#include <algorithm>

struct PositionAndRotation {
    double x;
    double y;
    double z;
    float yaw;
    float pitch;
    bool onGround;
    std::vector<unsigned char> bytes();
    static PositionAndRotation parse(std::vector<unsigned char> byteArr);
};

#endif //MCPROXY_POSITIONANDROTATION_H
