#include "Hex.h" 
#include <algorithm>
#include <sstream>

const std::string B64 = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                        "abcdefghijklmnopqrstuvwxyz"
                        "0123456789+/";

hex intToHex(int x, int length){
    std::stringstream ss;
    ss << std::hex << x;
    std::string ret = ss.str();
    std::string padding(length - ret.length(), '0');
    return hex{padding + ret};
}

hex toHex(std::string s){
    std::string ret = "";
    for (auto c : s){
        ret += intToHex(c, 2).raw;
    }
    return hex{ret};
}

int intFromHexRaw(std::string s){
    return std::stoi(s, 0, 16);
}

std::string hex::toString(){
    std::string ret = "";
    int length = this->raw.length();
    for (int i = 0; i < length; i += 2){
        int val = intFromHexRaw(this->raw.substr(i, 2));
        if (val > 127 || val < 0){
            throw "invalid string";
        }
        ret += val;
    }
    return ret;
}

bool hex::isValidString(){
    try{
        this->toString();
        return true;
    }
    catch(const char* msg){
        return false;
    }
}

std::string hexTo64(hex hx){
    std::string curr = "   ";
    std::string ret = "";
    std::string s = hx.raw;
    for (int i = 1; i <= s.length(); i++){
        int j = s.length() - i;
        curr[j%3] = s[j];
        if (i%3 == 0 || j == 0){
            int val = intFromHexRaw(curr);
            curr = "   ";
            ret += B64[val & 63];
            ret += B64[val >> 6];
        }
    }
    reverse(ret.begin(), ret.end());
    return ret;
}

hex hexor(hex h1, hex h2){
    std::string ret = "";
    std::string s1 = h1.raw, s2 = h2.raw;
    int len = std::min(s1.length(), s2.length());
    for (int i = 0; i < len; i += 7){
        int sublen = std::min(7, len - i);
        int val1 = intFromHexRaw(s1.substr(i, sublen));
        int val2 = intFromHexRaw(s2.substr(i, sublen));
        int val = val1 ^ val2;
        ret += intToHex(val, sublen).raw;
    }
    return hex{ret};
}

