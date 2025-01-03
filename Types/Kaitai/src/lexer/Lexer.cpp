//
// Created by stefan on 1/3/2025.
//

#include "Lexer.hpp"

namespace kaitai::detail {
Lexer::Lexer(std::string&& string) : _str{std::move(string)}, _input{_str} {}

auto Lexer::operator()() -> std::optional<Token> {
  if (_input.empty()) {
    return std::nullopt;
  }

  if (isAlphaNum(_input.front())) {
    return handleAlphaNum();
  }
  if (isWhitespace(_input.front())) {
    return handleWhitespace();
  }

  switch (_input.front()) {
    using enum TokenType;
    case ':': {
    _input.remove_prefix(1);
      return Token{Colon};
    }
    case '-': {
    _input.remove_prefix(1);
      return Token{Dash};
    }
    default: {
      return std::nullopt;
    }
  }
}

auto Lexer::handleAlphaNum() -> std::optional<Token> {
  auto tokenStr = consumeWhile(isAlphaNum);
  if (tokenStr.has_value()) {
    using enum TokenType;
    auto val = tokenStr.value();
    if (val == "meta") {
      return Token{Meta};
    }
    if (val == "id") {
      return Token{Id};
    }
    if (val == "seq") {
      return Token{Seq};
    }
    if (val == "type") {
      return Token{Type};
    }
    return Token{Identifier, std::string{val}};
  }
  return std::nullopt;
}

auto Lexer::handleWhitespace() -> std::optional<Token> {
  using enum TokenType;
  if (_input.front() == ' ') {
    _input.remove_prefix(1);
    if (_input.empty()) {
      return std::nullopt;
    }
    if (_input.front() == ' ') {
      _input.remove_prefix(1);
      return Token{Tab};
    }
    return Token{Space};
  }

  switch (_input.front()) {
    case '\t': {
      _input.remove_prefix(1);
      return Token{Tab};
    }
    case '\n': {
      _input.remove_prefix(1);
      return Token{NewLine};
    }
    default: {
      return std::nullopt;
    }
  }
}

} // namespace kaitai::detail