#ifndef BITTORRENT_STARTER_CPP_BENCODEDDATA_H
#define BITTORRENT_STARTER_CPP_BENCODEDDATA_H

#include <variant>
#include <string>

#include "../memory/Box.h"

struct Integer {
    int value;
};

struct String {
    std::string value;
};

// For now this is all that we need.
using BencodedData = std::variant<Integer, String, Box<struct Array>>;

struct Array {
    std::vector<BencodedData> value;
};

std::string to_string(const Box<Array>& v) {
    // We will get to this later.
    throw std::exception();
}

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
