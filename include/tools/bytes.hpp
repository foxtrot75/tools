#pragma once

#include <string>
#include <vector>
#include <iomanip>

namespace tools
{

template<typename Numeric>
inline Numeric swap(Numeric num, std::size_t len = sizeof(Numeric))
{
    Numeric result = 0;
    uint8_t* src = (uint8_t*)&num;
    uint8_t* dest = (uint8_t*)&result;

    if(len > sizeof(Numeric))
        len = sizeof(Numeric);

    std::size_t shift = sizeof(Numeric)-len;
    for(std::size_t i = 0; i < len; ++i)
        dest[i] = src[sizeof(Numeric)-shift-i-1];

    return result;
}

inline std::string bytesToHex(std::vector<uint8_t> const& v)
{
    std::stringstream ss;
    ss << std::hex << std::setfill('0');
    for(auto it = v.begin(); it != v.end(); ++it)
        ss << std::setw(2) << (uint16_t)(*it);
    return ss.str();
}

inline std::string bytesToHex(std::string_view v)
{
    return bytesToHex(std::vector<uint8_t>(v.begin(), v.end()));
}

template <typename Numeric>
inline std::string bytesToHex(Numeric const v)
{
    Numeric rv = swap(v);
    return bytesToHex(std::vector<uint8_t>((uint8_t*)&rv, (uint8_t*)&rv+sizeof(Numeric)));
}

inline std::vector<uint8_t> hexToBytes(std::string_view hex)
{
    std::vector<uint8_t> v;
    for(auto it = hex.begin(); it != hex.end(); std::advance(it, 2)) {
        std::string slice(it, it+2);
        uint8_t byte = (uint8_t)std::stoi(slice, nullptr, 16);
        v.push_back(byte);
    }
    return v;
}

inline uint8_t byteToBcd(uint8_t b)   { b %= 100; return ((b/10)<<4)|(b%10); }
inline uint8_t byteFromBcd(uint8_t b) { return (b&0x0F)+10*(b>>4); }

inline std::vector<uint8_t> uintToBcd(uint u)
{
    std::vector<uint8_t> v;
    while(u > 0) {
        uint8_t b = byteToBcd(u%100);
        u /= 100;
        v.push_back(b);
    }
    return v;
}

}
