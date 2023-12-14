#ifndef BITTORRENT_STARTER_CPP_BENCODEDDATA_H
#define BITTORRENT_STARTER_CPP_BENCODEDDATA_H

#include <variant>
#include <string>
#include <numeric>

#include "../memory/Box.h"


// For now this is all that we need.
using BencodedData = std::variant<class Integer, class String, Box<class Array>>;

std::string convert_to_string(const BencodedData& data);

class Integer {
private:
    long long _value;

public:
    explicit Integer(long long i) : _value{i} {}

    [[nodiscard]] long long value() const noexcept { return _value; }
};

class String {
private:
    std::string _value;
public:
    explicit String(std::string&& data) : _value{data} {}

    String(String&& other) noexcept : _value{std::move(other._value)} {}

    [[nodiscard]] const std::string& value() const noexcept { return _value; }
};

class Array {
private:
    std::vector<BencodedData> _values;

public:
    explicit Array(std::vector<BencodedData>&& data) : _values{std::move(data)} {}
    [[nodiscard]] const std::vector<BencodedData>& values() const { return _values; }
};

std::string to_string(const Box<Array>& v);

std::string to_string(const String& v);

std::string to_string(const Integer& v);


#endif //BITTORRENT_STARTER_CPP_BENCODEDDATA_H
