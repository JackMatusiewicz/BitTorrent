#ifndef BITTORRENT_STARTER_CPP_METAINFO_H
#define BITTORRENT_STARTER_CPP_METAINFO_H

#include <string>
#include <utility>
#include <optional>
#include <vector>
#include "../parsing/BencodedData.h"

// For now this will only handle the single file metainfo files.
class MetaInfo {
private:
    std::string _tracker_url;
    std::string _name;
    long long _piece_length;
    std::vector<std::string> _piece_hashes_combined;
    long long _file_size;
public:
    explicit MetaInfo(
            const std::string&& tracker_url,
            const std::string&& name,
            long long piece_length,
            const std::vector<std::string>&& piece_hashes,
            long long file_size)
            : _tracker_url {tracker_url}
            , _name {name}
            , _piece_length {piece_length}
            , _piece_hashes_combined {piece_hashes}
            , _file_size {file_size} {}

    [[nodiscard]] const std::string& tracker_url() const noexcept { return _tracker_url; }
    [[nodiscard]] long long file_length() const noexcept { return _file_size; }
    [[nodiscard]] long long pieces_length() const noexcept { return _piece_length; }
    [[nodiscard]] const std::string& name() const noexcept { return _name; }
    [[nodiscard]] const std::vector<std::string>& pieces() const noexcept { return _piece_hashes_combined; }
};

std::optional<MetaInfo> convert_to_metainfo(const BencodedData& data);

#endif //BITTORRENT_STARTER_CPP_METAINFO_H
