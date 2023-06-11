#define CATCH_CONFIG_MAIN
#include "../src/aes.hpp"
#include "../src/bseq.hpp"
#include "../src/set1.hpp"
#include "catch.hpp"
#include <fstream>

TEST_CASE("Set 1 challenge 1: hex to b64") {
    std::ifstream in{"test/input/s1c1.txt"};
    std::ifstream expected{"test/expected/s1c1.txt"};
    std::string s_in, s_out;
    in >> s_in;
    expected >> s_out;
    ByteSeq b = ByteSeq{hex{s_in}};
    CHECK(b.toB64().raw == s_out);
}

TEST_CASE("Set 1 challenge 2: hex xor") {
    std::ifstream in{"test/input/s1c2.txt"};
    std::ifstream expected{"test/expected/s1c2.txt"};
    std::string s1, s2, s_out;
    in >> s1 >> s2;
    ByteSeq b1 = ByteSeq{hex{s1}};
    ByteSeq b2 = ByteSeq{hex{s2}};
    expected >> s_out;
    CHECK((b1 ^ b2).toHex().raw == s_out);
}

TEST_CASE("Set 1 challenge 3: single byte xor cipher") {
    std::ifstream in{"test/input/s1c3.txt"};
    std::ifstream expected{"test/expected/s1c3.txt"};
    std::string s_in, s_out;
    char expected_key;
    in >> s_in;
    expected >> expected_key;
    expected.ignore();
    std::getline(expected, s_out);
    auto [decrypted, _, key] = single_byte_xor_cipher(ByteSeq{hex{s_in}});
    CHECK(key == expected_key);
    CHECK(decrypted.toString() == s_out);
}

TEST_CASE("Set 1 challenge 4: detect single byte xor") {
    std::ifstream in{"test/input/s1c4.txt"};
    std::ifstream expected{"test/expected/s1c4.txt"};

    std::vector<ByteSeq> v;
    for (std::string s; in >> s;) {
        ByteSeq input = ByteSeq{hex{s}};
        v.push_back(input);
    }
    auto [decrypted, stringScore, key] = detect_single_byte_xor(v);
    char expected_key;
    std::string s_out;
    expected >> expected_key;
    expected.ignore();
    std::getline(expected, s_out);
    CHECK(key == expected_key);
    CHECK(decrypted.toString() == s_out + "\n");
}
//
// TEST_CASE("Set 1 challenge 5: repeating-key xor") {
//     std::ifstream in{"test/input/s1c5.txt"};
//     std::ifstream expected{"test/expected/s1c5.txt"};
//     std::string input;
//     std::string s_in, s_out;
//     std::string key;
//     getline(in, s_in);
//     input = s_in + "\n";
//     getline(in, s_in);
//     input += s_in;
//     in >> key;
//     expected >> s_out;
//     CHECK(repeating_key_xor(input, key).raw == s_out);
// }
//
// TEST_CASE("Set 1 challenge 6: break repeating-key xor") {
//     std::ifstream in{"test/input/s1c6.txt"};
//     std::ifstream expected{"test/expected/s1c6.txt"};
//     std::string s_in;
//     for (std::string input; in >> input;) {
//         s_in += input;
//     }
//     std::string s_out;
//     for (std::string input; getline(expected, input);) {
//         s_out += input + "\n";
//     }
//     CHECK(break_repeating_xor(b64ToHex(s_in)).toString() == s_out);
// }
//
// TEST_CASE("Set 1 challenge 7: AES-128 ECB mode") {
//     std::ifstream in{"test/input/s1c7.txt"};
//     std::ifstream expected{"test/expected/s1c7.txt"};
//
//     std::string encrypted = "";
//     for (std::string s; in >> s;) {
//         encrypted += s;
//     }
//     std::string s_out;
//     for (std::string input; getline(expected, input);) {
//         s_out += input + "\n";
//     }
//     hex cipher = b64ToHex(encrypted);
//     std::string ret = aes128_ecb_decrypt(cipher, "YELLOW
//     SUBMARINE").toString(); CHECK(ret + "\n" == s_out);
// }
//
// TEST_CASE("Set 1 challenge 8: detect AES ECB mode") {
//     std::ifstream in{"test/input/s1c8.txt"};
//     std::ifstream expected{"test/expected/s1c8.txt"};
//     std::vector<hex> v;
//     for (std::string s; in >> s;) {
//         v.push_back(hex{s});
//     }
//     std::string s_out;
//     expected >> s_out;
//     std::vector<hex> out = detect_aes_ecb(v);
//     CHECK(out.at(0).raw == s_out);
// }