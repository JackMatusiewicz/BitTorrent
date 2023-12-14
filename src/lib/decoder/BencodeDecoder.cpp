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

unsigned long long calculate_number(const std::vector<byte>& data) {
    auto sum = 0L;
    for(auto i : data) {
        sum = (sum * 10) + (i - '0');
    }
    return sum;
}

template<typename T>
std::vector<T> slice(const std::vector<T>& data, std::size_t start, std::size_t end) {
    auto s = data.begin() + start;
    auto e = data.begin() + end;
    return std::vector(s, e);
}

std::optional<BencodedData> decode_string(const std::vector<byte>& data, std::size_t pos) {
    auto colon_index = find_in_vector(data, pos, static_cast<unsigned char>(':'));
    if (colon_index != std::string::npos) {
        std::vector<byte> number_string = slice(data, 0, colon_index);
        auto number = calculate_number(number_string);

        auto final_position = colon_index + 1 + number;
        auto str_bytes = slice(data, colon_index + 1, final_position);
        auto string = std::string();
        string.assign(std::begin(str_bytes), std::end(str_bytes));

        return {BencodedData(String {std::move(string)})};
    }

    return std::nullopt;
}

std::optional<BencodedData> decode_integer(const std::string& encoded_value) {
    auto end_index = encoded_value.find('e');
    if (end_index == std::string::npos) {
        return std::nullopt;
    }
    auto digits = encoded_value.substr(1, end_index - 1);
    auto number = std::stoll(digits);
    return { Integer {number }};
}

std::optional<BencodedData> BencodeDecoder::consume() {
    if (_position >= _data->size()) {
        return std::nullopt;
    }
    if (std::isdigit((*_data)[_position])) {
        auto result = decode_string(*_data, _position);
        return result;
    } /*else if ((*_data)[_position] == 'i') {
        //return decode_integer(encoded_value);
    }*/ else {
        return std::nullopt;
    }
}