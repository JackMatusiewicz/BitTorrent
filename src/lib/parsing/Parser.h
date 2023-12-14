#ifndef BITTORRENT_STARTER_CPP_PARSER_H
#define BITTORRENT_STARTER_CPP_PARSER_H

#include <string>

#include "ParseResult.h"

template<typename T>
class Parser {
public:
    virtual ParseResult<std::tuple<T, std::string>, std::string> parse(std::string input) = 0;
};

#endif //BITTORRENT_STARTER_CPP_PARSER_H
