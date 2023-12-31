#ifndef BITTORRENT_STARTER_CPP_BENCODEDECODER_H
#define BITTORRENT_STARTER_CPP_BENCODEDECODER_H

#include <vector>
#include <memory>
#include <optional>
#include "../BencodedData.h"

class BencodeDecoder {
private:
    std::shared_ptr<std::vector<char>> _data;
    std::size_t _position;

    std::optional<String> decode_string();
    std::optional<Integer> decode_integer();
    std::optional<Box<Array>> decode_list();
    std::optional<Box<Dictionary>> decode_dictionary();

public:
    explicit BencodeDecoder(std::shared_ptr<std::vector<char>>&& data) : _data{std::move(data)}, _position{0} {}

    BencodeDecoder(const BencodeDecoder& rhs) = default;
    BencodeDecoder(BencodeDecoder&& rhs)  noexcept : _data{std::move(rhs._data)}, _position{rhs._position} {}

    std::optional<BencodedData> consume();
};

#endif //BITTORRENT_STARTER_CPP_BENCODEDECODER_H
