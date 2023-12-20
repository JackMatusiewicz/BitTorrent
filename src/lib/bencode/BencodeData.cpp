#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <optional>

#include "BencodedData.h"

std::string convert_to_string(const Box<Array>& v) {
    std::vector<std::string> strings{};
    for (const auto& elem : v->values()) {
        strings.push_back(elem.to_string());
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

std::string convert_to_string(const String& v) {
    return '\"' + v.value() + '\"';
}

std::string convert_to_string(const Integer& v) {
    return std::to_string(v.value());
}

std::string convert_to_string(const Box<Dictionary>& v) {
    std::vector<std::string> element_strings{};
    for (const auto& kvp : v->values()) {
        auto key_string = "\"" + kvp.first + "\"";
        auto value_string = kvp.second->to_string();
        element_strings.push_back(key_string.append(":").append(value_string));
    }
    std::sort(element_strings.begin(), element_strings.end());
    std::string elements_str = std::accumulate(
            std::begin(element_strings),
            std::end(element_strings),
            std::string(),
            [] (const std::string& state, const std::string& next) {
                return state.empty() ? next : state + "," + next;
            }
    );
    return "{" + elements_str + "}";
}

std::string BencodedData::to_string() const noexcept {
    return std::visit(Overload {
            [] (const Integer& i) { return convert_to_string(i); },
            [] (const String& i) { return convert_to_string(i); },
            [] (const Box<Array>& i) { return convert_to_string(i); },
            [] (const Box<Dictionary>& i) { return convert_to_string(i); },
    }, _data);
}

std::optional<Box<BencodedData>> BencodedData::value(const std::string& key) const noexcept {
    return std::visit(Overload{
            [](const Integer &i) -> std::optional<Box<BencodedData>> { return std::nullopt; },
            [](const String &i) -> std::optional<Box<BencodedData>> { return std::nullopt; },
            [](const Box<Array> &i) -> std::optional<Box<BencodedData>> { return std::nullopt; },
            [&key](const Box<Dictionary> &i) -> std::optional<Box<BencodedData>> {
                auto v = (i->values()).find(key);
                if (v != i->values().cend()) {
                    return v->second;
                }
                return std::nullopt;
            }
    }, _data);
}

std::optional<long long> BencodedData::get_int() const noexcept {
    return std::visit(Overload{
            [](const Integer &i) -> std::optional<long long> { return i.value(); },
            [](const String &i) -> std::optional<long long> { return std::nullopt; },
            [](const Box<Array> &i) -> std::optional<long long> { return std::nullopt; },
            [](const Box<Dictionary> &i) -> std::optional<long long> { return std::nullopt; }
    }, _data);
}

std::optional<std::string> BencodedData::get_string() const noexcept {
    return std::visit(Overload{
            [](const Integer &i) -> std::optional<std::string> { return std::nullopt; },
            [](const String &i) -> std::optional<std::string> { return i.value(); },
            [](const Box<Array> &i) -> std::optional<std::string> { return std::nullopt; },
            [](const Box<Dictionary> &i) -> std::optional<std::string> { return std::nullopt; },
            [](const int& i) -> std::optional<std::string> { return std::nullopt; }
    }, _data);
}