//
// Created by stefan on 1/28/2025.
//

#include <gtest/gtest.h>
#include "utils/BinaryParserMock.hpp"

namespace {
using namespace kaitai::testing;

using kaitai::detail::Parser;
using kaitai::detail::KaitaiStruct;
}

TEST(BinaryParserTest, OverallLittleEndian) {
  std::stringstream str {R"(meta:
  endian: le
seq:
  - id: t1
    type: t1
types:
  t1:
    seq:
      - id: t1Content
        contents: 'T1'
      - id: t2Size
        size: 3
      - id: t3Type
        type: u2
)"};
  Parser parser {str};
  try {
    auto kstruct = parser();
    BinaryParser binaryParser{kstruct};
    std::stringstream test{"T1abc\x64\x65"};
    auto result = binaryParser(test);
    ASSERT_TRUE(compareBinaries(result,
      parsedData(
        header("t1", content("t1Content", true), size("t2Size", "abc"), type("t3Type", 0x6465))
      )
    ));
  } catch (...) {
    ASSERT_FALSE("Functional code throws");
  }
}

TEST(BinaryParserTest, OverallBigEndian) {
  std::stringstream str {R"(meta:
  endian: be
seq:
  - id: t1
    type: t1
types:
  t1:
    seq:
      - id: t1Content
        contents: 'T1'
      - id: t2Size
        size: 3
      - id: t3Type
        type: u2
)"};
  Parser parser {str};
  try {
    auto kstruct = parser();
    BinaryParser binaryParser{kstruct};
    std::stringstream test{"T1abc\x64\x65"};
    auto result = binaryParser(test);
    ASSERT_TRUE(compareBinaries(result,
      parsedData(
        header("t1", content("t1Content", false), size("t2Size", "cba"), type("t3Type", 0x6564))
      )
    ));
  } catch (...) {
    ASSERT_FALSE("Functional code throws");
  }
}