#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <cstdlib>
#include <optional>

#include "lib/nlohmann/json.hpp"
#include "lib/parsing/BencodedData.h"

using json = nlohmann::json;

std::optional<BencodedData> decode_string(const std::string& encoded_value) {
    auto colon_index = encoded_value.find(':');
    if (colon_index != std::string::npos) {
        std::string number_string = encoded_value.substr(0, colon_index);
        auto number = std::stoll(number_string);
        std::string str = encoded_value.substr(colon_index + 1, number);
        return {String {std::move(str)}};
    }

    return std::nullopt;
}

std::optional<BencodedData> decode_integer(const std::string& encoded_value) {
    auto end_index = encoded_value.find('e');
    if (end_index == std::string::npos) {
        return std::nullopt;
    }
    auto digits = encoded_value.substr(1, end_index - 1);
    auto number = std::stoi(digits);
    return { Integer {number }};
}

std::optional<BencodedData> decode_bencoded_value(const std::string& encoded_value) {
    if (std::isdigit(encoded_value[0])) {
        return decode_string(encoded_value);
    } else if (encoded_value[0] == 'i') {
        return decode_integer(encoded_value);
    } else {
        return std::nullopt;
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " decode <encoded_value>" << std::endl;
        return 1;
    }

    std::string command = argv[1];

    if (command == "decode") {
        if (argc < 3) {
            std::cerr << "Usage: " << argv[0] << " decode <encoded_value>" << std::endl;
            return 1;
        }
        // You can use print statements as follows for debugging, they'll be visible when running tests.
        //std::cout << "Logs from your program will appear here!" << std::endl;

        // Uncomment this block to pass the first stage
        std::string encoded_value = argv[2];
        std::optional<BencodedData> decoded_value = decode_bencoded_value(encoded_value);
        std::cout << convert_to_string(decoded_value.value()) << std::endl;
    } else {
        std::cerr << "unknown command: " << command << std::endl;
        return 1;
    }

    return 0;
}
