#pragma once

#include <string>
#include <iomanip>

namespace tools
{

inline std::string urlEncode(std::string const& in)
{
    std::string out;
    out.reserve(in.size() * 1.5);

    for(uint8_t c : in) {
        if(std::isalnum(c)) {
            out.push_back(c);
        } else {
            std::stringstream ss;
            ss << "%" << std::hex << std::setfill('0') << std::setw(2) << (uint16_t)c;
            out.append(ss.str());
        }
    }

    return out;
}

}
