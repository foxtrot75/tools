#pragma once

#include <string>
#include <iomanip>

#include <openssl/md5.h>
#include <openssl/sha.h>
#include <openssl/hmac.h>

namespace tools
{

inline std::string md5(std::string const& data)
{
    unsigned char hash[MD5_DIGEST_LENGTH];
    MD5_CTX md5;
    MD5_Init(&md5);
    MD5_Update(&md5, (uint8_t*)data.data(), data.size());
    MD5_Final(hash, &md5);
    std::stringstream ss;
    ss << std::hex << std::setfill('0');
    for(int i = 0; i < MD5_DIGEST_LENGTH; i++)
        ss << std::setw(2) << (uint16_t)hash[i];
    return ss.str();
}

inline std::string sha1(std::string const& data)
{
    unsigned char hash[SHA_DIGEST_LENGTH];
    SHA_CTX sha1;
    SHA1_Init(&sha1);
    SHA1_Update(&sha1, (uint8_t*)data.data(), data.size());
    SHA1_Final(hash, &sha1);
    std::stringstream ss;
    ss << std::hex << std::setfill('0');
    for(int i = 0; i < SHA_DIGEST_LENGTH; i++)
        ss << std::setw(2) << (uint16_t)hash[i];
    return ss.str();
}

inline std::string sha256(std::string const& data)
{
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, (uint8_t*)data.data(), data.size());
    SHA256_Final(hash, &sha256);
    std::stringstream ss;
    ss << std::hex << std::setfill('0');
    for(int i = 0; i < SHA256_DIGEST_LENGTH; i++)
        ss << std::setw(2) << (uint16_t)hash[i];
    return ss.str();
}

inline std::string hmac_(EVP_MD const* evp_md, char const* key, size_t const size_key, uint8_t const* data, size_t size_data)
{
    unsigned char hash[HMAC_MAX_MD_CBLOCK];
    HMAC(EVP_md5(), key, size_key, data, size_data, hash, nullptr);
    std::stringstream ss;
    ss << std::hex << std::setfill('0');
    for(int i = 0; i < HMAC_MAX_MD_CBLOCK; i++)
        ss << std::setw(2) << (uint16_t)hash[i];
    return ss.str();
}


inline std::string hmac_md5(char const* key, size_t const size_key, uint8_t const* data, size_t size_data)
{
    return hmac_(EVP_md5(), key, size_key, data, size_data);
}

inline std::string hmac_sha1(char const* key, size_t const size_key, uint8_t const* data, size_t size_data)
{
    return hmac_(EVP_sha1(), key, size_key, data, size_data);
}

inline std::string hmac_sha256(char const* key, size_t const size_key, uint8_t const* data, size_t size_data)
{
    return hmac_(EVP_sha256(), key, size_key, data, size_data);
}


inline std::string hmac_md5(char const* key, size_t const size_key, std::string const& data)
{
    return hmac_(EVP_md5(), key, size_key, (uint8_t const*)data.data(), data.size());
}

inline std::string hmac_sha1(char const* key, size_t const size_key, std::string const& data)
{
    return hmac_(EVP_sha1(), key, size_key, (uint8_t const*)data.data(), data.size());
}

inline std::string hmac_sha256(char const* key, size_t const size_key, std::string const& data)
{
    return hmac_(EVP_sha256(), key, size_key, (uint8_t const*)data.data(), data.size());
}

}
