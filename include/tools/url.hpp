#pragma once

#include <string>
#include <iomanip>

namespace tools
{

inline std::string urlEncode(std::string_view in)
{
    std::stringstream ss;
    for(uint8_t c: in)
        if(std::isalnum(c))
            ss << c;
        else
            ss << "%" << std::hex << std::setfill('0') << std::setw(2) << (uint16_t)c;
    return ss.str();
}

}
