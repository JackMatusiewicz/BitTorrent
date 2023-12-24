#ifndef SOCKETS_AUTOCONNECTINGSOCKETCLIENT_H
#define SOCKETS_AUTOCONNECTINGSOCKETCLIENT_H

#include <memory>
#include "ConnectedSocketClient.h"

class AutoConnectingSocketClient {
private:
    std::unique_ptr<ConnectedSocketClient> _client;

    explicit AutoConnectingSocketClient(SocketInfo&& info);

public:
    static Result<AutoConnectingSocketClient, std::string> create(SocketInfo&& info);

    ~AutoConnectingSocketClient();

    AutoConnectingSocketClient(const AutoConnectingSocketClient& rhs) = delete;

    AutoConnectingSocketClient(AutoConnectingSocketClient&& rhs) = default;

    [[nodiscard]]
    Result<int, std::string> send_data(const std::vector<char>& data) const noexcept;

    [[nodiscard]]
    Result<std::vector<char>, std::string> receive_data() const noexcept;
};

#endif //SOCKETS_AUTOCONNECTINGSOCKETCLIENT_H
