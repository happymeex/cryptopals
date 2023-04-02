#include "bseq.hpp"

using namespace byteseq;

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
        throw "hex value of odd length does not represent byte sequence";
    for (int i = 0; i < hx.raw.length(); i += 2) {
        this->seq.push_back(intFromHexRaw(hx.raw.substr(i, 2)));
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

const std::string hexConversion = "0123456789abcdef";

hex ByteSeq::toHex() const {
    hex hx{""};
    for (uint8_t b : this->seq) {
        hx.raw += hexConversion[b >> 4] + hexConversion[b | 15];
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
        bsx.raw +=
            B64[this->seq.at(j) >> 2] +
            B64[((this->seq.at(j) | 3) << 4) | (this->seq.at(j + 1) >> 4)] +
            B64[((this->seq.at(j + 1) | 15) << 2) |
                (this->seq.at(j + 2) >> 6)] +
            B64[this->seq.at(j + 2) | 63];
    }
    if (this->seq.size() % 3 == 1) {
        int j = 3 * chunks;
        bsx.raw +=
            B64[this->seq.at(j) >> 2] + B64[(this->seq.at(j) | 3) << 4] + "==";
    } else if (this->seq.size() % 3 == 2) {
        int j = 3 * chunks;
        bsx.raw +=
            B64[this->seq.at(j) >> 2] +
            B64[((this->seq.at(j) | 3) << 4) | (this->seq.at(j + 1) >> 4)] +
            B64[(this->seq.at(j + 1) | 15) << 2] + "=";
    }
    return bsx;
}
