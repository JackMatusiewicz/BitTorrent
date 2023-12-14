#include <string>
#include <vector>
#include <unordered_map>

#include "BencodedData.h"

std::string to_string(const Box<Array>& v) {
    std::vector<std::string> strings{};
    for (const auto& elem : v->values()) {
        strings.push_back(convert_to_string(elem));
    }
    std::string elements_str = std::accumulate(
            std::begin(strings),
            std::end(strings),
            std::string(),
            [] (const std::string& state, const std::string& next) {
                return state.empty() ? next : state + "," + next;
            }
    );

    return "[" + elements_str + "]";
}

std::string to_string(const String& v) {
    return '\"' + v.value() + '\"';
}

std::string to_string(const Integer& v) {
    return std::to_string(v.value());
}

std::string to_string(const Box<Dictionary>& v) {
    std::vector<std::string> element_strings{};
    for (const auto& kvp : v->values()) {
        auto key_string = "\"" + kvp.first + "\"";
        auto value_string = convert_to_string(kvp.second);
        element_strings.push_back(key_string.append(":").append(value_string));
    }
    std::string elements_str = std::accumulate(
            std::begin(element_strings),
            std::end(element_strings),
            std::string(),
            [] (const std::string& state, const std::string& next) {
                return state.empty() ? next : state + "," + next;
            }
    );
    return "[" + elements_str + "]";
}

std::string convert_to_string(const BencodedData& data) {
    return std::visit([](auto&& arg){ return to_string(arg); }, data);
}