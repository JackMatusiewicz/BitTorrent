#include "BencodeEncoder.h"

std::string encode(long long value) {
    return "i" + std::to_string(value) + "e";
}

std::string encode(const std::string& value) {
    return std::to_string(value.size()) + ":" + value;
}

std::string encode_metainfo(const MetaInfo& metaInfo) {
    return "d" +
        encode("length") + encode(metaInfo.file_length()) +
        encode("name") + encode(metaInfo.name()) +
        encode("piece length") + encode(metaInfo.file_length()) +
        encode("pieces") + "" +
        "e";
}