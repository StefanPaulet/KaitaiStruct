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
    Exception(std::format("Expecting token of type {} but found {}", expected, actual)) {}
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

class TopLevelSeqTypeException : public detail::Exception {
public:
  explicit TopLevelSeqTypeException() : Exception("Main seq items must have type 'type'") {}
};

class UnexpectedSeqItemType : public detail::Exception {
public:
  explicit UnexpectedSeqItemType(TokenType type) :
  Exception(std::format("Unknown type {} in seq specification", type)) {}
};

class UnusedTypeException : public detail::Exception {
public:
  explicit UnusedTypeException(std::string_view id) :
    Exception(std::format("Type {} defined but not used in top-level seq specification", id)) {}
};

class ErroneousTypeDefinitionException : public detail::Exception {
public:
  explicit ErroneousTypeDefinitionException(std::string_view id) :
    Exception(std::format("Cannot define types using user-defined types (field={})", id)) {}
};

class UnalignedEntryException : public detail::Exception {
public:
  explicit UnalignedEntryException(TokenType type) :
    Exception(std::format("Missing indentation on token {}", type)) {}

};

class PositionalException : public detail::Exception {
public:
  PositionalException(Exception const& baseException, std::string_view position) :
    Exception(std::format("{} at:\n {}", baseException.what(), position)) {}
};
} // kaitai::exceptions