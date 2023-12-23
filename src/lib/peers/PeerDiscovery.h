#ifndef BITTORRENT_STARTER_CPP_PEERDISCOVERY_H
#define BITTORRENT_STARTER_CPP_PEERDISCOVERY_H

#include <vector>

#include "../info/MetaInfo.h"
#include "Peer.h"

namespace peers {
    std::vector<Peer> get_peers(const MetaInfo& info);
}

#endif //BITTORRENT_STARTER_CPP_PEERDISCOVERY_H
