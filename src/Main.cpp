#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <cstdlib>
#include <optional>

#include "lib/nlohmann/json.hpp"
#include "lib/decoder/BencodeDecoder.h"

using json = nlohmann::json;

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

        std::string encoded_value = argv[2];
        BencodeDecoder decoder(std::make_shared<std::vector<byte>>(std::vector<byte>(encoded_value.begin(), encoded_value.end())));
        auto decoded_value = decoder.consume();
        std::cout << convert_to_string(decoded_value.value()) << std::endl;
    } else {
        std::cerr << "unknown command: " << command << std::endl;
        return 1;
    }

    return 0;
}
