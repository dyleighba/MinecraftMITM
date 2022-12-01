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

Position PositionAndRotation::position() {
    return Position{this->x, this->y, this->z, this->onGround};
}

bool PositionAndRotation::operator==(const PositionAndRotation &rhs) const {
    if (std::abs(this->x - rhs.x) > 0.1) return false;
    if (std::abs(this->x - rhs.x) > 0.1) return false;
    if (std::abs(this->x - rhs.x) > 0.1) return false;
    if (std::abs(this->yaw - rhs.yaw) > 0.1) return false;
    if (std::abs(this->pitch - rhs.pitch) > 0.1) return false;
    if (this->onGround != rhs.onGround) return false;
    return true;
}
