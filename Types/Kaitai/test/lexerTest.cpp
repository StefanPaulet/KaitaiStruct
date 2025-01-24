//
// Created by stefan on 1/4/2025.
//

#include "utils/LexerMock.hpp"
#include <gtest/gtest.h>

namespace {
using namespace kaitai::testing;
using namespace kaitai::exceptions;
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

TEST(LexerTest, FileExt) {
  Lexer lexer{"file-extension\n"};
  ASSERT_TRUE(compareTokens(lexer, TokenStream{fileExt, newline}));
}

TEST(LexerTest, Endian) {
  Lexer lexer{"endian\n"};
  ASSERT_TRUE(compareTokens(lexer, TokenStream{endian, newline}));
}

TEST(LexerTest, Size) {
  Lexer lexer{"size\n"};
  ASSERT_TRUE(compareTokens(lexer, TokenStream{size, newline}));
}

TEST(LexerTest, Types) {
  Lexer lexer{"types\n"};
  ASSERT_TRUE(compareTokens(lexer, TokenStream{types, newline}));
}

TEST(LexerTest, Contents) {
  Lexer lexer{"contents\n"};
  ASSERT_TRUE(compareTokens(lexer, TokenStream{contents, newline}));
}

TEST(LexerTest, IntLiteral) {
  Lexer lexer{"123\n"};
  ASSERT_TRUE(compareTokens(lexer, TokenStream{intLit(123), newline}));
}

TEST(LexerTest, StringLiteral) {
  Lexer lexer{"'abcde'\n"};
  ASSERT_TRUE(compareTokens(lexer, TokenStream{strLit("abcde"), newline}));
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

TEST(LexerTest, UnknownSymbol) {
  Lexer lexer{"$"};
  try {
    lexer();
  } catch (UnknownSymbolException const& e) {
    ASSERT_EQ(std::string_view(e.what()), "Unknown symbol '$'");
  }
}

TEST(LexerTest, ContinuousInput) {
  Lexer lexer{"id: meta"};
  ASSERT_TRUE(compareTokens(lexer, TokenStream{id, colon, blank}));
  lexer.input("l\n");
  ASSERT_TRUE(compareTokens(lexer, TokenStream{identifier("metal"), newline}));
}