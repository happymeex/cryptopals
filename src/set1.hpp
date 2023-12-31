#ifndef _SET1_
#define _SET1_

#include "bseq.hpp"
#include <vector>

/**
 * Given a byte sequence assumed to be an encryption of
 * an English message using single-byte xor, returns the most likely decrypted
 * byte sequence.
 *
 * @returns a tuple consisting of decrypted byte sequence, its score
 * (Bhattacharyya index) as a metric of likelihood of English, and the character
 * key used to generate the encryption
 */
std::tuple<ByteSeq, double, char> single_byte_xor_cipher(ByteSeq bs);

/**
 * Given a vector of hex strings, determine the string
 * most likely to be an English message encrypted with single-byte xor.
 */
std::tuple<ByteSeq, double, char>
detect_single_byte_xor(const std::vector<ByteSeq> &v);

/**
 * Returns the repeating key xor encryption of one byte sequence by another.
 */
ByteSeq repeating_key_xor(ByteSeq bs, ByteSeq key);

/**
 * Given a hex value assumed to be a cipher encrypted using
 * repeating key xor, returns a hex value that is likely to be the decrypted
 * text.
 */
ByteSeq break_repeating_xor(const ByteSeq bs);

/**
 * Given a vector of hex values, returns the vector consisting of those
 * that contain some duplicate block of 16 bytes (32 chars in the raw string).
 * Throws an error if any of the hex values has byte length not divisible by 16.
 */
std::vector<hex> detect_aes_ecb(const std::vector<hex> &in);
#endif