#pragma once

#include <string>
#include <fstream>
#include <filesystem>

#include <sys/stat.h>

#include <zlib.h>
#include <archive.h>
#include <archive_entry.h>

namespace tools
{

inline std::string zip(std::string const& data)
{
    std::string result;

    z_stream stream;
    stream.zalloc = nullptr;
    stream.zfree = nullptr;
    stream.opaque = nullptr;
    stream.next_in = (uint8_t*)data.data();
    stream.avail_in = (uint)data.size();

    if(deflateInit(&stream, Z_BEST_COMPRESSION) != Z_OK)
        return {};

    size_t zipSize = 0;
    do {
        size_t resize = zipSize + data.size() * 2;
        result.resize(resize);
        stream.avail_out = (uint)(data.size() * 2);
        stream.next_out = (Bytef*)(&result[0] + zipSize);
        int ret = deflate(&stream, Z_FINISH);
        if(ret != Z_STREAM_END && ret != Z_OK && ret != Z_BUF_ERROR) {
            // std::string errorMsg = stream.msg;
            deflateEnd(&stream);
            return {};
        }
        zipSize += (2 * data.size() - stream.avail_out);
    }
    while(stream.avail_out == 0);

    deflateEnd(&stream);

    result.resize(zipSize);
    return result;
}

inline std::string unzip(std::string const& data)
{
    std::string result;

    z_stream stream;
    stream.zalloc = nullptr;
    stream.zfree = nullptr;
    stream.opaque = nullptr;
    stream.next_in = (uint8_t*)data.data();
    stream.avail_in = (uint)data.size();

    // The windowBits parameter is the base two logarithm of the window size (the size of the history buffer).
    // It should be in the range 8..15 for this version of the library.
    // Larger values of this parameter result in better compression at the expense of memory usage.
    // This range of values also changes the decoding type:
    //  -8 to -15 for raw deflate
    //  8 to 15 for zlib
    // (8 to 15) + 16 for gzip
    // (8 to 15) + 32 to automatically detect gzip/zlib header
    // int windowBits = -15;

    // if(inflateInit2(&stream, windowBits) != Z_OK)
    if(inflateInit(&stream) != Z_OK)
        return {};

    size_t unzipSize = 0;
    do {
        size_t resize = unzipSize + data.size() * 2;
        result.resize(resize);
        stream.avail_out = (uint)(data.size() * 2);
        stream.next_out = (Bytef*)(&result[0] + unzipSize);
        int ret = inflate(&stream, Z_FINISH);
        if(ret != Z_STREAM_END && ret != Z_OK && ret != Z_BUF_ERROR) {
            // std::string errorMsg = stream.msg;
            inflateEnd(&stream);
            return {};
        }
        unzipSize += (2 * data.size() - stream.avail_out);
    }
    while(stream.avail_out == 0);

    inflateEnd(&stream);

    result.resize(unzipSize);
    return result;
}

inline bool zipFile(std::filesystem::path const& in, std::filesystem::path const& out)
{
    std::ifstream ifs(in);
    gzFile outFile = gzopen(out.c_str(), "wb");

    if(outFile == nullptr)
        return false;

    std::string tmp(1024, '\0');
    while(size_t available = ifs.readsome(tmp.data(), tmp.size()))
        gzwrite(outFile, tmp.data(), available);

    gzclose(outFile);

    return true;
}

inline bool archiveFiles(std::vector<std::filesystem::path> const& in, std::filesystem::path const& out)
{
    archive* arch = archive_write_new();
    archive_write_set_format_zip(arch);
    archive_write_open_filename(arch, out.c_str());

    for(auto& file : in) {
        struct stat st;
        stat(file.c_str(), &st);

        archive_entry* entry = archive_entry_new();
        archive_entry_set_pathname(entry, file.filename().c_str());
        archive_entry_copy_stat(entry, &st);
        archive_write_header(arch, entry);

        std::ifstream ifs(file);
        std::string tmp(1024, '\0');
        while(size_t available = ifs.readsome(tmp.data(), tmp.size()))
            archive_write_data(arch, tmp.data(), available);

        archive_entry_free(entry);
        archive_write_finish_entry(arch);
    }

    archive_free(arch);

    return true;
}

}
