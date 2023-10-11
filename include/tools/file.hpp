#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <filesystem>

#include <unistd.h>

namespace tools
{

inline std::string& trimStr(std::string& str)
{
    str.erase(0, str.find_first_not_of(" \t\n\r"));
    str.erase(str.find_last_not_of(" \t\n\r") + 1);
    return str;
}

inline std::string readFile(std::filesystem::path const& path, bool trim = true)
{
    std::string result;

    std::ifstream f(path.string(), std::ios::binary);

    try {
        f.seekg(0, std::ios::end);

        int const maybeSize = f.tellg();
        if(maybeSize > 0)
            result.reserve(maybeSize);

        f.seekg(0, std::ios::beg);
        result.assign(
            std::istreambuf_iterator<char>(f),
            std::istreambuf_iterator<char>());

        if(trim)
            trimStr(result);
    }
    catch(std::exception const& e) {
        return {};
    }

    return result;
}

inline bool writeFile(std::filesystem::path const& path, char const* data, size_t size, bool truncate = true)
{
    std::filesystem::create_directories(path.parent_path());

    std::ofstream f(path.string(), std::ios::binary | (truncate ? std::ios::trunc : std::ios::app));

    try {
        f.write((char*)data, size);
        f.flush();
        f.close();

        sync();
    }
    catch(std::exception const& e) {
        return false;
    }

    return true;
}

inline bool writeFile(std::filesystem::path const& path, std::string const& data, bool truncate = true)
{
    return writeFile(path, data.data(), data.size(), truncate);
}

inline bool writeFile(std::filesystem::path const& path, std::vector<uint8_t> const& data, bool truncate = true)
{
    return writeFile(path, (char*)data.data(), data.size(), truncate);
}

}
