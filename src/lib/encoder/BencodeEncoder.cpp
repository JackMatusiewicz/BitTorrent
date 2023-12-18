#include "BencodeEncoder.h"
#include "../tinySHA/TinySHA1.hpp"
#include <numeric>

std::string encode(long long value) {
    return "i" + std::to_string(value) + "e";
}

std::string encode(const std::string& value) {
    return std::to_string(value.size()) + ":" + value;
}

std::string encode(const std::vector<std::string>& value) {
    auto concat = std::accumulate(
            value.begin(),
            value.end(),
            std::string(""),
            [](const std::string& state, const std::string& b) { return state + b; });
    return encode(concat);
}

std::string encode_metainfo(const MetaInfo& metaInfo) {
    return "d" +
        encode("length") + encode(metaInfo.file_length()) +
        encode("name") + encode(metaInfo.name()) +
        encode("piece length") + encode(metaInfo.pieces_length()) +
        encode("pieces") + "" + encode(metaInfo.pieces()) +
        "e";
}

std::tuple<std::array<uint32_t, 5>, std::string> get_info_hash(const MetaInfo& metaInfo) {
    auto encoded_metainfo = encode_metainfo(metaInfo);
    sha1::SHA1 sha;
    sha.processBytes(encoded_metainfo.c_str(), encoded_metainfo.size());
    std::array<uint32_t, 5> digg{};
    auto digest = std::make_unique<uint32_t*>(new uint32_t[5]);
    sha.getDigest(digg.begin());
    char tmp[48];
    snprintf(tmp, 45, "%08x%08x%08x%08x%08x", digg[0], digg[1], digg[2], digg[3], digg[4]);
    return std::make_tuple(digg, tmp);
}

