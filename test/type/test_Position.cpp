//
// Created by Dylan on 12/1/2022.
//
#include "catch2/generators/catch_generators.hpp"
#include <catch2/catch_test_macros.hpp>
#include <type/Position.h>
#include <network/Packet.h>

bool testPositionClass(Position testPos) {
    std::vector<unsigned char> generatedBytes = testPos.bytes();
    if (generatedBytes.size() != 25) {
        printf("Incorrect packet size: %zu", generatedBytes.size());
        return false;
    }
    Position readbackPos = Position::parse(generatedBytes);
    return testPos == readbackPos;
}
TEST_CASE( "Position packet creation") {
    double x = GENERATE(-63497.4, 2.3, -53.95, 2466);
    double y = GENERATE(-20.5, 87, 65.5, -40);
    double z = GENERATE(-4968.4, 853.1, -7.1, 5.3);
    bool onGround = GENERATE(true, false);
    REQUIRE(testPositionClass(Position {
            x, y, z, onGround
    }));
}