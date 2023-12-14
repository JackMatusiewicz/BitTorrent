cmake_minimum_required(VERSION 3.13)
project(bittorrent-starter-cpp)
set(CMAKE_CXX_STANDARD 20) # Enable the C++20 standard
set(SOURCE_FILES src/Main.cpp src/lib/nlohmann/json.hpp
        src/lib/memory/Box.h
        src/lib/parsing/BencodedData.h src/lib/parsing/Parser.h src/lib/parsing/ParseResult.h)
add_executable(bittorrent ${SOURCE_FILES})
