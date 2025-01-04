//
// Created by stefan on 1/4/2025.
//

#include "utils/LexerMock.hpp"
#include <gtest/gtest.h>

namespace {
using namespace kaitai::testing;
} // namespace

TEST(LexerTest, Blank) {
  Lexer lexer{" \n"};
  ASSERT_TRUE(compareTokens(lexer, TokenStream{blank, newline}));
}

TEST(LexerTest, Tab) {
  Lexer lexer{"\t"};
  ASSERT_TRUE(compareTokens(lexer, TokenStream{tab}));
}

TEST(LexerTest, Newline) {
  Lexer lexer{"\n"};
  ASSERT_TRUE(compareTokens(lexer, TokenStream{newline}));
}

TEST(LexerTest, Dash) {
  Lexer lexer{"-"};
  ASSERT_TRUE(compareTokens(lexer, TokenStream{dash}));
}

TEST(LexerTest, Colon) {
  Lexer lexer{":"};
  ASSERT_TRUE(compareTokens(lexer, TokenStream{colon}));
}

TEST(LexerTest, Meta) {
  Lexer lexer{"meta\n"};
  ASSERT_TRUE(compareTokens(lexer, TokenStream{meta, newline}));
}

TEST(LexerTest, Id) {
  Lexer lexer{"id\n"};
  ASSERT_TRUE(compareTokens(lexer, TokenStream{id, newline}));
}

TEST(LexerTest, Seq) {
  Lexer lexer{"seq\n"};
  ASSERT_TRUE(compareTokens(lexer, TokenStream{seq, newline}));
}

TEST(LexerTest, Type) {
  Lexer lexer{"type\n"};
  ASSERT_TRUE(compareTokens(lexer, TokenStream{type, newline}));
}

TEST(LexerTest, TabOfSpaces) {
  Lexer lexer{"  "};
  ASSERT_TRUE(compareTokens(lexer, TokenStream{tab}));
}

TEST(LexerTest, General) {
  Lexer lexer{R"(meta:
  id: gif
seq:
  - type: header
)"};
  ASSERT_TRUE(
    compareTokens(
      lexer,
      TokenStream{
        meta, colon, newline,
        tab, id, colon, blank, identifier("gif"), newline,
        seq, colon, newline,
        tab, dash, blank, type, colon, blank, identifier("header"), newline}));
}