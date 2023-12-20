#ifndef BITTORRENT_STARTER_CPP_OPTIONAL_EXT_H
#define BITTORRENT_STARTER_CPP_OPTIONAL_EXT_H

#include <optional>
#include <functional>

namespace optional {
    template<typename T, typename U>
    std::optional<U> map(std::optional<T>&& input, std::function<U(T&&)> f) {
        if (!input.has_value()) {
            return std::nullopt;
        }
        return f(std::move(input.value()));
    }

    template<typename T, typename U>
    std::optional<U> bind(std::optional<T>&& input, std::function<std::optional<U>(T&&)> f) {
        if (!input.has_value()) {
            return std::nullopt;
        }
        return f(input.value());
    }
}

#endif //BITTORRENT_STARTER_CPP_OPTIONAL_EXT_H
