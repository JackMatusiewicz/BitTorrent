#ifndef BITTORRENT_STARTER_CPP_BENCODEDDATA_H
#define BITTORRENT_STARTER_CPP_BENCODEDDATA_H

#include <variant>
#include <string>
#include <numeric>
#include <unordered_map>

#include "../memory/Box.h"
#include "../visit/Overload.h"

class BencodedData;

std::string convert_to_string(const BencodedData& data);

std::optional<BencodedData> get_from(const BencodedData& data, const std::string& key);

std::optional<long long> get_int_value(const BencodedData& data);

std::optional<std::string> get_string_value(const BencodedData& data);

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

    String(const String& other) = default;

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

class Dictionary {
private:
    std::unordered_map<std::string, Box<BencodedData>> _data;
public:
    explicit Dictionary(std::unordered_map<std::string, Box<BencodedData>>&& data)
        : _data{std::move(data)} {}

    [[nodiscard]] const std::unordered_map<std::string, Box<BencodedData>>& values() const { return _data; }
};

class BencodedData {
private:
    std::variant<class Integer, class String, Box<Array>, Box<Dictionary>> _data;

public:
    explicit BencodedData(String&& value) : _data{value} {}
    explicit BencodedData(const Integer& value) : _data{value} {}
    explicit BencodedData(Box<Array>&& value) : _data{value} {}
    explicit BencodedData(Box<Dictionary>&& value) : _data{value} {}

    [[nodiscard]]
    const std::variant<class Integer, class String, Box<Array>, Box<Dictionary>>& data() const noexcept {
        return _data;
    }
};

#endif //BITTORRENT_STARTER_CPP_BENCODEDDATA_H
