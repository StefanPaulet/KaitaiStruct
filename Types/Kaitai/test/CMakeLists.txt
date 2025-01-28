include(FetchContent)

FetchContent_Declare(googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG 9b12f74)
FetchContent_GetProperties(googletest)
if(NOT googletest_POPULATED)
    FetchContent_MakeAvailable(googletest)
    add_library(lib.gtest
            ${googletest_SOURCE_DIR}/googletest/src/gtest-all.cc)
    target_include_directories(lib.gtest
            PUBLIC ${googletest_SOURCE_DIR}/googletest/include/
            PRIVATE ${googletest_SOURCE_DIR}/googletest)
endif()

add_executable(test
    test.cpp
    lexerTest.cpp
    parserTest.cpp
    binaryParserTest.cpp)

target_link_libraries(test
    PRIVATE
    lib.gtest
    KaitaiStruct)

target_include_directories(test PRIVATE ../include)