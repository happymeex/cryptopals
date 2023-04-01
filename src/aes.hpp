#ifndef _AES_
#define _AES_

#include "../src/hex.hpp"
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

hex aes128_ecb_decrypt(const hex &cipher, const std::string &key);

/**
 * Inverse Rijndael column mixing step.
 */
block inverseMixColumns(const block &in);

#endif
