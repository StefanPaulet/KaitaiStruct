//
// Created by stefan on 1/22/2025.
//

#include "Parser.hpp"

namespace kaitai::detail {
Parser::Parser(InputStream& istream) : _istream(istream) {}

auto Parser::operator()() -> KaitaiStruct {
  KaitaiStruct kaitaiStruct{};

  try {
    kaitaiStruct.setMeta(parseMeta());

    auto sequence = parseSequence();
    KaitaiStruct::Types compoundTypes {};
    for (auto const& chunk : sequence) {
      compoundTypes.emplace(std::get<1>(chunk).name, CompoundType{});
    }
    kaitaiStruct.setSequence(sequence);

    parseTypes(compoundTypes);
    kaitaiStruct.setTypes(compoundTypes);
  } catch (exceptions::detail::Exception const& e) {
    throw exceptions::PositionalException{e, _lexer.leftoverString()};
  }

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

  while (consumeIndent() != 0) {
    auto [entryType, data] = consumeEntry();
    switch (entryType) {
      case Id: {
        result.id = std::get<std::string>(data);
        break;
      }
      case FileExt: {
        result.fileExtension = std::get<std::string>(data);
        break;
      }
      case Endian: {
        if (auto tokenData = std::get<std::string>(data); tokenData == "le") {
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
  }
  return result;
}

auto Parser::parseSequence() noexcept(false) -> KaitaiStruct::MainSequence {
  KaitaiStruct::MainSequence result{};
  using enum TokenType;
  consumeHeader(Seq);

  while (consumeIndent() != 0) {
    auto newChunk = consumeSeqItem(2);
    if (!std::holds_alternative<UserDefinedType>(newChunk)) {
      throw exceptions::MainSeqTypeException{};
    }
    result.push_back(std::move(std::get<UserDefinedType>(newChunk)));
  }

  return result;
}

auto Parser::parseTypes(KaitaiStruct::Types& types) noexcept(false) -> void {
  using enum TokenType;
  consumeHeader(Types);

  _lastIndent = consumeIndent();
  while (_lastIndent != 0) {
    auto currentType = consumeToken(Identifier).stringData();
    consumeToken(Colon);
    consumeToken(NewLine);
    if (!types.contains(currentType)) {
      throw exceptions::UnusedTypeException{currentType};
    }
    types[currentType].seq = consumeUserDefinedTypeEntry();
  }
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

auto Parser::consumeValueToken() noexcept(false) -> Token {
  using enum TokenType;
  if (!_peek) {
    throw exceptions::EndOfStreamException{};
  }
  auto peekValue = _peek.value();
  if (peekValue.type != IntLiteral && peekValue.type != StringLiteral
    && peekValue.type != Identifier) {
    throw exceptions::NonValueTokenException(peekValue.type);
  }
  _peek = getToken(_lexer, _istream);
  return peekValue;
}

auto Parser::consumeHeader(TokenType expected) noexcept(false) -> void {
  using enum TokenType;
  consumeToken(expected);
  consumeToken(Colon);
  consumeToken(NewLine);
}

auto Parser::consumeEntry() noexcept(false) -> std::tuple<TokenType, TokenValue> {
  using enum TokenType;

  if (!_peek) {
    throw exceptions::EndOfStreamException{};
  }
  auto entryType = _peek.value().type;
  _peek = getToken(_lexer, _istream);
  consumeToken(Colon);
  consumeToken(Blank);
  auto entryValue = consumeValueToken();
  consumeToken(NewLine);
  return {entryType, entryValue.data()};
}

auto Parser::consumeSeqItem(unsigned int indent) noexcept(false) -> std::variant<Chunk, UserDefinedType> {
  using enum TokenType;

  consumeToken(Dash);
  consumeToken(Blank);
  consumeToken(Id);
  consumeToken(Colon);
  consumeToken(Blank);
  auto typeName = consumeToken(Identifier).stringData();
  consumeToken(NewLine);

  if (indent != consumeIndent()) {
    throw exceptions::MisalignedEntryException{_peek.value().type, indent, _lastIndent};
  }

  auto [entryType, data] = consumeEntry();
  switch (entryType) {
    case Type: {
      if (auto it = _rawTypes.find(std::get<std::string>(data));
        it != _rawTypes.end()) {
        return Chunk{typeName, it->second};
      }
      return KaitaiStruct::NamedType{typeName, CompoundType{std::get<std::string>(data), {}}};
    }
    case Contents: {
      return Chunk{typeName, std::get<std::string>(data)};
    }
    case Size: {
      return Chunk{typeName, std::get<unsigned int>(data)};
    }
    default: {
      throw exceptions::UnexpectedSeqItemType{entryType};
    }
  }
}

auto Parser::consumeIndent() noexcept(false) -> unsigned int {
  unsigned int indent = 0;
  while (_peek && _peek.value().type == TokenType::Tab) {
    ++indent;
    _peek = getToken(_lexer, _istream);
  }
  _lastIndent = indent;
  return indent;
}

auto Parser::consumeUserDefinedTypeEntry() noexcept(false) -> Sequence {
  using enum TokenType;

  Sequence result{};

  if (consumeIndent() != 2) {
    throw exceptions::MisalignedEntryException{_peek.value().type, 2, _lastIndent};
  }

  consumeHeader(Seq);
  consumeIndent();
  while (_lastIndent == 3) {
    auto item = consumeSeqItem(4);
    if (std::holds_alternative<UserDefinedType>(item)) {
      throw exceptions::ErroneousTypeDefinitionException{std::get<0>(std::get<UserDefinedType>(item))};
    }
    result.addChunk(std::move(std::get<Chunk>(item)));
    consumeIndent();
  }

  return result;
}


}