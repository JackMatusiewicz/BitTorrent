#include "MetaInfo.h"

std::optional<MetaInfo> convert_to_metainfo(const BencodedData& data) {
    auto tracker_url = data.value("announce");
    auto info_dict = data.value("info");
    if (!tracker_url.has_value() || !info_dict.has_value()) {
        return std::nullopt;
    }
    auto id = info_dict.value();
    auto name = id->value("name");
    auto file_size = id->value("length");
    auto piece_length = id->value("piece length");
    auto piece_hashes = id->value("pieces").value()->get_string().value();
    std::vector<std::string> pieces{};
    for(auto i = 0; i < piece_hashes.length(); i += 20) {
        pieces.push_back(piece_hashes.substr(i, 20));
    }

    return MetaInfo(
            tracker_url.value()->get_string().value(),
            name.value()->get_string().value(),
            piece_length.value()->get_int().value(),
            std::move(pieces),
            file_size.value()->get_int().value());
}