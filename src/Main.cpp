#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#include "lib/nlohmann/json.hpp"
#include "lib/decoder/BencodeDecoder.h"
#include "lib/info/MetaInfo.h"
#include "lib/encoder/BencodeEncoder.h"
#include "lib/http/httplib.h"

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
        std::cout << "Info Hash: " << std::get<1>(hash) <<std::endl;
        std::cout << "Piece Length: " << mi.pieces_length() << std::endl;
        std::cout << "Piece Hashes:" << std::endl;
        print_piece_hashes_hex(mi.pieces());
    } else if (command == "peers") {
        auto mi = get_metainfo(argv[2]);
        httplib::Client cli(mi.tracker_url());
        httplib::Params params{
                {"peer_id", "00112233445566778899"},
                {"port", "6881"},
                {"uploaded", "0"},
                {"downloaded", "0"},
                {"left", std::to_string(mi.file_length())},
                {"compact", "1"}
        };
        httplib::Headers headers{};
        //auto resp = cli.Get("", params, headers);
        auto resp =
                httplib::Client("http://bittorrent-test-tracker.codecrafters.io")
                // By moving the info hash here we can avoid the url encoding of the query parameter.
                .Get("/announce?info_hash=%d6%9f%91%e6%b2%aeLT%24h%d1%07%3aq%d4%ea%13%87%9a%7f", params, headers);
        std::cout << resp->body << std::endl;

    } else {
        std::cerr << "unknown command: " << command << std::endl;
        return 1;
    }

    return 0;
}
