//
// Created by stefan on 1/4/2025.
//

#include "utils/LexerMock.hpp"
#include <gtest/gtest.h>

namespace {
using namespace kaitai::testing;
} // namespace

TEST(LexerTest, Test1) {
  std::string input = R"(meta:
  id: gif
seq:
  - type: header
)";
  ASSERT_TRUE(
    compareTokens(
      std::move(input),
      TokenStream{
        meta, colon, newline,
        tab, id, colon, blank, identifier("gif"), newline,
        seq, colon, newline,
        tab, dash, blank, type, colon, blank, identifier("header"), newline}));
}