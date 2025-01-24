//
// Created by stefan on 1/22/2025.
//

#include "KaitaiStruct.hpp"

namespace kaitai::detail {

auto KaitaiStruct::getMeta() const -> Meta const& { return _meta; }

auto KaitaiStruct::setMeta(Meta const& other) -> void {
  _meta = other;
}

auto KaitaiStruct::getSequence() const -> MainSequence const& { return _sequence; }

auto KaitaiStruct::setSequence(MainSequence const& other) -> void {
  _sequence = other;
}

auto KaitaiStruct::getTypes() const -> Types const& { return _compoundTypes; }

auto KaitaiStruct::setTypes(Types const& other) -> void {
  _compoundTypes = other;
}


} // namespace kaitai::detail