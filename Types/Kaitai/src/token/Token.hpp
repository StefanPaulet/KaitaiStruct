//
// Created by stefan on 1/3/2025.
//

#pragma once

#include <cassert>
#include <format>
#include <iostream>
#include <optional>
#include <string>
#include <variant>

namespace kaitai::detail {

using TokenValue = std::variant<unsigned int, std::string>;

enum class TokenType {
  Identifier,
  Blank,
  Tab,
  NewLine,
  Dash,
  Colon,
  Meta,
  Id,
  Seq,
  Type
};

struct Token {
  constexpr explicit Token(TokenType tType) : type{tType}, value{std::nullopt} {}
  constexpr Token(TokenType tType, TokenValue const& tValue) : type{tType}, value{tValue} {}

  auto operator<=>(Token const& other) const = default;

  TokenType type;
  std::optional<TokenValue> value;
};
} // namespace kaitai::detail

template <> struct std::formatter<kaitai::detail::TokenType> {
  constexpr auto parse(auto& ctx) {
    auto it = ctx.begin();
    if (it == ctx.end()) {
      return it;
    }
    if (it != ctx.end() && *it != '}') {
      throw std::format_error("Invalid format args for Token.");
    }

    return it;
  }

  auto format(kaitai::detail::TokenType tokenType, format_context& ctx) const {
    using namespace kaitai::detail;
    auto toString = [](TokenType t) -> std::string {
      switch (t) {
        using enum TokenType;
        case Identifier: { return "identifier"; }
        case Blank: { return "<whitespace>"; }
        case Tab: { return "<tab>"; }
        case NewLine: { return "<newline>"; }
        case Dash: { return "'-'"; }
        case Colon: { return "':'"; }
        case Meta: { return "'meta'"; }
        case Id: { return "'id'"; }
        case Seq: { return "'seq'"; }
        case Type: { return "'type'"; }
        default: { assert(false && "Unknown token type"); }
      }
    };
    return formatter<std::string>{}.format(toString(tokenType), ctx);
  }
};

template <> struct std::formatter<kaitai::detail::Token> {
  constexpr auto parse(auto& ctx) {
    auto it = ctx.begin();
    if (it == ctx.end()) {
      return it;
    }
    if (it != ctx.end() && *it != '}') {
      throw std::format_error("Invalid format args for Token.");
    }

    return it;
  }

  auto format(kaitai::detail::Token const& token, format_context& ctx) const {
  using namespace kaitai::detail;
    auto toString = [](Token const& t) -> std::string {
      switch (t.type) {
        using enum TokenType;
        case Identifier: { return std::format("identifier['{}']", std::get<std::string>(t.value.value())); }
        case Blank:
        case Tab:
        case NewLine:
        case Dash:
        case Colon:
        case Meta:
        case Id:
        case Seq:
        case Type: { return std::format("{}", t.type);  }
        default: { assert(false && "Unknown token type"); }
      }
    };
    return formatter<std::string>{}.format(std::format("Token={}", toString(token)), ctx);
  }
};