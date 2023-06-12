#ifndef _AES_
#define _AES_

#include "../src/bseq.hpp"
#include <array>
#include <string>
#include <vector>

/**
 * Represents a 4x4 matrix of bytes, column-major order.
 */
typedef std::array<int, 16> block;

/**
 * Returns the last four bytes in the block.
 */
std::array<int, 4> lastFour(const block &b);

ByteSeq aes128_ecb_decrypt(const ByteSeq &cipher, const std::string &key);

/**
 * Inverse Rijndael column mixing step.
 */
block inverseMixColumns(const block &in);

#endif
