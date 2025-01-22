//
// Created by stefan on 1/22/2025.
//

#pragma once

#include "../lexer/Lexer.hpp"
#include "../kaitaiStruct/KaitaiStruct.hpp"
#include "ParserExceptions.hpp"

#include <iostream>

namespace kaitai::detail {
class Parser {
private:
  using InputStream = std::basic_istream<char>;

public:
  explicit Parser(InputStream& istream);
  auto operator()() noexcept(false) -> KaitaiStruct;

private:
  [[nodiscard]] static auto getToken(Lexer& lexer, InputStream& istream) noexcept(false) -> std::optional<Token>;
  auto parseMeta() noexcept(false) -> Meta;
  auto consumeToken(TokenType expected) noexcept(false) -> Token;
  auto consumeHeader(TokenType expected) noexcept(false) -> void;
  auto consumeEntry() noexcept(false) -> Token;

  static constexpr auto readChunkSize = 1024;
  InputStream& _istream;
  std::optional<Token> _peek{TokenType::Blank};
  Lexer _lexer{};
};

} // namespace kaitai::detail
