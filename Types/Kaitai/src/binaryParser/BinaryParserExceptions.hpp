//
// Created by stefan on 1/27/25.
//

#pragma once

#include <format>
#include "../common/Exceptions.hpp"

namespace kaitai::exceptions {
class MaximumSizeExceededException : public detail::Exception {
public:
  explicit MaximumSizeExceededException(unsigned int size) :
      Exception{std::format("Cannot have a size field larger than {}", size)} {}
};
} // namespace kaitai::exceptions