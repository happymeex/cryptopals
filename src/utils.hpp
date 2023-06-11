#ifndef _UTILS_
#define _UTILS_

#include "bseq.hpp"
#include <string>

/**
 * Given a string of ASCII characters, computes the Bhattacharyya coefficient
 * between the character distribution of the string and the character
 * distribution known to English.
 *
 * @returns the Bhattacharyya coefficient. Larger indicates more likely to be
 * English. Always between 0 and 1.
 */
double score(std::string s);

/**
 * Computes the hamming distance between two strings (viewed
 * as binary strings via ASCII).
 *
 * @returns hamming distance, or -1 if strings have different length
 */
int hammingDistance(std::string s1, std::string s2);

/**
 * Computes the hamming distance between two hex values (viewed
 * as binary strings).
 *
 * @returns hamming distance, or -1 if strings have different length
 * @throws error if hex values have differing lengths
 */
int hammingDistance(hex hx1, hex hx2);

#endif