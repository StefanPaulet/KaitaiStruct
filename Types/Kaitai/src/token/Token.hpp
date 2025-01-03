//
// Created by stefan on 1/3/2025.
//

#pragma once

#include <iostream>
#include <format>
#include <optional>
#include <string>
#include <variant>

namespace kaitai::detail {

using TokenValue = std::variant<unsigned int, std::string>;

enum class TokenType {
  Identifier,
  Space,
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
  explicit Token(TokenType tType) : type{tType}, value{std::nullopt} {}
  Token(TokenType tType, TokenValue const& tValue) : type{tType}, value{tValue} {}

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