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
    Exception(std::format("Expecting token of type '{}' but found '{}'", expected, actual)) {}
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
} // kaitai::exceptions