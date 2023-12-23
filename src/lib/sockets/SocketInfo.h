#ifndef SOCKETS_SOCKETINFO_H
#define SOCKETS_SOCKETINFO_H

#include <string>
#include "SocketType.h"

/// The information that is requires to connect to a socket
class SocketInfo {
private:
    SocketType _type;
    std::string _address;
    uint16_t _port;
    bool _use_machine_ip;

public:
    explicit SocketInfo(SocketType type, std::string&& address, uint16_t port)
            : _type(type)
            , _address {std::move(address)}
            , _port(port)
            , _use_machine_ip(false){}

    explicit SocketInfo(SocketType type, uint16_t port)
            : _type(type)
            , _address {}
            , _port(port)
            , _use_machine_ip(true){}

    [[nodiscard]] const std::string& address() const noexcept { return _address; }

    [[nodiscard]] SocketType socketType() const noexcept { return _type; }

    [[nodiscard]] uint16_t port() const noexcept { return _port; }

    [[nodiscard]] bool wants_local_machine() const noexcept { return _use_machine_ip; }
};

#endif //SOCKETS_SOCKETINFO_H
