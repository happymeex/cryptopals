#include "catch.hpp"
#include "../src/aes.h"

TEST_CASE("inverseMixColumns", "[inverseMixColumns]"){
    std::vector<int> in = {
        142, 77, 161, 188,
        159, 220, 88, 157,
        1, 1, 1, 1,
        213, 213, 215, 214
    };
    std::vector<int> out = inverseMixColumns(in);
    std::vector<int> expected = {
        219, 19, 83, 69,
        242, 10, 34, 92,
        1, 1, 1, 1,
        212, 212, 212, 213
    };
    CHECK(out == expected);
}
