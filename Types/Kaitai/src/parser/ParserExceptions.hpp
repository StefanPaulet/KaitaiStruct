//
// Created by stefan on 1/22/2025.
//

#pragma once

#include "../common/Exceptions.hpp"
#include <format>

namespace kaitai::exceptions {
using kaitai::detail::TokenType;
using kaitai::detail::Token;

class UnexpectedTokenException : public detail::Exception {
public:
  UnexpectedTokenException(TokenType expected, Token const& actual) :
    Exception(std::format("Expecting token {} but found {}", expected, actual)) {}
};

class EndOfStreamException : public detail::Exception {
public:
  EndOfStreamException() : Exception("Stream terminated") {}
};

class UnknownEndianException : public detail::Exception {
public:
  explicit UnknownEndianException(std::string endianess) :
    Exception(std::format("Unknown endianness {}", endianess)) {}
};

class MainSeqTypeException : public detail::Exception {
public:
  explicit MainSeqTypeException() : Exception("Main 'seq' items must have type 'type'") {}
};

class UnexpectedSeqItemType : public detail::Exception {
public:
  explicit UnexpectedSeqItemType(TokenType type) :
  Exception(std::format("Unknown type {} in 'seq' specification", type)) {}
};

class UnusedTypeException : public detail::Exception {
public:
  explicit UnusedTypeException(std::string_view id) :
    Exception(std::format("Type {} defined but not used in top-level 'seq' specification", id)) {}
};

class ErroneousTypeDefinitionException : public detail::Exception {
public:
  explicit ErroneousTypeDefinitionException(std::string_view id) :
    Exception(std::format("User-defined types may not appear in anot types definition (field={})", id)) {}
};

class MisalignedEntryException : public detail::Exception {
public:
  explicit MisalignedEntryException(TokenType type, unsigned int expected, unsigned int actual) :
    Exception(std::format("Bad indentation on token {}: expected indentation {} but has {}", type, expected, actual)) {}

};

class NonValueTokenException : public detail::Exception {
public:
  explicit NonValueTokenException(TokenType type) :
    Exception(std::format("Entry items must be an 'identifier', <string_literal> or <int_literal>, not {}", type)) {}
};

class PositionalException : public detail::Exception {
public:
  PositionalException(Exception const& baseException, std::string_view position) :
    Exception(std::format("{} at:\n {}", baseException.what(), position)) {}
};
} // kaitai::exceptions