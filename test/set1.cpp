#include "../src/Hex.h"
#include <algorithm>
#include <cctype>
#include <fstream>
#include <math.h>
#include <numbers>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <cmath>
#include "../src/aes.h"

double CHAR_DISTRIBUTION[128];

/**
 * Helper function run once by main to load English character frequency data
 */
void makeCharFreq(){
    std::ifstream freq{"test/data/char_freqs.txt"};
    for (int i = 0; i < 128; i++){
        int c; freq >> c;
        char colon; freq >> colon;
        double frequency; freq >> frequency;
        CHAR_DISTRIBUTION[c] = frequency;
    }
}

int hammingDistance(hex hx1, hex hx2){
    if (hx1.raw.length() != hx2.raw.length()) 
        throw "cannot compute hamming distance between strings of unequal length";
    hex hx = hexor(hx1, hx2);
    int len = hx.raw.length();
    int numOnes = 0;
    for (int i = 0 ; i < len; i++) {
        int val = std::stoi(hx.raw.substr(i, 1), 0, 16);
        while (val){
            numOnes += val & 1;
            val >>= 1;
        }
    }
    return numOnes;
}


/**
 * Computes the hamming distance between two strings (viewed
 * as binary strings via ASCII).
 *
 * @returns hamming distance, or -1 if strings have different length
 */
int hammingDistance(std::string s1, std::string s2){
    try{
        return hammingDistance(toHex(s1), toHex(s2));
    }
    catch(const char* msg){
        std::cout << msg << std::endl;
        return -1;
    } 
}

/**
 * Given a string of ASCII characters, computes the Bhattacharyya coefficient
 * between the character distribution of the string and the character distribution known to English.
 *
 * @returns the Bhattacharyya coefficient. Larger indicates more likely to be English.
 *      Always between 0 and 1. 
 */
double score(std::string s){
    int len = s.length();
    double freqs[128]{0};
    for (auto c: s){
        if (c > 127){
            std::cout<<"weird character: " << c << std::endl;
            continue;
        }
        freqs[c] += ((double) 1)/len;
    }
    double bc = 0;
    for (int i = 0; i < 127; i++){
        bc += sqrt(CHAR_DISTRIBUTION[i] * freqs[i]);
    }
    return bc;
}

/**
 * Set 1 challenge 3: given a hex string assumed to be an encryption of
 * an English message using single-byte xor, returns the most likely decrypted hex.
 *
 * @returns a tuple consisting of the decrypted hex, its score (Bhattacharyya index)
 *      as a metric of likelihood of English, and the character key used to generate
 *      the encryption
 */
std::tuple<hex, double, char> single_byte_xor_cipher(hex hx){
    if (!hx.isValidString()) throw "this hex value represents no ASCII string";
    if (hx.raw == "") throw "single byte xor called on empty hex value";
    int len = hx.raw.length() / 2;
    double highestScore = 0;
    hex best;
    int bestKey = -1;
    for (int i = 0; i < 128; i++){
        hex key = toHex(std::string(len, i));
        hex decrypted = hexor(hx, key);
        double stringScore = score(decrypted.toString());
        if (stringScore > highestScore){
            best = decrypted;
            highestScore = stringScore;
            bestKey = i;
        }
    }
    return std::tuple<hex, double, char>{best, highestScore, bestKey};
}

/**
 * Set 1 challenge 4: given a vector of hex strings, determine the string
 * most likely to be an English message encrypted with single-byte xor.
 */
std::tuple<hex, double, char> detect_single_byte_xor(const std::vector<hex> &v){
    double highestScore = 0;
    hex best;
    int bestKey = -1;
    for (hex hx: v){
        try{
            auto [decrypted, stringScore, key] = single_byte_xor_cipher(hx);
            if (stringScore > highestScore){
                highestScore = stringScore;
                best = decrypted;
                bestKey = key;
            }
        }
        catch (const char* msg){
            //catch gibberish hex (i.e. not arising from ASCII text encryption)
            continue;
        }
    } 
    return std::tuple<hex, double, char>{best, highestScore, bestKey};
}

/**
 * Returns the repeating key xor encryption of a hex string by a hex key,
 * assuming both are encodings of ASCII strings.
 */
hex repeating_key_xor(hex hx, hex key){
    if (!hx.isValidString() || !key.isValidString()) throw "these hex values do not represent strings";
    while (key.raw.length() < hx.raw.length()){
        key.raw += key.raw;
    }
    return hexor(hx, key);
}

/**
 * Set 1 challenge 5: Returns the repeating key xor encryption of a cipher string by a key string.
 */
hex repeating_key_xor(std::string cipher, std::string keyString){
    return repeating_key_xor(toHex(cipher), toHex(keyString));
}

/**
 * Set 1 challenge 6: Given a hex value assumed to be a cipher encrypted using repeating key xor,
 * returns a hex value that is likely to be the decrypted text.
 */
