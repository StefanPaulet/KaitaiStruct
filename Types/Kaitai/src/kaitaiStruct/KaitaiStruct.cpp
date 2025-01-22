//
// Created by stefan on 1/22/2025.
//

#include "KaitaiStruct.hpp"

namespace kaitai::detail {

auto KaitaiStruct::getMeta() const -> Meta const& { return _meta; }

auto KaitaiStruct::setMeta(Meta const& other) -> void {
  _meta = other;
}


} // namespace kaitai::detail