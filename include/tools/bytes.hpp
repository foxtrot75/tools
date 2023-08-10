#pragma once

#include <string>
#include <vector>
#include <iomanip>

namespace tools
{

template <typename Numeric>
inline Numeric swap(Numeric num, size_t len = sizeof(Numeric))
{
    Numeric result = 0;
    uint8_t* src = (uint8_t*)&num;
    uint8_t* dest = (uint8_t*)&result;

    if(len > sizeof(Numeric))
        len = sizeof(Numeric);

    size_t shift = sizeof(Numeric)-len;
    for(size_t i = 0; i < len; ++i)
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

inline std::string bytesToHex(std::string const& v)
{
    return bytesToHex(std::vector<uint8_t>{v.begin(), v.end()});
}

template <typename Numeric>
inline std::string bytesToHex(Numeric const v)
{
    Numeric rv = swap(v);
    return bytesToHex(std::vector<uint8_t>{(uint8_t*)&rv, (uint8_t*)&rv+sizeof(Numeric)});
}

inline std::vector<uint8_t> hexToBytes(std::string hex)
{
    std::vector<uint8_t> bytes;
    for(size_t i = 0; i < hex.length(); i += 2) {
        std::string slice = hex.substr(i, 2);
        uint8_t byte = (uint8_t)std::stoi(slice, nullptr, 16);
        bytes.push_back(byte);
    }
    return bytes;
}

}
