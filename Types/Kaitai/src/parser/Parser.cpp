//
// Created by stefan on 1/22/2025.
//

#include "Parser.hpp"

namespace kaitai::detail {
Parser::Parser(InputStream& istream) : _istream(istream) {}

auto Parser::operator()() -> KaitaiStruct {
  KaitaiStruct kaitaiStruct{};
  kaitaiStruct.setMeta(parseMeta());
  return kaitaiStruct;
}

auto Parser::getToken(Lexer& lexer, InputStream& istream) noexcept(false) -> std::optional<Token> {
  auto nextToken = lexer();
  if (!nextToken.has_value()) {
    if (istream.eof()) {
      return std::nullopt;
    }
    std::string input(readChunkSize, '\0');
    istream.read(input.data(), readChunkSize);
    lexer.input(std::move(input));
    return lexer().value();
  }
  return nextToken.value();
}

auto Parser::parseMeta() -> Meta {
  Meta result{};
  using enum TokenType;
  _peek = getToken(_lexer, _istream).value();
  consumeHeader(Meta);

  auto updateField = [&result](Token const& token) {
    switch (token.type) {
      case Id: {
        result.id = std::get<1>(token.data());
        break;
      }
      case FileExt: {
        result.fileExtension = std::get<1>(token.data());
        break;
      }
      case Endian: {
        if (auto tokenData = std::get<1>(token.data()); tokenData == "le") {
          result.endian = Endian::LITTLE;
        } else {
          if (tokenData == "be") {
            result.endian = Endian::BIG;
          } else {
            throw exceptions::UnknownEndianException(tokenData);
          }
        }
        break;
      }
      default: {
        assert(false && "Unhandled entry for meta field");
      }
    }
  };

  while (true) {
    if (!_peek || _peek.value().type != Tab) {
      break;
    }
    _peek = getToken(_lexer, _istream).value();
    updateField(consumeEntry());
  }
  return result;
}

auto Parser::consumeToken(TokenType expected) noexcept(false) -> Token {
  if (!_peek) {
    throw exceptions::EndOfStreamException{};
  }
  auto peekValue = _peek.value();
  if (expected != peekValue.type) {
    throw exceptions::UnexpectedTokenException(expected, peekValue);
  }
  _peek = getToken(_lexer, _istream);
  return peekValue;
}

auto Parser::consumeHeader(TokenType expected) noexcept(false) -> void {
  consumeToken(expected);
  consumeToken(TokenType::Colon);
  consumeToken(TokenType::NewLine);
}

auto Parser::consumeEntry() noexcept(false) -> Token {
  if (!_peek) {
    throw exceptions::EndOfStreamException{};
  }
  auto entryType = _peek.value().type;
  _peek = getToken(_lexer, _istream);
  consumeToken(TokenType::Colon);
  consumeToken(TokenType::Blank);
  auto entryValue = consumeToken(TokenType::Identifier).data();
  consumeToken(TokenType::NewLine);
  return Token{entryType, entryValue};
}


}