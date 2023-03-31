#include "Hex.h" 
#include <algorithm>
#include <cctype>
#include <sstream>

const std::string B64 = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                        "abcdefghijklmnopqrstuvwxyz"
                        "0123456789+/";

hex intToHex(int x, int length){
    std::stringstream ss;
    ss << std::hex << x;
    std::string ret = ss.str();
    try{
        std::string padding(length - ret.length(), '0');
        return hex{padding + ret};
    }
    catch (...){
        throw "input integer does not fit in hex of given length";
    }
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
        if (val > 255 || val < 0){
            throw "hex doublet does not represent 8-bit value";
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

int b64ToInt(char c){
    if (std::isupper(c)) return c - 'A';
    else if (std::islower(c)) return c - 'a' + 26;
    else if (std::isdigit(c)) return c - '0' + 52;
    else if (c == '+') return 62;
    else if (c == '/') return 63;
    throw "invalid base64";
}

/**
 * Converts a base 64 string to an integer, assuming
 * the true value fits into an integer.
 * Throws an error if input contains invalid base 64 character.
 */
int b64ToInt(std::string s){
    int ret = 0;
    for (auto it = s.begin(); it < s.end(); it++){
        ret |= b64ToInt(*it);
        ret <<= 6;
    }
    ret >>= 6;
    return ret;
}

hex b64ToHex(std::string s){
    int len = s.length();
    if (len == 0) return hex{""};
    if (len % 4 != 0) throw "Improperly formatted base 64";
    std::string raw = "";
    for (int i = 0; i < len - 4; i += 4){
        raw += intToHex(b64ToInt(s.substr(i, 4)), 6).raw;
    }
    std::string last = s.substr(len - 4, 4);
    if (last[2] == '=' && last[3] == '='){
        raw += intToHex(b64ToInt(last.substr(0, 2)) >> 4, 2).raw;
    }
    else if (last[3] == '='){
        raw += intToHex(b64ToInt(last.substr(0, 3)) >> 2, 4).raw;
    }
    else raw += intToHex(b64ToInt(last), 6).raw;
    return hex{raw};
}

