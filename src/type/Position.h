//
// Created by Dylan on 11/15/2022.
//

#ifndef MCPROXY_POSITION_H
#define MCPROXY_POSITION_H

#include <vector>

struct Position {
    double x;
    double y;
    double z;
    bool onGround;
    std::vector<unsigned char> bytes();
    static Position parse(std::vector<unsigned char> byteArr);
};

#endif //MCPROXY_POSITION_H
