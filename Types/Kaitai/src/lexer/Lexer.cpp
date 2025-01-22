//
// Created by stefan on 1/3/2025.
//

#include "Lexer.hpp"

namespace kaitai::detail {
Lexer::Lexer() = default;

Lexer::Lexer(std::string&& string) : _str{std::move(string)}, _input{_str} {}

auto Lexer::operator()() -> std::optional<Token> {
  if (_input.empty() || _input.front() == '\0') {
    return std::nullopt;
  }

  if (isAlphaNum(_input.front())) {
    std::optional<Token> token;
    std::tie(_input, token) = handleAlphaNum(_input);
    return token;
  }
  if (isWhitespace(_input.front())) {
    std::optional<Token> token;
    std::tie(_input, token) = handleWhitespace(_input);
    return token;
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
      throw exceptions::UnknownSymbolException{_input.front()};
    }
  }
}

auto Lexer::handleAlphaNum(std::string_view input) -> std::tuple<std::string_view, std::optional<Token>> {
  auto [result, tokenStr] = consumeWhile(input, isAlphaNum);
  if (tokenStr.has_value()) {
    using enum TokenType;
    auto val = tokenStr.value();
    if (val == "meta") {
      return {result, Token{Meta}};
    }
    if (val == "id") {
      return {result, Token{Id}};
    }
    if (val == "seq") {
      return {result, Token{Seq}};
    }
    if (val == "type") {
      return {result, Token{Type}};
    }
    if (val == "file-extension") {
      return {result, Token{FileExt}};
    }
    if (val == "endian") {
      return {result, Token{Endian}};
    }
    if (val == "size") {
      return {result, Token{Size}};
    }
    return {result, Token{Identifier, std::string{val}}};
  }
  return {result, std::nullopt};
}

auto Lexer::handleWhitespace(std::string_view input) -> std::tuple<std::string_view, std::optional<Token>> {
  using enum TokenType;
  if (input.front() == ' ') {
    input.remove_prefix(1);
    if (input.empty()) {
      return {input, std::nullopt};
    }
    if (input.front() == ' ') {
      input.remove_prefix(1);
      return {input, Token{Tab}};
    }
    return {input, Token{Blank}};
  }

  switch (input.front()) {
    case '\t': {
      input.remove_prefix(1);
      return {input, Token{Tab}};
    }
    case '\n': {
      input.remove_prefix(1);
      return {input, Token{NewLine}};
    }
    default: {
      assert(false && "Unhandled whitespace symbol");
    }
  }
}

auto Lexer::input(std::string&& string) -> void {
  _str = std::string(_input) + std::move(string);
  _input = _str;
}


auto Lexer::isAlpha(char chr) -> bool {
  return chr >= 'a' && chr <= 'z' || chr >= 'A' && chr <= 'Z' || chr == '_' || chr == '-';
}

auto Lexer::isDigit(char chr) -> bool {
  return chr >= '0' && chr <= '9';
}

auto Lexer::isAlphaNum(char chr) -> bool {
  return isAlpha(chr) || isDigit(chr);
}

auto Lexer::isWhitespace(char chr) -> bool {
  return chr == ' ' || chr == '\t' || chr == '\n' || chr == '\r';
}
} // namespace kaitai::detail