#include "bseq.hpp"
#include <iostream>

// using namespace byteseq;

ByteSeq::ByteSeq(std::vector<uint8_t> seq) : seq(seq) {}

ByteSeq::ByteSeq(const std::string &s) {
    for (char c : s) {
        this->seq.push_back(c);
    }
}

/**
 * Converts a string represented an unsigned hex value to an integer.
 */
int intFromHexRaw(std::string s) { return std::stoi(s, 0, 16); }

ByteSeq::ByteSeq(const hex &hx) {
    int len = hx.raw.length();
    if (len % 2 != 0)
        throw "hex -> byteseq conversion requires even length";
    for (int i = 0; i < hx.raw.length(); i += 2) {
        this->seq.push_back(intFromHexRaw(hx.raw.substr(i, 2)));
    }
}

/**
 * Converts a single character to the base 64 value it represents.
 */
uint8_t b64ToInt(char c) {
    if (std::isupper(c))
        return c - 'A';
    else if (std::islower(c))
        return c - 'a' + 26;
    else if (std::isdigit(c))
        return c - '0' + 52;
    else if (c == '+')
        return 62;
    else if (c == '/')
        return 63;
    throw "invalid base64";
}

ByteSeq::ByteSeq(const b64 &bsx) {
    int len = bsx.raw.length();
    if (len % 4 != 0)
        throw "base 64 -> byteseq conversion requires length divisible by 4";
    int chunks = len / 4;
    for (int i = 0; i < chunks; i++) {
        int j = 4 * i;
        if (bsx.raw[j] == '=' || bsx.raw[j + 1] == '=')
            throw "unexpected char `=` while parsing base 64";
        uint8_t vals[4];
        for (int t = i; t < i + 4; t++)
            vals[t] = bsx.raw[t] == '=' ? 64 : b64ToInt(bsx.raw[t]);
        this->seq.push_back(vals[0] << 2 | vals[1] >> 4);
        if (vals[2] < 64) {
            this->seq.push_back((vals[1] << 4) | vals[2] >> 2);
            if (vals[3] < 64) {
                this->seq.push_back(vals[3] | vals[2] << 6);
            }
        }
    }
}

ByteSeq ByteSeq::operator^(const ByteSeq &b) const {
    std::vector<uint8_t> ret;
    int len = this->seq.size();
    if (len != b.seq.size())
        throw "Cannot xor byte sequences of differing lengths";
    for (int i = 0; i < len; i++) {
        ret.push_back(this->seq.at(i) ^ b.seq.at(i));
    }
    return ByteSeq{ret};
}

int ByteSeq::length() const { return this->seq.size(); }

std::string ByteSeq::toString() const {
    std::string ret = "";
    for (auto c : this->seq) {
        ret += c;
    }
    return ret;
}

const std::string hexConversion = "0123456789abcdef";

hex ByteSeq::toHex() const {
    hex hx{""};
    for (uint8_t b : this->seq) {
        hx.raw += hexConversion[b >> 4];
        hx.raw += hexConversion[b & 15];
    }
    return hx;
}

const std::string B64 = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                        "abcdefghijklmnopqrstuvwxyz"
                        "0123456789+/";

b64 ByteSeq::toB64() const {
    b64 bsx{""};
    int i = 0;
    int chunks = this->seq.size() / 3;
    for (int i = 0; i < chunks; i++) {
        int j = 3 * i;
        bsx.raw += B64.at(this->seq.at(j) >> 2);
        bsx.raw +=
            B64.at(((this->seq.at(j) & 3) << 4) | (this->seq.at(j + 1) >> 4));
        bsx.raw += B64.at(((this->seq.at(j + 1) & 15) << 2) |
                          (this->seq.at(j + 2) >> 6));
        bsx.raw += B64.at(this->seq.at(j + 2) & 63);
    }
    if (this->seq.size() % 3 == 1) {
        int j = 3 * chunks;
        bsx.raw += B64.at(this->seq.at(j) >> 2);
        bsx.raw += B64.at((this->seq.at(j) & 3) << 4);
        bsx.raw += "==";
    } else if (this->seq.size() % 3 == 2) {
        int j = 3 * chunks;
        bsx.raw += B64.at(this->seq.at(j) >> 2);
        bsx.raw +=
            B64.at(((this->seq.at(j) & 3) << 4) | (this->seq.at(j + 1) >> 4));
        bsx.raw += B64.at((this->seq.at(j + 1) & 15) << 2);
        bsx.raw += "=";
    }
    return bsx;
}
