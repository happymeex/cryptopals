#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "../src/Hex.h"

TEST_CASE("intToHex", "[intToHex]"){
    CHECK(intToHex(0, 1).raw == "0");
    CHECK(intToHex(26, 5).raw == "0001a");
    CHECK_THROWS(intToHex(124932, 3));
}

TEST_CASE("b64ToHex", "[b64ToHex]"){
    CHECK(b64ToHex("").raw == "");
    CHECK(b64ToHex("TWE=").raw == "4d61");
    CHECK(b64ToHex("TQ==").raw == "4d");
    CHECK_THROWS(b64ToHex("sjd"), "Should throw on unpadded input");
}

