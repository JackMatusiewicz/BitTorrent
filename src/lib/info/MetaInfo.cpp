#include "MetaInfo.h"

std::optional<MetaInfo> convert_to_metainfo(const BencodedData& data) {
    auto tracker_url = get_from(data, "announce");
    auto info_dict = get_from(data, "info");
    if (!tracker_url.has_value() || !info_dict.has_value()) {
        return std::nullopt;
    }
    auto id = info_dict.value();
    auto name = get_from(id, "name");
    auto file_size = get_from(id, "length");
    auto piece_length = get_from(id, "piece length");
    auto piece_hashes = get_string_value(get_from(id, "pieces").value()).value();
    std::vector<std::string> pieces{};
    for(auto i = 0; i < piece_hashes.length(); i += 20) {
        pieces.push_back(piece_hashes.substr(i, 20));
    }

    return MetaInfo(
            get_string_value(tracker_url.value()).value(),
            get_string_value(name.value()).value(),
            get_int_value(piece_length.value()).value(),
            std::move(pieces),
            get_int_value(file_size.value()).value());
}