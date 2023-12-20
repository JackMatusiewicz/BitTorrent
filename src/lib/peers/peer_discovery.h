#ifndef BITTORRENT_STARTER_CPP_PEER_DISCOVERY_H
#define BITTORRENT_STARTER_CPP_PEER_DISCOVERY_H

#include <vector>

#include "../info/MetaInfo.h"
#include "peer.h"

namespace peers {
    std::vector<Peer> get_peers(const MetaInfo& info);
}

#endif //BITTORRENT_STARTER_CPP_PEER_DISCOVERY_H
