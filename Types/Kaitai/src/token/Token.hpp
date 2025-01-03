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

  friend auto operator<<(std::ostream& os, Token const& t) -> std::ostream& {
    switch (t.type) {
      case TokenType::Identifier: {
        os << std::format("'Identifier: {}'", std::get<std::string>(t.value.value()));
        break;
      }
      case TokenType::Space: {
        os << "<whitspace>";
        break;
      }
      case TokenType::Tab: {
        os << "<tab>";
        break;
      }
      case TokenType::NewLine: {
        os << "<newline>";
        break;
      }
      case TokenType::Dash: {
        os << "'-'";
        break;
      }
      case TokenType::Colon: {
        os << "':'";
        break;
      }
      case TokenType::Meta: {
        os << "'meta'";
        break;
      }
      case TokenType::Id: {
        os << "'id'";
        break;
      }
      case TokenType::Seq: {
        os << "'seq'";
        break;
      }
      case TokenType::Type: {
        os << "'type'";
        break;
      }
    }
    return os;
  }

  TokenType type;
  std::optional<TokenValue> value;
};
} // namespace kaitai::detail



template <> struct std::formatter<kaitai::detail::Token> {
  bool quoted = false;

  constexpr auto parse(auto& ctx) {
    auto it = ctx.begin();
    if (it == ctx.end())
      return it;
    if (it != ctx.end() && *it != '}')
      throw std::format_error("Invalid format args for Token.");

    return it;
  }

  auto format(kaitai::detail::Token const& token, format_context& ctx) const {
  using namespace kaitai::detail;
    auto toString = [](Token const& t) -> std::string {
      switch (t.type) {
        using enum TokenType;
        case Identifier: { return std::format("identifier['{}']", std::get<std::string>(t.value.value())); }
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
    return formatter<std::string>{}.format(std::format("Token={}", toString(token)), ctx);
  }
};