hex break_repeating_xor(const hex hx){
    //start by guessing the keysize: for each guessed size, 
    int upperBound = std::min(40, (int) hx.raw.length() / 4);
    std::vector<std::pair<int, double>> keySize {{2, 8}, {3, 8}};
    const int numSamples = 50;
    for (int guess = 2; guess < upperBound; guess++){
        std::vector<hex> samples;
        for (int j = 0; j < numSamples && 2*j*guess < hx.raw.length(); j++){
            samples.push_back(toHex(hx.raw.substr(2*j*guess, 2*guess)));
        }
        double totalHamming = 0;
        for (int i = 0; i < samples.size(); i++){
            for (int j = i+1; j < samples.size(); j++){
                totalHamming += hammingDistance(samples[i], samples[j]);
            }
        }
        double avgHamming = totalHamming / (guess * numSamples * (numSamples - 1) / 2.0);
        for (int i = 0; i < keySize.size(); i++){
            if (avgHamming < keySize[i].second){
                keySize[i] = {guess, avgHamming};
                break;
            }
        }
    }

    //std::cout << "keysize guess: " << keySize[0].first << " " << keySize[1].first << std::endl;
    std::vector<hex> ret;
    for (auto [size, _]: keySize){
        std::vector<hex> block;
        for (int i = 0; i < size; i++) block.push_back(hex{""});
        int len = hx.raw.length();
        for (int i = 0; i < len; i+=2){
            block[(i/2) % size].raw += hx.raw.substr(i, 2);
        }
        std::string key = "";
        for (const auto &hxi: block){
            auto [_, __, blockKey] = single_byte_xor_cipher(hxi);
            key += blockKey;
        }
        ret.push_back(repeating_key_xor(hx, toHex(key)));
    }
    //std::cout << "Guesses: " << ret[0].toString() << std::endl << ret[1].toString() << std::endl;
    return ret[0];
}

/**
 * Given a vector of hex values, returns the vector consisting of those
 * that contain some duplicate block of 16 bytes (32 chars in the raw string).
 * Throws an error if any of the hex values has byte length not divisible by 16.
 */
std::vector<hex> detect_aes_ecb(const std::vector<hex> &in){
    std::vector<hex> ret;
    for (hex hx: in){
        int len = hx.raw.length();
        if (len % 32 != 0) throw "improperly padded hex value";
        std::unordered_set<std::string> blocks;
        for (int i = 0; i < len; i += 32) {
            std::string block = hx.raw.substr(i, 32);
            if (blocks.contains(block)){
                ret.push_back(hx);
                break;
            }
            blocks.insert(block);
        }
    }
    return ret;
}

int main(int argc, char** argv){
    makeCharFreq();
    if (argc < 2){
        std::cout << "expected a test name" << std::endl;
        return 0;
    }
    std::string test(argv[1]);
    if (test == "hex_to_64"){
        if (argc != 3) std::cout << "expected 1 argument" << std::endl;
        else std::cout << hexTo64(hex{argv[2]}) << std::endl;
    }

    else if (test == "fixed_xor"){
        if (argc != 4) std::cout << "expected 2 arguments" << std::endl;
        else std::cout << hexor(hex{argv[2]}, hex{argv[3]}).raw << std::endl;
    }

    else if (test == "single_byte_xor_cipher"){
        if (argc != 3) std::cout << "expected 1 argument" << std::endl;
        auto [decrypted, stringScore, key] = single_byte_xor_cipher(hex{argv[2]});
        std::cout << "key: " << key << std::endl << decrypted.toString() << std::endl;
    }

    else if (test == "detect_single_byte_xor"){
        if (argc != 2) std::cout << "expected no arguments" <<std::endl;
        else{
            std::ifstream file{"test/data/s1c4.txt"};
            std::vector<hex> v;
            for (std::string s; file >> s;){
                //std::cout << s << std::endl;
                hex input = hex{s};
                v.push_back(hex{s});
            }
            auto [decrypted, stringScore, key] = detect_single_byte_xor(v);
            std::cout << "key: " << key << std::endl << decrypted.toString() << std::endl;
            file.close();
        }
    }

    else if (test == "repeating_key_xor"){
        if (argc != 4) std::cout << "expected 2 arguments" << std::endl;
        else{
            std::cout << repeating_key_xor(argv[2], argv[3]).raw << std::endl;
        }
    }

    else if (test == "hamming"){
        if (argc != 4) std::cout << "expected 2 arguments" << std::endl;
        else{
                std::cout << hammingDistance(argv[2], argv[3]) << std::endl;
        }
    }

    else if (test == "break_repeating_key_xor"){
        std::ifstream file{"test/data/s1c6.txt"};
        std::string encrypted = "";
        for (std::string s; file >> s;){
            encrypted += s;
        }
        hex decrypted = break_repeating_xor(b64ToHex(encrypted));
        std::cout << decrypted.toString() << std::endl;
    }

    else if (test == "aes"){
        try{
            std::ifstream file{"test/data/s1c7.txt"};
            std::string encrypted = "";
            for (std::string s; file >> s;){
                encrypted += s;
            }
            hex cipher = b64ToHex(encrypted);
            hex ret = aes128_ecb_decrypt(cipher, "YELLOW SUBMARINE");
            std::cout << ret.toString() << std::endl;
        }
        catch (const char* message){
            std::cout << message << std::endl;
        }

    }

    else if (test == "detect_aes"){
        try{
            std::ifstream file{"test/data/s1c8.txt"};
            std::vector<hex> in;
            for (std::string s; file >> s;){
                in.push_back(hex{s});
            }
            std::vector<hex> out = detect_aes_ecb(in);
            for (const auto &hx: out){
                std::cout << hx.raw << std::endl;
            }
        }
        catch (const char* message){
            std::cout << message << std::endl;
        }
    }
}
