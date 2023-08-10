#pragma once

#include <string>
#include <vector>

namespace tools
{

namespace {

static const std::string base64Chars =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789+/";

}

inline std::string base64Encode(std::string const& in)
{
    std::string out;

    int val = 0;
    int valb = -6;

    for(uint8_t c : in) {
        val = (val << 8) + c;
        valb += 8;

        while(valb >= 0) {
            out.push_back(base64Chars[(val >> valb) & 0x3F]);
            valb -= 6;
        }
    }

    if(valb > -6)
        out.push_back(base64Chars[((val << 8) >> (valb + 8)) & 0x3F]);

    while(out.size() % 4)
        out.push_back('=');

    return out;
}

inline std::string base64Decode(std::string const& in)
{
    std::string out;
    std::vector<int> T(256, -1);

    for(int i = 0; i < 64; ++i)
        T[base64Chars[i]] = i;

    int val = 0;
    int valb = -8;

    for(uint8_t c : in) {
        if(T[c] == -1)
            break;

        val = (val << 6) + T[c];
        valb += 6;

        if(valb >= 0) {
            out.push_back((char)((val >> valb) & 0xFF));
            valb -= 8;
        }
    }

    return out;
}

}
