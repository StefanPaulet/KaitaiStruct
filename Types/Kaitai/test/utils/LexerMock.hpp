//
// Created by stefan on 1/4/2025.
//

#pragma once

#include <kaitai/Lexer>
#include <deque>
#include <iostream>

namespace kaitai::testing {
using kaitai::detail::Token;
using kaitai::detail::TokenType;
using kaitai::detail::TokenValue;
using kaitai::detail::Lexer;

using enum TokenType;

constexpr auto blank = Token{Blank};
constexpr auto tab = Token{Tab};
constexpr auto newline = Token{NewLine};
constexpr auto dash = Token{Dash};
constexpr auto colon = Token{Colon};
constexpr auto meta = Token{Meta};
constexpr auto id = Token{Id};
constexpr auto seq = Token{Seq};
constexpr auto type = Token{Type};
constexpr auto identifier = [](TokenValue const& value) { return Token{Identifier, value}; };

using TokenStream = std::deque<Token>;

auto compareTokens(std::string&& string, TokenStream&& stream) -> bool {
  Lexer lexer {std::move(string)};
  for (auto token = lexer(); token.has_value(); token = lexer()) {
    if (stream.empty()) {
      std::cout << std::format("Found token '{}', but expected none\n", token.value());
      return false;
    }
    if (token != stream.front()) {
      std::cout << std::format("Expected token '{}', but found '{}' instead\n", stream.front(), token.value());
      return false;
    }
    stream.pop_front();
  }
  if (!stream.empty()) {
    std::cout << std::format("Still expecting tokens (front is '{}'), but found none\n", stream.front());
    return false;
  }

  return true;
}

} // namespace kaitai::testing