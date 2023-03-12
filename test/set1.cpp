#include "../src/Hex.h"
#include <ostream>
#include <string>
#include <unordered_map>

const std::unordered_map<char, float> english_char_freqs = {
    {'e', 0.1116},
    {'a', 0.0847},
    {'r', 0.0758},
    {'i', 0.0754},
    {'o', 0.0716},
    {'t', 0.0691},
    {'n', 0.0654},
    {'s', 0.0574},
    {'l', 0.0549},
    {'c', 0.0454},
    {'u', 0.0384},
    {'d', 0.0384},
    {'p', 0.0318},
    {'m', 0.0301},
    {'h', 0.0300},
    {'g', 0.0247},
    {'b', 0.0207},
    {'f', 0.0181}
};

std::string hex_to_64(hex hx){
    return hexTo64(hx);
}

void single_byte_xor_cipher(hex hx){
    int len = hx.raw.length() / 2;    
    std::unordered_map<char, float> char_freqs;
    for (int i = 0; i < 128; i++){
        std::string key = std::string(len, i);
        
    }
}

int main(int argc, char** argv){
    std::string test(argv[1]);
    if (test == "hex_to_64"){
        if (argc != 3) std::cout << "expected 1 argument" << std::endl;
        else std::cout << hex_to_64(hex{argv[2]}) << std::endl;
    }
}
