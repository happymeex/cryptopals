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
std::tuple<byteseq::ByteSeq, double, char>
single_byte_xor_cipher(byteseq::ByteSeq bs);

/**
 * Given a vector of hex strings, determine the string
 * most likely to be an English message encrypted with single-byte xor.
 */
std::tuple<hex, double, char> detect_single_byte_xor(const std::vector<hex> &v);

/**
 * Returns the repeating key xor encryption of a hex string by a hex key,
 * assuming both are encodings of ASCII strings.
 */
hex repeating_key_xor(hex hx, hex key);

/**
 * Returns the repeating key xor encryption of a cipher
 * string by a key string.
 */
hex repeating_key_xor(std::string cipher, std::string keyString);

/**
 * Given a hex value assumed to be a cipher encrypted using
 * repeating key xor, returns a hex value that is likely to be the decrypted
 * text.
 */
hex break_repeating_xor(const hex hx);

/**
 * Given a vector of hex values, returns the vector consisting of those
 * that contain some duplicate block of 16 bytes (32 chars in the raw string).
 * Throws an error if any of the hex values has byte length not divisible by 16.
 */
std::vector<hex> detect_aes_ecb(const std::vector<hex> &in);
#endif