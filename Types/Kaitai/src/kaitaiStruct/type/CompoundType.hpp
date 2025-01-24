//
// Created by stefan on 1/23/2025.
//

#pragma once

#include "../sequence/Sequence.hpp"
#include <string>

namespace kaitai::detail {
class Sequence;

struct CompoundType {
  std::string name;
  Sequence seq;
};
} // namespace kaitai::detail