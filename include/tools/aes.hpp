#pragma once

#include <string>

#include <openssl/aes.h>
#include <openssl/evp.h>

namespace tools
{

inline std::string aes256CbcDecode(std::string const& data, std::string const& key)
{
    size_t const offset = AES_BLOCK_SIZE + 32;

    std::string out(data.size(), 0);
    int len = out.size();

    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    EVP_DecryptInit(ctx, EVP_aes_256_cbc(), (uint8_t*)key.data(), (uint8_t*)data.data());
    EVP_DecryptUpdate(ctx, (uint8_t*)&out[0], &len, (uint8_t*)&data[offset], data.size());
    EVP_DecryptFinal(ctx, (uint8_t*)&out[0 + len], &len);
    EVP_CIPHER_CTX_free(ctx);

    out.resize(data.size() - offset);

    return out;
}

}
