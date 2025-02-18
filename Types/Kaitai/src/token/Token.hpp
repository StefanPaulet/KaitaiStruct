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
  Type,
  FileExt,
  Endian,
  Size,
  Types,
  Contents,

  IntLiteral,
  StringLiteral
};

class Token {
public:
  constexpr explicit Token(TokenType tType) : type{tType}, _data{std::nullopt} {}
  constexpr Token(TokenType tType, TokenValue const& tValue) : type{tType}, _data{tValue} {
    assert((tType == TokenType::Identifier || tType == TokenType::IntLiteral || tType == TokenType::StringLiteral)
      && "Bad token type for a valued token");
  }

  auto operator<=>(Token const& other) const = default;

  auto data() const { return _data.value(); }
  auto stringData() const { return std::get<std::string>(_data.value()); }
  auto intData() const { return std::get<unsigned int>(_data.value()); }

  TokenType type;

private:
  std::optional<TokenValue> _data;
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
        case FileExt: { return "'file-extension'"; }
        case Endian: { return "'endian'"; }
        case Size: { return "'size'"; }
        case Types: { return "'types'"; }
        case Contents: { return "'contents'"; }
        case IntLiteral: { return "<int_literal>"; }
        case StringLiteral: { return "<string_literal>"; }
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
        case Identifier: { return std::format("identifier['{}']", t.stringData()); }
        case Blank:
        case Tab:
        case NewLine:
        case Dash:
        case Colon:
        case Meta:
        case Id:
        case Seq:
        case Type:
        case Types:
        case Contents:
        case FileExt:
        case Endian:
        case Size: { return std::format("{}", t.type);  }
        case IntLiteral: { return std::format("{}[{}]", t.type, t.intData()); }
        case StringLiteral: { return std::format("{}[{}]", t.type, t.stringData()); }
        default: { assert(false && "Unknown token type"); }
      }
    };
    return formatter<std::string>{}.format(std::format("Token={}", toString(token)), ctx);
  }
};