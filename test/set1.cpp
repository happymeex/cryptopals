#include "../src/Hex.h"
#include <algorithm>
#include <cctype>
#include <fstream>
#include <math.h>
#include <numbers>
#include <string>
#include <unordered_map>
#include <cmath>

double CHAR_DISTRIBUTION[128];

void makeCharFreq(){
    std::ifstream freq{"test/data/char_freqs.txt"};
    for (int i = 0; i < 128; i++){
        int c; freq >> c;
        char colon; freq >> colon;
        double frequency; freq >> frequency;
        CHAR_DISTRIBUTION[c] = frequency;
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


std::tuple<hex, double, char> single_byte_xor_cipher(hex hx){
    if (!hx.isValidString()) throw "this hex value represents no ASCII string";
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
            continue;
        }
    } 
    return std::tuple<hex, double, char>{best, highestScore, bestKey};
}

hex repeated_key_xor(hex hx, hex key){
    if (!hx.isValidString() || !key.isValidString()) throw "these hex values do not represent strings";
    while (key.raw.length() < hx.raw.length()){
        key.raw += key.raw;
    }
    return hexor(hx, key);
}

int main(int argc, char** argv){
    makeCharFreq();
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
            hex hx = toHex(argv[2]);
            hex key = toHex(argv[3]);
            std::cout << repeated_key_xor(hx, key).raw << std::endl;
        }
    }
}
