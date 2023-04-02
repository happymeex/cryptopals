#ifndef _BYTE_
#define _BYTE_

#include <string>
#include <vector>

struct hex {
    std::string raw;
};

struct b64 {
    std::string raw;
};

namespace byteseq {
/**
 * An immutable data type representing a sequence of bytes.
 */
class ByteSeq {
  private:
    // The byte sequence.
    std::vector<uint8_t> seq;

  public:
    /**
     * Constructs a byte sequence from the vector of bytes.
     */
    ByteSeq(std::vector<uint8_t> seq);

    /**
     * Constructs a byte sequence from a string, byte-for-byte.
     */
    ByteSeq(const std::string &s);

    /**
     * Constructs a byte sequence from a hex value.
     *
     * @throws Error if hex value has odd length.
     */
    ByteSeq(const hex &hx);

    ByteSeq(const b64 &bsx);

    /**
     * Bitwise xor of two byte sequences.
     */
    ByteSeq operator^(const ByteSeq &b) const;

    // Convert byte sequence to hex.
    hex toHex() const;

    // Convert byte sequence to b64.
    b64 toB64() const;
};
} // namespace byteseq

#endif