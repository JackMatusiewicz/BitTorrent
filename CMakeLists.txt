cmake_minimum_required(VERSION 3.13)
project(bittorrent-starter-cpp)
set(CMAKE_CXX_STANDARD 20) # Enable the C++20 standard
set(SOURCE_FILES
        src/Main.cpp
        src/lib/nlohmann/json.hpp
        src/lib/memory/Box.h
        src/lib/bencode/BencodedData.h
        src/lib/bencode/decoder/BencodeDecoder.h src/lib/bencode/decoder/BencodeDecoder.cpp src/lib/bencode/BencodeData.cpp src/lib/info/MetaInfo.h src/lib/visit/Overload.h src/lib/tinySHA/TinySHA1.hpp src/lib/bencode/encoder/BencodeEncoder.h src/lib/bencode/encoder/BencodeEncoder.cpp src/lib/info/MetaInfo.cpp
        src/lib/http/httplib.h
        src/lib/peers/peer.h
        src/lib/peers/peer_discovery.h
        src/lib/peers/peer_discovery.cpp
        src/lib/optional/optional_ext.h)
add_executable(bittorrent ${SOURCE_FILES})
