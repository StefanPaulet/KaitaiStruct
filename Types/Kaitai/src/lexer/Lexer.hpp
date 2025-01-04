//
// Created by stefan on 1/3/2025.
//

#pragma once

#include "../token/Token.hpp"
#include "Exceptions.hpp"

namespace kaitai::detail {

class Lexer {
public:
  explicit Lexer(std::string&& string);
  auto operator()() -> std::optional<Token>;

private:
  static auto isAlpha(char chr) {
    return chr >= 'a' && chr <= 'z' || chr >= 'A' && chr <= 'Z' || chr == '_';
  }

  static auto isDigit(char chr) {
    return chr >= '0' && chr <= '9';
  }

  static auto isAlphaNum(char chr) {
    return isAlpha(chr) || isDigit(chr);
  }

  static auto isWhitespace(char chr) {
    return chr == ' ' || chr == '\t' || chr == '\n' || chr == '\r';
  }

  template <typename Pred>
  auto consumeWhile(Pred&& pred) -> std::optional<std::string_view> {
    unsigned int idx = 0;
    while (idx != _input.size() && std::invoke(std::forward<Pred>(pred), _input[idx])) {
      ++idx;
    }
    if (idx == _input.size()) {
      return std::nullopt;
    }
    auto retVal = _input.substr(0, idx);
    _input.remove_prefix(idx);
    return retVal;
  }

  auto handleAlphaNum() -> std::optional<Token>;
  auto handleWhitespace() -> std::optional<Token>;

  std::string _str;
  std::string_view _input;
};

} // namespace kaitai::detail
