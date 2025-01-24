//
// Created by stefan on 1/4/2025.
//

#pragma once

#include "../common/Exceptions.hpp"

namespace kaitai::exceptions {
class UnknownSymbolException : public detail::Exception {
public:
  explicit UnknownSymbolException(char c) : Exception(std::format("Unknown symbol '{}'", c)) {}
};
class UnmatchedSingleQuoteException : public detail::Exception {
public:
  UnmatchedSingleQuoteException() : Exception("Unmatched \"'\" symbol") {}
};
} // kaitai::exceptions