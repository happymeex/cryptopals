#define CATCH_CONFIG_MAIN
#include "../src/bseq.hpp"
#include "../src/utils.hpp"
#include "catch.hpp"

TEST_CASE("ByteSeq toB64 method") {
    ByteSeq bs{"no way"};
    CHECK(bs.toB64().raw == "bm8gd2F5");
    bs = ByteSeq{"sadness!"};
    CHECK(bs.toB64().raw == "c2FkbmVzcyE=");
    bs = ByteSeq{"what a waste!"};
    CHECK(bs.toB64().raw == "d2hhdCBhIHdhc3RlIQ==");
}

TEST_CASE("ByteSeq constructor from base 64") {
    b64 in = ByteSeq{"no way"}.toB64();
    CHECK(ByteSeq{in}.toString() == "no way");
    in = ByteSeq{"sadness!"}.toB64();
    CHECK(ByteSeq{in}.toString() == "sadness!");
    in = ByteSeq{"what a waste!"}.toB64();
    CHECK(ByteSeq{in}.toString() == "what a waste!");
}

TEST_CASE("Hamming distance") {
    CHECK_THROWS(hammingDistance(ByteSeq{"one"}, ByteSeq{"three"}));
    CHECK(hammingDistance(ByteSeq{"wokka wokka!!!"},
                          ByteSeq{"this is a test"}) == 37);
}
