#include "BencodeDecoder.h"
#include "../../optional/optional_ext.h"

#include <optional>

template<typename T>
unsigned long long int find_in_vector(const std::vector<T>& data, std::size_t start_position, T to_find) {
    for (auto i = start_position; i < data.size(); ++i) {
        if (data[i] == to_find) {
            return i;
        }
    }
    return std::string::npos;
}

long long calculate_number(const std::vector<byte>& data) {

    auto string = std::string();
    string.assign(std::begin(data), std::end(data));
    return std::stoll(string);
}

template<typename T>
std::vector<T> slice(const std::vector<T>& data, std::size_t start, std::size_t end) {
    auto s = data.begin() + start;
    auto e = data.begin() + end;
    return std::vector(s, e);
}

std::optional<String> BencodeDecoder::decode_string() {
    auto colon_index = find_in_vector(*_data, _position, static_cast<byte>(':'));
    if (colon_index != std::string::npos) {
        std::vector<byte> number_string = slice(*_data, _position, colon_index);
        auto number = calculate_number(number_string);

        auto final_position = colon_index + 1 + number;
        auto str_bytes = slice(*_data, colon_index + 1, final_position);
        auto string = std::string();
        string.assign(std::begin(str_bytes), std::end(str_bytes));
        _position = final_position;
        return {String {std::move(string)}};
    }

    return std::nullopt;
}

std::optional<Integer> BencodeDecoder::decode_integer() {
    auto end_index = find_in_vector(*_data, _position, static_cast<byte>('e'));
    if (end_index == std::string::npos) {
        return std::nullopt;
    }
    auto digits = slice(*_data, _position + 1, end_index);
    auto number = calculate_number(digits);
    _position = end_index + 1;
    return { Integer {number }};
}

std::optional<Box<Array>> BencodeDecoder::decode_list() {
    std::vector<BencodedData> _elements{};
    _position += 1;
    while (true) {
        if (_position >= _data->size()) {
            return std::nullopt;
        } else if ((*_data)[_position] == 'e') {
            _position += 1;
            return { Box(Array { std::move(_elements) })};
        } else {
            auto elem = consume();
            if (!elem.has_value()) {
                return std::nullopt;
            } else {
                _elements.push_back(std::move(elem.value()));
            }
        }
    }
}

std::optional<Box<Dictionary>> BencodeDecoder::decode_dictionary() {
    std::unordered_map<std::string, Box<BencodedData>> elements{};
    _position += 1;
    while (true) {
        if (_position >= _data->size()) {
            return std::nullopt;
        }
        else if ((*_data)[_position] == 'e') {
            _position += 1;
            return { Box(Dictionary{std::move(elements)})};
        }
        auto key = decode_string();
        if (!key.has_value()) {
            return std::nullopt;
        }
        auto value = consume();
        if (!value.has_value()) {
            return std::nullopt;
        }
        elements.insert({key.value().value(), Box(std::move(value.value()))});
    }
}

std::optional<BencodedData> BencodeDecoder::consume() {
    if (_position >= _data->size()) {
        return std::nullopt;
    }
    if (std::isdigit((*_data)[_position])) {
        return optional::map(
                decode_string(),
                static_cast<std::function<BencodedData(String&&)>>(
                        [](String&& data) -> BencodedData {return BencodedData(std::move(data));}));
    } else if ((*_data)[_position] == 'i') {
        return optional::map(
                decode_integer(),
                static_cast<std::function<BencodedData(Integer&&)>>(
                        [](Integer&& data) -> BencodedData {return BencodedData(std::move(data));}));
    } else if ((*_data)[_position] == 'l') {
        return optional::map(
                decode_list(),
                static_cast<std::function<BencodedData(Box<Array>&&)>>(
                        [](Box<Array>&& data) -> BencodedData {return BencodedData(std::move(data));}));
    } else if ((*_data)[_position] == 'd') {
        return optional::map(
                decode_dictionary(),
                static_cast<std::function<BencodedData(Box<Dictionary>&&)>>(
                        [](Box<Dictionary>&& data) -> BencodedData {return BencodedData(std::move(data));}));
    } else {
        return std::nullopt;
    }
}
