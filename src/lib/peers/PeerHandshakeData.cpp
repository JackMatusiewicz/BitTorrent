#include "PeerHandshakeData.h"

const std::string BITTORRENT_PROTOCOL = std::string("BitTorrent protocol");

std::vector<char> PeerHandshakeData::encode() const noexcept {
    std::vector<char> data{};

    data.push_back(19);

    for (auto c : BITTORRENT_PROTOCOL) {
        data.push_back(c);
    }

    for (auto i = 0; i < 8; ++i) {
        data.push_back(0);
    }

    for(auto i : _hash.hash) {
        auto byte4 =  i & 0x000000ff;
        auto byte3 = (i & 0x0000ff00) >> 8;
        auto byte2 = (i & 0x00ff0000) >> 16;
        auto byte1 = (i & 0xff000000) >> 24;
        data.push_back(static_cast<char>(byte1));
        data.push_back(static_cast<char>(byte2));
        data.push_back(static_cast<char>(byte3));
        data.push_back(static_cast<char>(byte4));
    }

    for (auto c : _user_id) {
        data.push_back(c);
    }

    return data;
}