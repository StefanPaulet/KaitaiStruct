//
// Created by stefan on 1/4/2025.
//

#pragma once

#include <kaitai/Lexer>
#include <span>
#include <vector>

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
constexpr auto fileExt = Token{FileExt};
constexpr auto endian = Token{Endian};
constexpr auto size = Token{Size};
constexpr auto types = Token{Types};
constexpr auto contents = Token{Contents};
constexpr auto intLit = [](unsigned int val) { return Token{IntLiteral, val}; };
constexpr auto strLit = [](std::string const& val) { return Token{StringLiteral, val}; };
constexpr auto identifier = [](TokenValue const& value) { return Token{Identifier, value}; };

using TokenStream = std::vector<Token>;

auto compareTokens(Lexer& lexer, TokenStream const& stream) -> bool {
  std::span remaining{stream.begin(), stream.size()};
  for (auto token = lexer(); token.has_value(); token = lexer()) {
    if (remaining.empty()) {
      std::cout << std::format("Found token '{}', but expected none\n", token.value());
      return false;
    }
    if (token != remaining.front()) {
      std::cout << std::format("Expected token '{}', but found '{}' instead\n", remaining.front(), token.value());
      if (auto matchCount = stream.size() - remaining.size(); matchCount != 0) {
        std::cout << "Following tokens matched:\n";
        for (auto const& matched : std::span{stream.begin(), matchCount}) {
          std::cout << std::format("{}\n", matched);
        }
      }
      return false;
    }
    remaining = remaining.subspan(1, remaining.size() - 1);
  }
  if (!remaining.empty()) {
    std::cout << std::format("Still expecting tokens (front is '{}'), but found none\n", remaining.front());
    return false;
  }

  return true;
}

} // namespace kaitai::testing