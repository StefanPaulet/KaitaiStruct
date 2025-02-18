//
// Created by stefan on 1/22/2025.
//

#pragma once

#include <string>

namespace kaitai::detail {

enum class Endian {
  LITTLE, BIG
};

struct Meta {
  auto operator<=>(Meta const& other) const = default;

  std::string id;
  std::string fileExtension;
  Endian endian;
};

} // namespace kaitai::detail
