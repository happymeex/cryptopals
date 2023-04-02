#include "bseq.hpp"

using namespace byteseq;

ByteSeq::ByteSeq(std::vector<std::byte> seq) : seq(seq) {}

ByteSeq::ByteSeq(const std::string &s) {
    for (char c : s) {
        this->seq.push_back((std::byte)c);
    }
}

ByteSeq::ByteSeq(const hex &hx) {
    int len = hx.raw.length();
    if (len % 2 != 0)
        throw "hex value of odd length does not represent byte sequence";
    for (int i = 0; i < hx.raw.length(); i += 2) {
        this->seq.push_back((std::byte)intFromHexRaw(hx.raw.substr(i, 2)));
    }
}

ByteSeq ByteSeq::operator^(const ByteSeq &b) const {
    std::vector<std::byte> ret;
    int len = this->seq.size();
    if (len != b.seq.size())
        throw "Cannot xor byte sequences of differing lengths";
    for (int i = 0; i < len; i++) {
        ret.push_back(this->seq.at(i) ^ b.seq.at(i));
    }
    return ByteSeq{ret};
}
