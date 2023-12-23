#ifndef BITTORRENT_STARTER_CPP_BENCODEENCODER_H
#define BITTORRENT_STARTER_CPP_BENCODEENCODER_H

#include <string>

#include "../../info/MetaInfo.h"
#include "../../info/MetaInfoHash.h"

std::string encode_metainfo(const MetaInfo& metaInfo);

std::string get_info_hash_string(const MetaInfo& metaInfo);

MetaInfoHash get_info_hash(const MetaInfo& metaInfo);

#endif //BITTORRENT_STARTER_CPP_BENCODEENCODER_H
