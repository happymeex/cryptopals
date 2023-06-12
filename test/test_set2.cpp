#define CATCH_CONFIG_MAIN
#include "../src/bseq.hpp"
#include "catch.hpp"
#include <fstream>

TEST_CASE("ok") {
    std::ifstream in{"test/input/s2c1.txt"};
    std::string s_in;
    getline(in, s_in);
    ByteSeq bs{s_in};
    CHECK(bs.pkcsPad(20).toString() == "YELLOW SUBMARINE\x04\x04\x04\x04");
}