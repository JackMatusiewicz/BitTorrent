#ifndef BITTORRENT_STARTER_CPP_PARSERESULT_H
#define BITTORRENT_STARTER_CPP_PARSERESULT_H

#include <optional>
#include <functional>

template<typename TSuccess, typename TError>
class ParseResult {
private:
    TSuccess _result;
    TError _error;
    bool _is_success;

public:
    explicit ParseResult(TSuccess success) {
        _result = success;
        _is_success = true;
    }

    explicit ParseResult(TError error) {
        _error = error;
        _is_success = false;
    }

public:
    template<typename TNextSuccess>
    [[nodiscard]] ParseResult<TNextSuccess, TError> and_then(
            std::function<ParseResult<TNextSuccess, TError>(TSuccess)> f) const noexcept {
        if (!_is_success) {
            return ParseResult(_error);
        }
        return f(_result);
    }

    template<typename TNextSuccess>
    [[nodiscard]] ParseResult<TNextSuccess, TError> map(std::function<TNextSuccess(TSuccess)> f) const noexcept {
        if (!_is_success) {
            return ParseResult(_error);
        }
        return ParseResult(f(_result));
    }
};

#endif //BITTORRENT_STARTER_CPP_PARSERESULT_H
