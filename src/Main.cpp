#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#include "lib/nlohmann/json.hpp"
#include "lib/bencode/decoder/BencodeDecoder.h"
#include "lib/info/MetaInfo.h"
#include "lib/bencode/encoder/BencodeEncoder.h"
#include "lib/peers/PeerDiscovery.h"
#include "lib/sockets/ConnectedSocketClient.h"
#include "lib/peers/PeerHandshakeData.h"

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
    BencodeDecoder decoder(std::make_shared<std::vector<char>>(std::vector<char>(file_data.begin(), file_data.end())));
    auto dictionary = decoder.consume();
    auto mi = convert_to_metainfo(dictionary.value()).value();

    return mi;
}

std::tuple<std::string, uint16_t> parse_connection_info(const std::string& ip_and_port) {
    auto colon_pos = ip_and_port.find(':');
    auto ip = ip_and_port.substr(0, colon_pos);
    auto port = ip_and_port.substr(colon_pos + 1, ip_and_port.size() - colon_pos);
    return std::make_tuple(ip, std::stoi(port));
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
        BencodeDecoder decoder(std::make_shared<std::vector<char>>(std::vector<char>(encoded_value.begin(), encoded_value.end())));
        auto decoded_value = decoder.consume();
        std::cout << decoded_value.value().to_string() << std::endl;
    } else if (command == "info") {
        auto mi = get_metainfo(argv[2]);
        auto hash = get_info_hash_string(mi);
        std::cout << "Tracker URL: " << mi.tracker_url() << std::endl;
        std::cout << "Length: " << mi.file_length() << std::endl;
        std::cout << "Info Hash: " << hash <<std::endl;
        std::cout << "Piece Length: " << mi.pieces_length() << std::endl;
        std::cout << "Piece Hashes:" << std::endl;
        print_piece_hashes_hex(mi.pieces());
    } else if (command == "peers") {
        auto mi = get_metainfo(argv[2]);
        auto peers = peers::get_peers(mi);
        for (auto peer : peers) {
            std::cout << peer.to_string() << std::endl;
        }
    } else if (command == "handshake") {
        auto mi = get_metainfo(argv[2]);
        auto ip_and_port = parse_connection_info(argv[3]);
        auto client = ConnectedSocketClient(
                SocketInfo(
                        SocketType::Stream,
                        std::move(std::get<0>(ip_and_port)),
                        std::get<1>(ip_and_port)));
        auto handshake_data = PeerHandshakeData(get_info_hash(mi), "00112233445566778899");
        printf("%02x\n", 'a');
        auto data_to_send = handshake_data.encode();
        auto open_result = client.open_connection();
        auto send_result = client.send_data(data_to_send);
        auto receive_result = client.receive_data();
        auto close_result = client.close_connection();

        // We know that the final 20 bytes are the peerId, so we can lift them from the returned data and print it.
        if (!receive_result.is_error()) {
            auto data = receive_result.get_success();
            // Need to print the string as hexadecimal
            std::cout << "Peer ID: ";
            for(auto i = data.size() - 20; i < data.size(); ++i) {
                printf("%02x", static_cast<unsigned char>(data[i]));
            }
            std::cout << std::endl;
        }
    } else {
        std::cerr << "unknown command: " << command << std::endl;
        return 1;
    }

    return 0;
}
