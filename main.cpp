#include <cctype>
#include <iostream>
#include <algorithm>
#include <iterator>
#include <sstream>
#include <string>

const std::string B64 = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                        "abcdefghijklmnopqrstuvwxyz"
                        "0123456789+/";

std::string toHex(long x, int length){
    std::stringstream ss;
    ss << std::hex << x;
    std::string ret = ss.str();
    std::string padding(length - ret.length(), '0');
    return padding + ret;
}

std::string hexTo64(std::string s){
    std::string curr = "   ";
    std::string ret = "";
    for (int i = 1; i <= s.length(); i++){
        int j = s.length() - i;
        curr[j%3] = s[j];
        if (i%3 == 0 || j == 0){
            std::cout << curr << std::endl;
            int val = std::stoi(curr, 0, 16);
            curr = "   ";
            ret += B64[val & 63];
            ret += B64[val >> 6];
        }
    }
    reverse(ret.begin(), ret.end());
    return ret;
}

//assumes hex
std::string fxor(std::string s1, std::string s2){
    std::cout << s1 << " " << s2 << std::endl;
    std::string ret = "";
    for (int i = 0; i < s1.length(); i += 8){
        int val1 = std::stoi(s1.substr(i, 8), 0, 16);
        int val2 = std::stoi(s2.substr(i, 8), 0, 16);
        int val = val1 ^ val2;
        ret += toHex(val, 8);
    }
    return ret;
}

int main(int argc, char** argv){
    //std:: cout << hexTo64(argv[1]) << std::endl;
    std::cout << fxor(argv[1], argv[2]) << std::endl;
}

