cmake_minimum_required(VERSION 3.28)

set(CMAKE_CXX_STANDARD 20)

project(kaitaiStruct)

add_library(KaitaiStruct
    STATIC
    src/lexer/Lexer.cpp
    src/parser/Parser.cpp
    src/kaitaiStruct/KaitaiStruct.cpp
    src/kaitaiStruct/meta/Meta.cpp)

add_executable(main src/main.cpp)

target_link_libraries(main KaitaiStruct)

target_include_directories(main PRIVATE include)

add_subdirectory(test)