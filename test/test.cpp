#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "../src/Hex.h"

TEST_CASE("intToHex", "[intToHex]"){
    CHECK(intToHex(0, 1).raw == "0");
    CHECK(intToHex(0, 5).raw == "00000");
}

