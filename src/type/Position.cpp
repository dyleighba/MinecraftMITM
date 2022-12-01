//
// Created by Dylan on 11/29/2022.
//

#include "Position.h"

std::vector<unsigned char> Position::bytes() {
    const int arrSize = (sizeof(double)*3) + 1;
    auto doubleArr = static_cast<double *>(malloc(arrSize));
    doubleArr[0] = swapEndian(this->x);
    doubleArr[1] = swapEndian(this->y);
    doubleArr[2] = swapEndian(this->z);
    auto finalArr = reinterpret_cast<unsigned char *>(doubleArr);
    finalArr[arrSize-1] = this->onGround ? 0x01 : 0x00;
    std::vector<unsigned char> posBytes;
    posBytes.reserve(arrSize);
    std::span finalArrSpan(finalArr, arrSize);
    posBytes.insert(posBytes.begin(), finalArrSpan.begin(), finalArrSpan.end());
    free(doubleArr);
    return posBytes;
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

bool Position::operator==(const Position &rhs) const {
    if (std::abs(this->x - rhs.x) > 0.1) return false;
    if (std::abs(this->x - rhs.x) > 0.1) return false;
    if (std::abs(this->x - rhs.x) > 0.1) return false;
    if (this->onGround != rhs.onGround) return false;
    return true;
}
