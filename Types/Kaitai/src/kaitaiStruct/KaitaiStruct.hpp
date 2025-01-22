//
// Created by stefan on 1/22/2025.
//

#pragma once

#include "meta/Meta.hpp"

namespace kaitai::detail {

class KaitaiStruct {
public:
  auto getMeta() const -> Meta const&;
  auto setMeta(Meta const& other) -> void;

private:
  Meta _meta;
};

} // namespace kaitai::detail

