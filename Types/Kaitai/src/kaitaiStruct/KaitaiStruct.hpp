//
// Created by stefan on 1/22/2025.
//

#pragma once

#include <unordered_map>

#include "Meta.hpp"
#include "type/CompoundType.hpp"

namespace kaitai::detail {

class KaitaiStruct {
public:
  using NamedType = std::tuple<std::string, CompoundType>;
  using Types = std::unordered_map<std::string, CompoundType>;
  using MainSequence = std::vector<NamedType>;

  [[nodiscard]] auto getMeta() const -> Meta const&;
  auto setMeta(Meta const& other) -> void;
  [[nodiscard]] auto getSequence() const -> MainSequence const&;
  auto setSequence(MainSequence const& other) -> void;
  [[nodiscard]] auto getTypes() const -> Types const&;
  auto setTypes(Types const& other) -> void;

private:
  Meta _meta{};
  MainSequence _sequence{};
  Types _compoundTypes{};
};

} // namespace kaitai::detail

