//
// Created by Dylan on 11/13/2022.
//

#ifndef MCPROXY_VARLONG_H
#define MCPROXY_VARLONG_H

#include <stdexcept>
#include <vector>
#include "VarNumber.h"

class VarLong {
private:
    const static int maxBits = 64;
public:
    [[nodiscard]] std::vector<unsigned char> bytes() const;
    static VarLong fromBytes(const std::vector<unsigned char>& byteArr);
    int value;
    operator long() const { return value; };
};

inline VarLong operator+(const VarLong lhs, const VarLong & rhs) { return VarLong{lhs.value+rhs.value}; };
inline VarLong operator+(const VarLong lhs, const int rhs) { return VarLong{lhs.value+rhs}; };
inline long operator+(long lhs, const VarLong & rhs) { return lhs + rhs.value; };

#endif //MCPROXY_VARLONG_H
