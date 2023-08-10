#pragma once

#include <string>

namespace tools
{

namespace
{

// Determines if a character is within the specified range
inline bool inRange(uint8_t u, uint8_t lower, uint8_t upper)
{
    return ((u >= lower) && (u <= upper));
}

}

inline std::string utf8ToCp1251(std::string const& str)
{
    std::string res(str.size() * 2 + 2, '\0');

    for(size_t i = 0, j = 0; i < str.size(); ++i) {
        if(str[i] < 128) {
            res[j++] = str[i];
        } else {
            // current char is utf-8 prefix
            uint8_t ctrlChar = (uint8_t)str[i];
            if(++i == str.size()) {
                // unexpected end of src string. ignore the prefix, just
                // break and let the dest string become null-terminated
                break;
            }

            // next char is utf-8 symbol data
            uint8_t dataChar = (uint8_t)str[i];

            if((ctrlChar == 0xd0) && (inRange(dataChar, 0x90, 0xbf))) {
                // cyrillic letters from 'А' to 'п'
                res[j++] = dataChar - 0x90 + 0xc0;
            } else if((ctrlChar == 0xd1) && (inRange(dataChar, 0x80, 0x8f))) {
                // cyrillic letters from 'р' to 'я'
                res[j++] = dataChar - 0x80 + 0xf0;
            } else if((ctrlChar == 0xd0) && (dataChar == 0x81)) {
                // cyrillic letter 'Ё'
                res[j++] = 0xa8;
            } else if((ctrlChar == 0xd1) && (dataChar == 0x91)) {
                // cyrillic letter 'ё'
                res[j++] = 0xb8;
            }
            // if none of the above is true, data char is simply ignored
            // dest counter won't be incremented in this case
        }
    }

    return res.data();
}

inline std::string cp1251ToUtf8(std::string const& str)
{
    std::string res(str.size() * 2 + 2, '\0');

    for(size_t i = 0, j = 0; i < str.size(); ++i) {
        if(str[i] < 128) {
            res[j++] = str[i];
        } else {
            // current char is 8-bit ascii symbol
            uint8_t dataChar = (uint8_t)str[i];

            if(inRange(dataChar, 0xc0, 0xef)) {
                // cyrillic letters from 'А' to 'п'
                res[j++] = 0xd0;
                res[j++] = dataChar - 0x30;
            } else if(inRange(dataChar, 0xf0, 0xff)) {
                // cyrillic letters from 'р' to 'я'
                res[j++] = 0xd1;
                res[j++] = dataChar - 0x70;
            } else if(dataChar == 0xa8) {
                // cyrillic letter 'Ё'
                res[j++] = 0xd0;
                res[j++] = 0x81;
            } else if(dataChar == 0xb8) {
                // cyrillic letter 'ё'
                res[j++] = 0xd1;
                res[j++] = 0x91;
            }
            // if none of the above is true, data char is simply ignored
            // dest counter won't be incremented in this case
        }
    }

    return res.data();
}

}
