#ifndef SOCKETS_RESULT_H
#define SOCKETS_RESULT_H

#include <optional>

template<typename TSuccess, typename TError>
class Result {
private:
    std::optional<TError> _error;
    std::optional<TSuccess> _success;

    explicit Result(std::optional<TSuccess>&& success, std::optional<TError>&& error)
            : _success {std::move(success)}
            , _error {std::move(error)}
    {}

    explicit Result(TSuccess&& success)
            : _success{std::move(success)}
            , _error{std::nullopt} {}

public:
    static Result fromSuccess(TSuccess&& success) {
        return Result(std::move(success));
    }

    static Result fromError(const TError&& error) {
        return Result(std::nullopt, {std::move(error)});
    }

    [[nodiscard]] bool is_error() const noexcept { return _error.has_value(); }

    [[nodiscard]] const TError& get_error() const noexcept {
        return _error.value();
    }

    [[nodiscard]] const TSuccess& get_success() const noexcept {
        return _success.value();
    }
};

#endif //SOCKETS_RESULT_H
