cmake_minimum_required(VERSION 3.28)

set(CMAKE_CXX_STANDARD 20)

project(kaitaiStruct)

add_executable(main
        src/main.cpp
        src/lexer/Lexer.cpp)
