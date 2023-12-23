#ifndef BITTORRENT_STARTER_CPP_METAINFOHASH_H
#define BITTORRENT_STARTER_CPP_METAINFOHASH_H

#include <cstdint>

struct MetaInfoHash {
    uint32_t hash[5];

    explicit MetaInfoHash(const uint32_t data[5]) {
        for (auto i = 0; i < 5; ++i) {
            hash[i] = data[i];
        }
    }
};

#endif //BITTORRENT_STARTER_CPP_METAINFOHASH_H
