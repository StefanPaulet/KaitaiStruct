//
// Created by stefan on 1/22/2025.
//

#pragma once

#include <unordered_map>

#include "Meta.hpp"
#include "sequence/Sequence.hpp"

namespace kaitai::detail {

class KaitaiStruct {
public:
  using NamedType = std::tuple<std::string, std::string>;
  using MainSequence = std::vector<NamedType>;
  using TypesMap = std::unordered_map<std::string, CompoundType>;

  [[nodiscard]] auto getMeta() const -> Meta const&;
  auto setMeta(Meta const& other) -> void;
  [[nodiscard]] auto getSequence() const -> MainSequence const&;
  auto setSequence(MainSequence const& other) -> void;
  [[nodiscard]] auto getTypes() const -> TypesMap const&;
  auto setTypes(TypesMap const& other) -> void;

private:
  Meta _meta{};
  MainSequence _sequence{};
  TypesMap _compoundTypes{};
};

} // namespace kaitai::detail

