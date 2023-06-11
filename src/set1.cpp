#include "set1.hpp"
#include "aes.hpp"
#include "bseq.hpp"
#include "utils.hpp"
#include <algorithm>
#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <math.h>
#include <numbers>
#include <string>
#include <unordered_map>
#include <unordered_set>

std::tuple<ByteSeq, double, char> single_byte_xor_cipher(ByteSeq bs) {
    if (bs.length() == 0)
        throw "single byte xor called on empty byte sequence";
    int len = bs.length();
    double highestScore = 0;
    ByteSeq best{""};
    int bestKey = -1;
    for (int i = 0; i < 128; i++) {
        ByteSeq key = ByteSeq{std::string(len, i)};
        ByteSeq decrypted = bs ^ key;
        double stringScore = score(decrypted.toString());
        if (stringScore > highestScore) {
            best = decrypted;
            highestScore = stringScore;
            bestKey = i;
        }
    }
    return std::tuple<ByteSeq, double, char>{best, highestScore, bestKey};
}

std::tuple<ByteSeq, double, char>
detect_single_byte_xor(const std::vector<ByteSeq> &v) {
    double highestScore = 0;
    ByteSeq best{""};
    int bestKey = -1;
    for (ByteSeq bs : v) {
        auto [decrypted, stringScore, key] = single_byte_xor_cipher(bs);
        if (stringScore > highestScore) {
            highestScore = stringScore;
            best = decrypted;
            bestKey = key;
        }
    }
    return std::tuple<ByteSeq, double, char>{best, highestScore, bestKey};
}
ByteSeq repeating_key_xor(ByteSeq bs, ByteSeq key) {
    while (key.length() < bs.length()) {
        key = key + key;
    }
    key = key.subseq(0, bs.length());
    return bs ^ key;
}
//
//  hex repeating_key_xor(std::string cipher, std::string keyString) {
//      return repeating_key_xor(toHex(cipher), toHex(keyString));
//  }
//
//  hex break_repeating_xor(const hex hx) {
//      // start by guessing the keysize: for each guessed size,
//      int upperBound = std::min(40, (int)hx.raw.length() / 4);
//      std::vector<std::pair<int, double>> keySize{{2, 8}, {3, 8}};
//      const int numSamples = 50;
//      for (int guess = 2; guess < upperBound; guess++) {
//          std::vector<hex> samples;
//          for (int j = 0; j < numSamples && 2 * j * guess < hx.raw.length();
//               j++) {
//              samples.push_back(toHex(hx.raw.substr(2 * j * guess, 2 *
//              guess)));
//          }
//          double totalHamming = 0;
//          for (int i = 0; i < samples.size(); i++) {
//              for (int j = i + 1; j < samples.size(); j++) {
//                  totalHamming += hammingDistance(samples[i], samples[j]);
//              }
//          }
//          double avgHamming =
//              totalHamming / (guess * numSamples * (numSamples - 1) / 2.0);
//          for (int i = 0; i < keySize.size(); i++) {
//              if (avgHamming < keySize[i].second) {
//                  keySize[i] = {guess, avgHamming};
//                  break;
//              }
//          }
//      }
//
//      // std::cout << "keysize guess: " << keySize[0].first << " " <<
//      // keySize[1].first << std::endl;
//      std::vector<hex> ret;
//      for (auto [size, _] : keySize) {
//          std::vector<hex> block;
//          for (int i = 0; i < size; i++)
//              block.push_back(hex{""});
//          int len = hx.raw.length();
//          for (int i = 0; i < len; i += 2) {
//              block[(i / 2) % size].raw += hx.raw.substr(i, 2);
//          }
//          std::string key = "";
//          for (const auto &hxi : block) {
//              auto [_, __, blockKey] = single_byte_xor_cipher(hxi);
//              key += blockKey;
//          }
//          ret.push_back(repeating_key_xor(hx, toHex(key)));
//      }
//      // std::cout << "Guesses: " << ret[0].toString() << std::endl <<
//      // ret[1].toString() << std::endl;
//      return ret[0];
//  }
//
//  std::vector<hex> detect_aes_ecb(const std::vector<hex> &in) {
//      std::vector<hex> ret;
//      for (hex hx : in) {
//          int len = hx.raw.length();
//          if (len % 32 != 0)
//              throw "improperly padded hex value";
//          std::unordered_set<std::string> blocks;
//          for (int i = 0; i < len; i += 32) {
//              std::string block = hx.raw.substr(i, 32);
//              if (blocks.contains(block)) {
//                  ret.push_back(hx);
//                  break;
//              }
//              blocks.insert(block);
//          }
//      }
//      return ret;
//  }

