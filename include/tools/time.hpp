#pragma once

#include <string>
#include <sstream>
#include <iomanip>

namespace tools
{

inline std::string getFormatTime(std::time_t time, std::string_view format)
{
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time), format.data());
    return ss.str();
}

inline std::string getShortTime(std::time_t time)
{
    return getFormatTime(time, "%Y%m%d%H%M%S");
}

inline std::string  getIsoTime(std::time_t time)
{
    return getFormatTime(time, "%Y%m%dT%H%M%S");
}

inline std::string getReadableTime(std::time_t time)
{
    return getFormatTime(time, "%Y-%m-%d %H:%M:%S");
}

}
