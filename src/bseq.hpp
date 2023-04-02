#ifndef _BYTE_
#define _BYTE_

#include "hex.hpp"
#include <cstddef>
#include <string>
#include <vector>

namespace byteseq {
/**
 * An immutable data type representing a sequence of bytes.
 */
class ByteSeq {
  private:
    // The byte sequence.
    std::vector<std::byte> seq;

  public:
    /**
     * Constructs a byte sequence from the vector of bytes.
     */
    ByteSeq(std::vector<std::byte> seq);

    /**
     * Constructs a byte sequence from a string, byte-for-byte.
     */
    ByteSeq(const std::string &s);

    /**
     * Constructs a byte sequence from a hex value.
     *
     * @throws Error if hex value has odd length.
     */
    ByteSeq(const hex &s);

    /**
     * Bitwise xor of two byte sequences.
     */
    ByteSeq operator^(const ByteSeq &b) const;
};
} // namespace byteseq

#endif