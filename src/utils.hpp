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
 * @throws error if `s` is not a valid string (i.e. characters have negative
 * ASCII value)
 */
double score(std::string s);

/**
 * Computes the hamming distance between two byte sequences (viewed
 * as binary strings).
 *
 * @throws error if hex values have differing lengths
 */
int hammingDistance(ByteSeq bs1, ByteSeq bs2);

#endif