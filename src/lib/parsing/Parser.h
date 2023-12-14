#ifndef BITTORRENT_STARTER_CPP_PARSER_H
#define BITTORRENT_STARTER_CPP_PARSER_H

#include <string>

#include "ParseResult.h"

template<typename T>
struct TextParserOutput {
    std::vector<std::tuple<T, std::string>> results;
};

template<typename T>
class Parser {
public:
    virtual ParseResult<TextParserOutput<T>, std::string> parse(std::string input) = 0;
    virtual ~Parser() = default;
};

class CharParser : public Parser<std::string> {
private:
    char _c;

public:
    explicit CharParser(char c) : _c{c} {}

    ParseResult<TextParserOutput<std::string>, std::string> parse(std::string input) override {
        if (input.starts_with(_c)) {
            auto output = TextParserOutput<std::string> {
                {std::make_tuple(std::string{_c}, input.substr(1))}
            };
            return ParseResult<TextParserOutput<std::string>, std::string>(output);
        }
        return ParseResult<TextParserOutput<std::string>, std::string>("");
    }
};

#endif //BITTORRENT_STARTER_CPP_PARSER_H
