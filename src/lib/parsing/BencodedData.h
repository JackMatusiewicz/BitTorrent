#ifndef BITTORRENT_STARTER_CPP_BENCODEDDATA_H
#define BITTORRENT_STARTER_CPP_BENCODEDDATA_H

#include <variant>
#include <string>

#include "../memory/Box.h"

class Integer {
private:
    long _value;

public:
    explicit Integer(int i) : _value{i} {}

    [[nodiscard]] int value() const noexcept { return _value; }
};

class String {
private:
    std::string _value;
public:
    explicit String(std::string&& data) : _value{data} {}

    String(String&& other) noexcept : _value{std::move(other._value)} {}

    [[nodiscard]] const std::string& value() const noexcept { return _value; }
};

// For now this is all that we need.
using BencodedData = std::variant<Integer, String, Box<class Array>>;

class Array {
    std::vector<BencodedData> value;
};

std::string to_string(const Box<Array>& v) {
    // We will get to this later.
    throw std::exception();
}

std::string to_string(const String& v) {
    return '\"' + v.value() + '\"';
}

std::string to_string(const Integer& v) {
    return std::to_string(v.value());
}

std::string convert_to_string(const BencodedData& data) {
    return std::visit([](auto&& arg){ return to_string(arg); }, data);
}


#endif //BITTORRENT_STARTER_CPP_BENCODEDDATA_H
