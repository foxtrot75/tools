#pragma once

#include <string>
#include <vector>
#include <iomanip>

#include <openssl/md5.h>
#include <openssl/sha.h>
#include <openssl/hmac.h>

#include "bytes.hpp"

namespace tools
{

inline std::string md5(std::string_view data)
{
    std::vector<uint8_t> hash(MD5_DIGEST_LENGTH);
    MD5((uint8_t*)data.data(), data.size(), hash.data());
    return bytesToHex(hash);
}

inline std::string sha1(std::string_view data)
{
    std::vector<uint8_t> hash(SHA_DIGEST_LENGTH);
    SHA1((uint8_t*)data.data(), data.size(), hash.data());
    return bytesToHex(hash);
}

inline std::string sha256(std::string_view data)
{
    std::vector<uint8_t> hash(SHA256_DIGEST_LENGTH);
    SHA256((uint8_t*)data.data(), data.size(), hash.data());
    return bytesToHex(hash);
}


inline std::string hmac(
    EVP_MD const* evpMd,
    uint8_t const* key, std::size_t const keySize,
    uint8_t const* data, std::size_t const dataSize)
{
    std::vector<uint8_t> hash(HMAC_MAX_MD_CBLOCK);
    uint mdSize = 0;
    HMAC(evpMd, key, keySize, data, dataSize, hash.data(), &mdSize);
    hash.resize(mdSize);
    return bytesToHex(hash);
}

template<typename K, typename D>
inline std::string hmac_md5(K const& key, D const& data)
{
    return hmac(EVP_md5(),
        (uint8_t*)std::data(key), std::size(key),
        (uint8_t*)std::data(data), std::size(data));
}

template<typename K, typename D>
inline std::string hmac_sha1(K const& key, D const& data)
{
    return hmac(EVP_sha1(),
        (uint8_t*)std::data(key), std::size(key),
        (uint8_t*)std::data(data), std::size(data));
}

template<typename K, typename D>
inline std::string hmac_sha256(K const& key, D const& data)
{
    return hmac(EVP_sha256(),
        (uint8_t*)std::data(key), std::size(key),
        (uint8_t*)std::data(data), std::size(data));
}

}