// int main(int argc, char **argv) {
//     makeCharFreq();
//     if (argc < 2) {
//         std::cout << "expected a test name" << std::endl;
//         return 0;
//     }
//     std::string test(argv[1]);
//     if (test == "hex_to_64") {
//         if (argc != 3)
//             std::cout << "expected 1 argument" << std::endl;
//         else
//             std::cout << hexTo64(hex{argv[2]}) << std::endl;
//     }
//
//     else if (test == "fixed_xor") {
//         if (argc != 4)
//             std::cout << "expected 2 arguments" << std::endl;
//         else
//             std::cout << hexor(hex{argv[2]}, hex{argv[3]}).raw << std::endl;
//     }
//
//     else if (test == "single_byte_xor_cipher") {
//         if (argc != 3)
//             std::cout << "expected 1 argument" << std::endl;
//         auto [decrypted, stringScore, key] =
//             single_byte_xor_cipher(hex{argv[2]});
//         std::cout << "key: " << key << std::endl
//                   << decrypted.toString() << std::endl;
//     }
//
//     else if (test == "detect_single_byte_xor") {
//         if (argc != 2)
//             std::cout << "expected no arguments" << std::endl;
//         else {
//             std::ifstream file{"test/data/s1c4.txt"};
//             std::vector<hex> v;
//             for (std::string s; file >> s;) {
//                 // std::cout << s << std::endl;
//                 hex input = hex{s};
//                 v.push_back(hex{s});
//             }
//             auto [decrypted, stringScore, key] = detect_single_byte_xor(v);
//             std::cout << "key: " << key << std::endl
//                       << decrypted.toString() << std::endl;
//             file.close();
//         }
//     }
//
//     else if (test == "repeating_key_xor") {
//         if (argc != 4)
//             std::cout << "expected 2 arguments" << std::endl;
//         else {
//             std::cout << repeating_key_xor(argv[2], argv[3]).raw <<
//             std::endl;
//         }
//     }
//
//     else if (test == "hamming") {
//         if (argc != 4)
//             std::cout << "expected 2 arguments" << std::endl;
//         else {
//             std::cout << hammingDistance(argv[2], argv[3]) << std::endl;
//         }
//     }
//
//     else if (test == "break_repeating_key_xor") {
//         std::ifstream file{"test/data/s1c6.txt"};
//         std::string encrypted = "";
//         for (std::string s; file >> s;) {
//             encrypted += s;
//         }
//         hex decrypted = break_repeating_xor(b64ToHex(encrypted));
//         std::cout << decrypted.toString() << std::endl;
//     }
//
//     else if (test == "aes") {
//         try {
//             std::ifstream file{"test/data/s1c7.txt"};
//             std::string encrypted = "";
//             for (std::string s; file >> s;) {
//                 encrypted += s;
//             }
//             hex cipher = b64ToHex(encrypted);
//             hex ret = aes128_ecb_decrypt(cipher, "YELLOW SUBMARINE");
//             std::cout << ret.toString() << std::endl;
//         } catch (const char *message) {
//             std::cout << message << std::endl;
//         }
//
//     }
//
//     else if (test == "detect_aes") {
//         try {
//             std::ifstream file{"test/data/s1c8.txt"};
//             std::vector<hex> in;
//             for (std::string s; file >> s;) {
//                 in.push_back(hex{s});
//             }
//             std::vector<hex> out = detect_aes_ecb(in);
//             for (const auto &hx : out) {
//                 std::cout << hx.raw << std::endl;
//             }
//         } catch (const char *message) {
//             std::cout << message << std::endl;
//         }
//     }
// }
