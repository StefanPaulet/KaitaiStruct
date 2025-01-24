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
  using Types = std::unordered_map<std::string, CompoundType>;
  using TopLevelSequence = std::vector<CompoundType>;

  [[nodiscard]] auto getMeta() const -> Meta const&;
  auto setMeta(Meta const& other) -> void;
  [[nodiscard]] auto getSequence() const -> TopLevelSequence const&;
  auto setSequence(TopLevelSequence const& other) -> void;
  [[nodiscard]] auto getTypes() const -> Types const&;
  auto setTypes(Types const& other) -> void;

private:
  Meta _meta{};
  TopLevelSequence _sequence{};
  Types _compoundTypes{};
};

} // namespace kaitai::detail

