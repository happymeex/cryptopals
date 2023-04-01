#define CATCH_CONFIG_MAIN
#include "../src/hex.hpp"
#include "../src/set1.hpp"
#include "catch.hpp"
#include <fstream>

TEST_CASE("Set 1 challenge 1: hex to b64") {
    std::ifstream in{"test/input/s1c1.txt"};
    std::ifstream expected{"test/expected/s1c1.txt"};
    std::string s_in, s_out;
    in >> s_in;
    expected >> s_out;
    CHECK(hexTo64(hex{s_in}) == s_out);
}

TEST_CASE("Set 1 challenge 2: hex xor") {
    std::ifstream in{"test/input/s1c2.txt"};
    std::ifstream expected{"test/expected/s1c2.txt"};
    std::string s1, s2, s_out;
    in >> s1 >> s2;
    expected >> s_out;
    CHECK(hexor(hex{s1}, hex{s2}).raw == s_out);
}

TEST_CASE("Set 1 challenge 3: ") {
    std::ifstream in{"test/input/s1c3.txt"};
    std::ifstream expected{"test/expected/s1c3.txt"};
    std::string s_in, s_out;
    char expected_key;
    in >> s_in;
    expected >> expected_key;
    expected.ignore();
    std::getline(expected, s_out);
    auto [decrypted, _, key] = single_byte_xor_cipher(hex{s_in});
    CHECK(key == expected_key);
    CHECK(decrypted.toString() == s_out);
}