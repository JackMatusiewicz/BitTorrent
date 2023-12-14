#ifndef BITTORRENT_STARTER_CPP_BENCODEDECODER_H
#define BITTORRENT_STARTER_CPP_BENCODEDECODER_H

#include <vector>
#include <memory>
#include "../parsing/ParseResult.h"
#include "../parsing/BencodedData.h"

using byte = unsigned char;

class BencodeDecoder {
private:
    std::shared_ptr<std::vector<byte>> _data;
    std::size_t _position;

    std::optional<BencodedData> decode_string(const std::vector<byte>& data);
    std::optional<BencodedData> decode_integer(const std::vector<byte>& data);

public:
    explicit BencodeDecoder(std::shared_ptr<std::vector<byte>>&& data) : _data{std::move(data)}, _position{0} {}

    BencodeDecoder(const BencodeDecoder& rhs) = default;
    BencodeDecoder(BencodeDecoder&& rhs)  noexcept : _data{std::move(rhs._data)}, _position{rhs._position} {}

    std::optional<BencodedData> consume();
};

#endif //BITTORRENT_STARTER_CPP_BENCODEDECODER_H
