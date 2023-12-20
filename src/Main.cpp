#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#include "lib/nlohmann/json.hpp"
#include "lib/bencode/decoder/BencodeDecoder.h"
#include "lib/info/MetaInfo.h"
#include "lib/bencode/encoder/BencodeEncoder.h"
#include "lib/peers/peer_discovery.h"

using json = nlohmann::json;

void print_piece_hashes_hex(const std::vector<std::string>& hashes) {
    for (const auto& hash_value : hashes) {
        for (const auto& c : hash_value) {
            auto unsigned_value = static_cast<unsigned char>(c);
            printf("%02x", unsigned_value);
        }
        std::cout << std::endl;
    }
}

MetaInfo get_metainfo(const std::string& path) {
    std::ifstream in_file(path, std::ios_base::binary);
    std::vector<char> file_data(
            (std::istreambuf_iterator<char>(in_file)),
            std::istreambuf_iterator<char>());
    BencodeDecoder decoder(std::make_shared<std::vector<byte>>(std::vector<byte>(file_data.begin(), file_data.end())));
    auto dictionary = decoder.consume();
    auto mi = convert_to_metainfo(dictionary.value()).value();

    return mi;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " decode <encoded_value>" << std::endl;
        return 1;
    }

    std::string command = argv[1];

    if (command == "decode") {
        if (argc < 3) {
            std::cerr << "Usage: " << argv[0] << " decode <encoded_value>" << std::endl;
            return 1;
        }
        std::string encoded_value = argv[2];
        BencodeDecoder decoder(std::make_shared<std::vector<byte>>(std::vector<byte>(encoded_value.begin(), encoded_value.end())));
        auto decoded_value = decoder.consume();
        std::cout << convert_to_string(decoded_value.value()) << std::endl;
    } else if (command == "info") {
        auto mi = get_metainfo(argv[2]);
        auto hash = get_info_hash(mi);
        std::cout << "Tracker URL: " << mi.tracker_url() << std::endl;
        std::cout << "Length: " << mi.file_length() << std::endl;
        std::cout << "Info Hash: " << hash <<std::endl;
        std::cout << "Piece Length: " << mi.pieces_length() << std::endl;
        std::cout << "Piece Hashes:" << std::endl;
        print_piece_hashes_hex(mi.pieces());
    } else if (command == "peers") {
        auto mi = get_metainfo(argv[2]);
        auto peers = get_peers(mi);
        for (auto peer : peers) {
            std::cout << peer.to_string() << std::endl;
        }
    } else {
        std::cerr << "unknown command: " << command << std::endl;
        return 1;
    }

    return 0;
}
