#ifndef BITTORRENT_STARTER_CPP_METAINFO_H
#define BITTORRENT_STARTER_CPP_METAINFO_H

#include <string>
#include <utility>
#include <optional>
#include "../parsing/BencodedData.h"

// For now this will only handle the single file metainfo files.
class MetaInfo {
private:
    std::string _tracker_url;
    long long _piece_length;
    std::string _piece_hashes_combined;
    long long _file_size;
public:
    explicit MetaInfo(
            std::string tracker_url,
            long long piece_length,
            std::string piece_hashes,
            long long file_size)
            : _tracker_url {std::move(tracker_url)}
            , _piece_length {piece_length}
            , _piece_hashes_combined {std::move(piece_hashes)}
            , _file_size {file_size} {}

    [[nodiscard]] const std::string& tracker_url() const noexcept { return _tracker_url; }
    [[nodiscard]] long long file_length() const noexcept { return _file_size; }
};


std::optional<MetaInfo> convert_to_metainfo(const BencodedData& data) {
    auto tracker_url = get_from(data, "announce");
    auto info_dict = get_from(data, "info");
    if (!tracker_url.has_value() || !info_dict.has_value()) {
        return std::nullopt;
    }
    auto id = info_dict.value();
    auto file_size = get_from(id, "length");
    auto piece_length = get_from(id, "piece length");
    auto piece_hashes = get_from(id, "pieces");

    return MetaInfo(
            get_string_value(tracker_url.value()).value(),
            get_int_value(piece_length.value()).value(),
            get_string_value(piece_hashes.value()).value(),
            get_int_value(file_size.value()).value());
}

#endif //BITTORRENT_STARTER_CPP_METAINFO_H
