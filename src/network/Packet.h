//
// Created by Dylan on 11/13/2022.
//

#ifndef MCPROXY_PACKET_H
#define MCPROXY_PACKET_H


#include <vector>

struct Packet {
    int id;
    std::vector<unsigned char> data;
};

#endif //MCPROXY_PACKET_H
