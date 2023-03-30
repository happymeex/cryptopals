#ifndef _HEX_
#define _HEX_

#include <iostream>

struct hex {
    /**
     * A string representing raw hexadecimal.
     */
    std::string raw;

    /**
     * Returns a string representing the hex's decoded string (via ASCII).
     * Throws an error if the hex value does not represent the ASCII encoding of a string.
     */
    std::string toString();

    /**
     * Checks if the raw hex value represents the ASCII encoding of a string.
     */
    bool isValidString();
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
hex intToHex(int x, int length);

/**
 * Converts a string represented an unsigned hex value to an integer.
 */
int intFromHexRaw(std::string s);

/**
 * Encodes a string to hex.
 */
hex toHex(std::string s);

/**
 * Converts a base 64 string to a hex. Requires the input string
 * to have even length.
 */
hex b64ToHex(std::string s);

#endif
