#include "peer_discovery.h"
#include "../http/httplib.h"
#include "../encoder/BencodeEncoder.h"
#include "../decoder/BencodeDecoder.h"

std::tuple<std::string, std::string> split_domain_and_endpoint(const std::string& tracker_url) {
    auto last_forward_slash_index = tracker_url.find_last_of('/');
    auto domain = tracker_url.substr(0, last_forward_slash_index);
    auto endpoint = tracker_url.substr(last_forward_slash_index, tracker_url.size() - last_forward_slash_index);

    return std::make_tuple(domain, endpoint);
}

std::string encode_info_hash(const std::string& hash) {
    std::string encoded;
    for(auto i = 0; i < hash.size(); i += 2) {
        encoded += '%' + hash.substr(i, 2);
    }

    return encoded;
}

std::vector<Peer> decode_peers(std::string&& encoded_peers) {
    std::vector<Peer> peers{};

    for(auto i = 0; i < encoded_peers.size(); i += 6) {
        auto peer = encoded_peers.substr(i, 6);
        // First four bytes are the ip, the final two bytes are the port in big endian.
        peers.push_back(
                Peer(
                {
                        static_cast<unsigned char>(peer[0]),
                        static_cast<unsigned char>(peer[1]),
                        static_cast<unsigned char>(peer[2]),
                        static_cast<unsigned char>(peer[3])
                    },
                (static_cast<unsigned short>(peer[4]) << 8) | static_cast<unsigned char>(peer[5])));
    }

    return peers;
}

std::vector<Peer> get_peers(const MetaInfo& mi) {
    httplib::Client cli(mi.tracker_url());
    httplib::Params params{
            {"peer_id", "00112233445566778899"},
            {"port", "6881"},
            {"uploaded", "0"},
            {"downloaded", "0"},
            {"left", std::to_string(mi.file_length())},
            {"compact", "1"}
    };
    auto domain_and_endpoint = split_domain_and_endpoint(mi.tracker_url());
    httplib::Headers headers{};
    //auto resp = cli.Get("", params, headers);
    auto resp =
            httplib::Client(std::get<0>(domain_and_endpoint))
                    // By moving the info hash here we can avoid the url encoding of the query parameter.
                    .Get(std::get<1>(domain_and_endpoint) + "?info_hash=" + encode_info_hash(get_info_hash(mi)), params, headers);

    BencodeDecoder decoder(std::make_shared<std::vector<byte>>(std::vector<byte>(resp->body.begin(), resp->body.end())));
    auto dictionary = decoder.consume();
    if (!dictionary.has_value()) {
        return {};
    }
    auto peers = get_from(dictionary.value(), "peers");
    if (!peers.has_value()) {
        return {};
    }
    return decode_peers(get_string_value(peers.value()).value());
}