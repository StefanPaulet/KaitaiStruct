//
// Created by stefan on 1/22/2025.
//

#pragma once

#include "../lexer/Lexer.hpp"
#include "../kaitaiStruct/KaitaiStruct.hpp"
#include "ParserExceptions.hpp"

#include <iostream>

namespace kaitai::detail {
class Parser {
private:
  using InputStream = std::basic_istream<char>;

public:
  explicit Parser(InputStream& istream);
  auto operator()() noexcept(false) -> KaitaiStruct;

private:
  [[nodiscard]] static auto getToken(Lexer& lexer, InputStream& istream) noexcept(false) -> std::optional<Token>;

  auto parseMeta() noexcept(false) -> Meta;
  auto parseSequence() noexcept(false) -> KaitaiStruct::TopLevelSequence;
  auto parseTypes(KaitaiStruct::Types& types) noexcept(false) -> void;

  auto consumeToken(TokenType expected) noexcept(false) -> Token;
  auto consumeValueToken() noexcept(false) -> Token;
  auto consumeHeader(TokenType expected) noexcept(false) -> void;
  auto consumeEntry() noexcept(false) -> std::tuple<TokenType, TokenValue>;
  auto consumeSeqItem(unsigned int indent) noexcept(false) -> std::variant<Chunk, CompoundType>;
  auto consumeIndent() noexcept(false) -> unsigned int;
  auto consumeUserDefinedTypeEntry() noexcept(false) -> Sequence;

  static constexpr auto readChunkSize = 1024;
  InputStream& _istream;
  std::optional<Token> _peek{TokenType::Blank};
  Lexer _lexer{};
  unsigned int _lastIndent{};
  static inline std::unordered_map<std::string, Chunk::RawType> _rawTypes = {
    {"u1", Chunk::RawType::U1}, {"u2", Chunk::RawType::U2}
  };
};

} // namespace kaitai::detail
