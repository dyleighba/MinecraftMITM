//
// Created by Dylan on 11/28/2022.
//
#include "catch2/generators/catch_generators.hpp"
#include <catch2/catch_test_macros.hpp>
#include <type/Position.h>
#include <network/Packet.h>

bool isAproxEqual(double d1, double d2) {
    return abs(d1 - d2) < 0.1;
}

bool testPosition(Position testPos) {
    std::vector<unsigned char> generatedBytes = testPos.bytes();
    if (generatedBytes.size() != 25) {
        printf("Incorrect packet size: %zu", generatedBytes.size());
        return false;
    }
    Position readbackPos = Position::parse(generatedBytes);
    printf("in: (%.2f, %.2f, %.2f, %s),  out: (%.2f, %.2f, %.2f, %s)",
           testPos.x, testPos.y, testPos.z, testPos.onGround ? "True" : "False",
           readbackPos.x, readbackPos.y, readbackPos.z, readbackPos.onGround ? "True" : "False");
    if (!isAproxEqual(testPos.x, readbackPos.x)) return false;
    if (!isAproxEqual(testPos.y, readbackPos.y)) return false;
    if (!isAproxEqual(testPos.z, readbackPos.z)) return false;
    return testPos.onGround == readbackPos.onGround;
}
TEST_CASE( "Position packet creation") {
    double x = GENERATE(-63497.4, 2.3, -53.95, 2466);
    double y = GENERATE(-20.5, 87, 65.5, -40);
    double z = GENERATE(-4968.4, 853.1, -7.1, 5.3);
    bool onGround = GENERATE(true, false);
    REQUIRE(testPosition(Position {
        x, y, z, onGround
    }));
}