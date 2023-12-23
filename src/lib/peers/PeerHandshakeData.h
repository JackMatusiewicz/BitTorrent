#ifndef BITTORRENT_STARTER_CPP_PEERHANDSHAKEDATA_H
#define BITTORRENT_STARTER_CPP_PEERHANDSHAKEDATA_H

#include <cstdint>
#include <string>
#include <vector>
#include "../info/MetaInfoHash.h"

class PeerHandshakeData {
private:
    MetaInfoHash _hash;
    std::string _user_id;

public:
    explicit PeerHandshakeData(MetaInfoHash&& hash, std::string&& user_id) : _hash{hash}, _user_id{user_id} {}

    std::vector<char> encode() const noexcept;
};

#endif //BITTORRENT_STARTER_CPP_PEERHANDSHAKEDATA_H
