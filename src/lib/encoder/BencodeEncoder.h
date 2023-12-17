#ifndef BITTORRENT_STARTER_CPP_BENCODEENCODER_H
#define BITTORRENT_STARTER_CPP_BENCODEENCODER_H

#include <string>

#include "../info/MetaInfo.h"

std::string encode_metainfo(const MetaInfo& metaInfo);

#endif //BITTORRENT_STARTER_CPP_BENCODEENCODER_H
