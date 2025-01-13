//
// Created by stefan on 1/4/2025.
//

#pragma once

#include <exception>
#include <string>

namespace kaitai::exceptions::detail {
class Exception : public std::exception {
public:
  explicit Exception(std::string_view message) : _message {message} {}

  [[nodiscard]] char const* what() const noexcept override { return _message.c_str(); }

protected:
  std::string _message {};
};
} // namespace kaitai::exceptions::detail