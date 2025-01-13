//
// Created by stefan on 1/3/2025.
//

#pragma once

#include "../token/Token.hpp"
#include "LexerExceptions.hpp"

namespace kaitai::detail {

class Lexer {
public:
  explicit Lexer(std::string&& string);
  auto operator()() -> std::optional<Token>;
  auto input(std::string&& newString) -> void;

private:
  static auto isAlpha(char chr) -> bool;

  static auto isDigit(char chr) -> bool;

  static auto isAlphaNum(char chr) -> bool;

  static auto isWhitespace(char chr) -> bool;

  template <typename Pred>
  static auto consumeWhile(std::string_view input, Pred&& pred) -> std::tuple<std::string_view, std::optional<std::string_view>> {
    unsigned int idx = 0;
    while (idx != input.size() && std::invoke(std::forward<Pred>(pred), input[idx])) {
      ++idx;
    }
    if (idx == input.size()) {
      return {input, std::nullopt};
    }
    auto retVal = input.substr(0, idx);
    input.remove_prefix(idx);
    return {input, retVal};
  }

  static auto handleAlphaNum(std::string_view input) -> std::tuple<std::string_view, std::optional<Token>>;
  static auto handleWhitespace(std::string_view input) -> std::tuple<std::string_view, std::optional<Token>>;

  std::string _str;
  std::string_view _input;
};

} // namespace kaitai::detail
