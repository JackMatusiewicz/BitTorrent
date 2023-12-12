#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <cstdlib>
#include <optional>

#include "lib/nlohmann/json.hpp"

using json = nlohmann::json;

std::optional<json> decode_bencoded_value(const std::string& encoded_value) {
    if (std::isdigit(encoded_value[0])) {
        auto colon_index = encoded_value.find(':');
        if (colon_index != std::string::npos) {
            std::string number_string = encoded_value.substr(0, colon_index);
            auto number = std::stoi(number_string);
            std::string str = encoded_value.substr(colon_index + 1, number);
            return {{str}};
        } else {
            return nullptr;
        }
    } else {
        return nullptr;
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
        std::optional<json> decoded_value = decode_bencoded_value(encoded_value);
        std::cout << decoded_value.value().dump().c_str() << std::endl;
    } else {
        std::cerr << "unknown command: " << command << std::endl;
        return 1;
    }

    return 0;
}
