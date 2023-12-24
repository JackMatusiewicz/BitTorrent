#ifndef SOCKETS_CONNECTEDSOCKETCLIENT_H
#define SOCKETS_CONNECTEDSOCKETCLIENT_H

#include <unistd.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <vector>

#include "SocketInfo.h"
#include "../result/Result.h"

/// This is a client that can requires that you connect. This means that you are using the underlying send() and recv(),
/// not the send_to() and recv_from() functions.
class ConnectedSocketClient {
private:

    SocketInfo _socketInfo;
    bool _has_socket;
    int _socket_fd;

public:

    explicit ConnectedSocketClient(SocketInfo&& socketInfo);

    [[nodiscard]] Result<int, std::string> open_connection() noexcept;

    Result<int, std::string> close_connection() noexcept;

    // This is a blocking method, will need to implement a way to make it not so.
    [[nodiscard]]
    Result<int, std::string> send_data(const std::vector<char>& data) const noexcept;

    [[nodiscard]]
    Result<std::vector<char>, std::string> receive_data() const noexcept;
};

#endif //SOCKETS_CONNECTEDSOCKETCLIENT_H
