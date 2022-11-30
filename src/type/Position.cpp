//
// Created by Dylan on 11/29/2022.
//

#include "Position.h"

std::vector<unsigned char> Position::bytes() {
    double altY = this->y-0.01;
    unsigned char* pointers[3] {
            (unsigned char*)&this->x,
            (unsigned char*)&altY,
            (unsigned char*)&this->z
    };
    unsigned char rawPacketData[(sizeof(double)*3)+1]{};
    for (int n = 0; n < 3; ++n) {
        for (int i = 0; i < 8; ++i) {
            rawPacketData[(n*8)+i] = pointers[n][i];
            //newPacket.data.push_back(pointers[n][i]);
        }
    }
    rawPacketData[(sizeof(double)*3)] = 0x00;
    //newPacket.data.push_back(0x00);
    std::vector<unsigned char> byteArr(rawPacketData, (rawPacketData)+(sizeof(double)*3)+1);
    return byteArr;
}

 Position Position::parse(std::vector<unsigned char> byteArr) {
    std::reverse(byteArr.begin(), byteArr.end());
    Position pos {
            *(double*)(byteArr.data()+17),
            *(double*)(byteArr.data()+9),
            *(double*)(byteArr.data()+1),
            *(bool*)(byteArr.data())
    };
    return pos;
}
