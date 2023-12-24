#include "AutoConnectingSocketClient.h"

Result<AutoConnectingSocketClient, std::string> AutoConnectingSocketClient::create(SocketInfo&& info) {
    auto client = AutoConnectingSocketClient(std::move(info));
    auto open_result = client._client->open_connection();
    if (open_result.is_error()) {
        return Result<AutoConnectingSocketClient, std::string>::fromError(std::move(open_result.get_error()));
    }
    return Result<AutoConnectingSocketClient, std::string>::fromSuccess(std::move(client));
}

AutoConnectingSocketClient::AutoConnectingSocketClient(SocketInfo&& info)
    : _client {std::make_unique<ConnectedSocketClient>(std::move(info))} {
}

AutoConnectingSocketClient::~AutoConnectingSocketClient() {
    if (_client != nullptr) {
        auto _ = _client->close_connection();
    }
}

Result<int, std::string> AutoConnectingSocketClient::send_data(const std::vector<char>& data) const noexcept {
    return _client->send_data(data);
}

Result<std::vector<char>, std::string> AutoConnectingSocketClient::receive_data() const noexcept {
    return _client->receive_data();
}