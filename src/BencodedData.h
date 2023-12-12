#ifndef BITTORRENT_STARTER_CPP_BENCODEDDATA_H
#define BITTORRENT_STARTER_CPP_BENCODEDDATA_H

#include <variant>
#include <string>

struct Integer {
    int value;
};

struct String {
    std::string value;
};

// For now this is all that we need.
using BencodedData = std::variant<Integer, String>;

std::string to_string(const String& v) {
    return '\"' + v.value + '\"';
}

std::string to_string(const Integer& v) {
    return std::to_string(v.value);
}

std::string convert_to_string(const BencodedData& data) {
    return std::visit([](auto&& arg){ return to_string(arg); }, data);
}


#endif //BITTORRENT_STARTER_CPP_BENCODEDDATA_H
