//
// Created by Dylan on 11/26/2022.
//

#ifndef MCPROXY_POSITIONANDROTATION_H
#define MCPROXY_POSITIONANDROTATION_H

#include <vector>
#include <algorithm>
#include "Position.h"

struct PositionAndRotation {
    double x;
    double y;
    double z;
    float yaw;
    float pitch;
    bool onGround;
    std::vector<unsigned char> bytes();
    static PositionAndRotation parse(std::vector<unsigned char> byteArr);
    Position position();
    bool operator==(const PositionAndRotation & rhs) const;
    bool operator!=(const PositionAndRotation & rhs) const { return !operator==(rhs); }
};

#endif //MCPROXY_POSITIONANDROTATION_H
