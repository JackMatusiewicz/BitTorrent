#ifndef BITTORRENT_STARTER_CPP_OVERLOAD_H
#define BITTORRENT_STARTER_CPP_OVERLOAD_H

template<typename... Ts>
struct Overload : Ts... {
    using Ts::operator()...;
};

template<typename... Ts>
Overload(Ts...) -> Overload<Ts...>;

#endif //BITTORRENT_STARTER_CPP_OVERLOAD_H
