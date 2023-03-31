#include <array>

#ifndef _AES_
#define _AES_

/**
 * Represents a 4x4 matrix of bytes, column-major order.
 */
typedef std::array<int, 16> block;

/**
 * Inverse Rijndael column mixing step.
 */
block inverseMixColumns(block in);


#endif
