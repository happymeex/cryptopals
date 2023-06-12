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
     * @throws Error if hex value has odd length or if hex does not
     * correspond to an ASCII string
     */
    ByteSeq(const hex &hx);

    /**
     * Constructs a byte sequence from a base 64 value.
     *
     * @throws Error if `strict` is true and the input does not correspond to an
     * ASCII string
     */
    ByteSeq(const b64 &bsx, bool strict = true);

    /**
     * Bitwise xor of two byte sequences.
     */
    ByteSeq operator^(const ByteSeq &b) const;

    /**
     * Concatenation of two byte sequences.
     */
    ByteSeq operator+(const ByteSeq &b) const;

    /**
     * Retrieves the element of the sequence at `index`.
     * @throws error if `index` is out of bounds.
     */
    uint8_t at(int index) const;

    /**
     * Subsequence starting at index `start` up to but not including index
     * `end`. Ignores out-of-bound indices.
     */
    ByteSeq subseq(int start, int end) const;

    // Length of the sequence.
    int length() const;

    // Hamming weight (number of 1s in binary representation of byte sequence).
    int hammingWeight() const;

    // Collate the byte sequence into a human-readable string.
    std::string toString() const;

    // Convert byte sequence to hex.
    hex toHex() const;

    // Convert byte sequence to b64.
    b64 toB64() const;

    // Returns ByteSeq obtained by padding PKCS#7 padding to `target` length
    ByteSeq pkcsPad(int target) const;
};

#endif