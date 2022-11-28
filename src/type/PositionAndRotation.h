//
// Created by Dylan on 11/26/2022.
//

#ifndef MCPROXY_POSITIONANDROTATION_H
#define MCPROXY_POSITIONANDROTATION_H

struct PositionAndRotation {
    double x;
    double y;
    double z;
    float yaw;
    float pitch;
    bool onGround;
};

#endif //MCPROXY_POSITIONANDROTATION_H
