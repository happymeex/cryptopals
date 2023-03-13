#ifndef _HEX_
#define _HEX_

#include <iostream>

struct hex {
    std::string raw;
    std::string toString();
};

/**
 * Computes the bitwise or of two hex strings.
 * Uses a truncation of one if necessary so that hex strings have equal length.
 */
hex hexor(hex hx1, hex hx2);

/**
 * Converts a hex string to base 64.
 */
std::string hexTo64(hex hx);

/**
 * Converts an unsigned integer to a hex string, zero-padded
 * if necessary to a given length.
 */
hex intToHex(long x, int length);

/**
 * Converts a string represented an unsigned hex value to an integer.
 */
int intFromHexRaw(std::string s);

/**
 * Encodes a string to hex.
 */
hex toHex(std::string s);

std::string fromHex(std::string s);

#endif
