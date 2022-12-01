//
// Created by Dylan on 11/15/2022.
//

#ifndef MCPROXY_POSITION_H
#define MCPROXY_POSITION_H

#include <vector>
#include <span>
#include <cstdio>
#include <network/ByteManipulation.h>

struct Position {
    double x;
    double y;
    double z;
    bool onGround;
    std::vector<unsigned char> bytes();
    static Position parse(std::vector<unsigned char> byteArr);
    bool operator==(const Position & rhs) const;
    bool operator!=(const Position & rhs) const { return !operator==(rhs); }
};

#endif //MCPROXY_POSITION_H
