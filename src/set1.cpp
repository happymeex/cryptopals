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

ByteSeq break_repeating_xor(const ByteSeq bs) {
    // start by guessing the keysize: for each guessed size,
    int upperBound = std::min(40, bs.length() / 2);
    std::vector<std::pair<int, double>> keySize{{2, 8}, {3, 8}};
    const int numSamples = 50;
    for (int guess = 2; guess < upperBound; guess++) {
        std::vector<ByteSeq> samples;
        for (int j = 0; j < numSamples && j * guess < bs.length(); j++) {
            samples.push_back(bs.subseq(j * guess, (j + 1) * guess));
        }
        double totalHamming = 0;
        for (int i = 0; i < samples.size(); i++) {
            for (int j = i + 1; j < samples.size(); j++) {
                totalHamming += hammingDistance(samples[i], samples[j]);
            }
        }
        double avgHamming =
            totalHamming / (guess * numSamples * (numSamples - 1) / 2.0);
        for (int i = 0; i < keySize.size(); i++) {
            if (avgHamming < keySize[i].second) {
                keySize[i] = {guess, avgHamming};
                break;
            }
        }
    }

    // std::cout << "keysize guess: " << keySize[0].first << " "
    //           << keySize[1].first << std::endl;
    std::vector<ByteSeq> ret;
    for (auto [size, _] : keySize) {
        std::vector<ByteSeq> block;
        for (int i = 0; i < size; i++)
            block.push_back(ByteSeq{""});
        int len = bs.length();
        for (int i = 0; i < len; i++) {
            block[i % size] = block[i % size] + bs.subseq(i, i + 1);
        }
        std::string key = "";
        for (const auto &bsi : block) {
            auto [_, __, blockKey] = single_byte_xor_cipher(bsi);
            key += blockKey;
        }
        ret.push_back(repeating_key_xor(bs, ByteSeq{key}));
    }
    // std::cout << "Guesses: " << ret[0].toString() << std::endl
    //           << ret[1].toString() << std::endl;
    return ret[0];
}
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
