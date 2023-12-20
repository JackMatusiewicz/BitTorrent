#ifndef BITTORRENT_STARTER_CPP_OPTIONAL_EXT_H
#define BITTORRENT_STARTER_CPP_OPTIONAL_EXT_H

#include <optional>
#include <functional>

namespace optional {
    template<typename T, typename U>
    std::optional<U> map(std::optional<T>&& input, std::function<U(T&&)> mapper) {
        if (!input.has_value()) {
            return std::nullopt;
        }
        return mapper(std::move(input.value()));
    }
}

#endif //BITTORRENT_STARTER_CPP_OPTIONAL_EXT_H
