#include "BencodeDecoder.h"

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

std::optional<BencodedData> BencodeDecoder::decode_string(const std::vector<byte>& data) {
    auto colon_index = find_in_vector(data, _position, static_cast<unsigned char>(':'));
    if (colon_index != std::string::npos) {
        std::vector<byte> number_string = slice(data, _position, colon_index);
        auto number = calculate_number(number_string);

        auto final_position = colon_index + 1 + number;
        auto str_bytes = slice(data, colon_index + 1, final_position);
        auto string = std::string();
        string.assign(std::begin(str_bytes), std::end(str_bytes));
        _position = final_position;
        return {BencodedData(String {std::move(string)})};
    }

    return std::nullopt;
}

std::optional<BencodedData> BencodeDecoder::decode_integer(const std::vector<byte>& data) {
    auto end_index = find_in_vector(data, _position, static_cast<unsigned char>('e'));
    if (end_index == std::string::npos) {
        return std::nullopt;
    }
    auto digits = slice(data, _position + 1, end_index);
    auto number = calculate_number(digits);
    _position = end_index + 1;
    return { Integer {number }};
}

std::optional<BencodedData> BencodeDecoder::decode_list(const std::vector<byte> &data) {
    std::vector<BencodedData> _elements{};
    _position += 1;
    while (true) {
        if (_position >= _data->size()) {
            return std::nullopt;
        } else if ((*_data)[_position] == 'e') {
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
    return std::nullopt;
}

std::optional<BencodedData> BencodeDecoder::consume() {
    if (_position >= _data->size()) {
        return std::nullopt;
    }
    if (std::isdigit((*_data)[_position])) {
        return decode_string(*_data);
    } else if ((*_data)[_position] == 'i') {
        return decode_integer(*_data);
    } else if ((*_data)[_position] == 'l') {
        return decode_list(*_data);
    } else {
        return std::nullopt;
    }
}
