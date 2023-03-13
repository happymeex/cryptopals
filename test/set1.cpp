#include "../src/Hex.h"
#include <algorithm>
#include <cctype>
#include <numbers>
#include <string>
#include <unordered_map>
#include <cmath>

const std::unordered_map<char, double> english_char_freqs = {
    {' ', 0.183},
    {'e', 0.103},
    {'t', 0.075},
    {'a', 0.065},
    {'o', 0.062},
    {'n', 0.057},
    {'i', 0.057},
    {'s', 0.053},
    {'r', 0.050},
    {'h', 0.050},
    {'l', 0.033},
    {'d', 0.033},
    {'u', 0.028},
    {'c', 0.023},
    {'m', 0.020},
    {'f', 0.020},
    {'w', 0.017},
    {'g', 0.016},
    {'p', 0.015},
};

double mse(const std::unordered_map<char, double> &m1, const std::unordered_map<char, double> &m2){
    double ret = 0;
    for (auto it : m1){
        double diff = it.second - (m2.contains(it.first) ? m2.at(it.first) : 0);
        ret += diff * diff;
    }
    return ret;
}

std::string hex_to_64(hex hx){
    return hexTo64(hx);
}

hex fixed_xor(hex hx1, hex hx2){
    return hexor(hx1, hx2);
}

hex single_byte_xor_cipher(hex hx){
    int len = hx.raw.length() / 2;
    double best_mse = 100;
    hex best;
    for (int i = 0; i < 128; i++){
        std::string key = std::string(len, i);
        hex hx_key = toHex(key);
        hex decrypted = hexor(hx, hx_key);
        std::unordered_map<char, double> char_freqs;
        std::string str = decrypted.toString();
        std::transform(str.begin(), str.end(), str.begin(), ::tolower);
        for (auto it: english_char_freqs) char_freqs[it.first] = 0;
        for (char c: str){
            if (!char_freqs.contains(c)){
                char_freqs[c] = 0;
            }
            char_freqs[c] += ((double) 1)/len;
        }
        double error  = mse(char_freqs, english_char_freqs);
        if (error < best_mse){
            best_mse = error;
            best = decrypted;
        }
    }

    return best;
}

int main(int argc, char** argv){
    std::string test(argv[1]);
    if (test == "hex_to_64"){
        if (argc != 3) std::cout << "expected 1 argument" << std::endl;
        else std::cout << hex_to_64(hex{argv[2]}) << std::endl;
    }

    else if (test == "single_byte_xor_cipher"){
        if (argc != 3) std::cout << "expected 1 argument" << std::endl;
        else std::cout << single_byte_xor_cipher(hex{argv[2]}).toString() << std::endl;
    }

    else if (test == "fixed_xor"){
        if (argc != 4) std::cout << "expected 2 arguments" << std::endl;
        else std::cout << hexor(hex{argv[2]}, hex{argv[3]}).raw << std::endl;
    }
}
