#include <string>
#include <vector>

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

std::string convert_to_string(const BencodedData& data) {
    return std::visit([](auto&& arg){ return to_string(arg); }, data);
}