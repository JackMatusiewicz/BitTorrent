#ifndef BITTORRENT_STARTER_CPP_PEER_H
#define BITTORRENT_STARTER_CPP_PEER_H

#include <array>

class Peer {
private:
    std::array<unsigned char, 4> _ipv4_address;
    unsigned short _port;

public:
    explicit Peer(std::array<unsigned char, 4>&& ip, unsigned short port) : _ipv4_address{ip}, _port{port} {}

    [[nodiscard]] const std::array<unsigned char, 4>& ip_address() const noexcept {
        return _ipv4_address;
    }

    [[nodiscard]] unsigned short port() const noexcept { return _port; }

    [[nodiscard]] std::string to_string() const noexcept {
        return std::to_string(_ipv4_address[0]) + "."
            + std::to_string(_ipv4_address[1]) + "."
            + std::to_string(_ipv4_address[2]) + "."
            + std::to_string(_ipv4_address[3]) + ":"
            + std::to_string(_port);
    }
};

#endif //BITTORRENT_STARTER_CPP_PEER_H
