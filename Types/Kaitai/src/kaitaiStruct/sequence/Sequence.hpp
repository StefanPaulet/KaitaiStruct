//
// Created by stefan on 1/23/2025.
//

#pragma once

#include <string>
#include <variant>
#include <vector>

namespace kaitai::detail {
struct Chunk {
  using Size = unsigned int;
  using Constant = std::string;

  enum class RawType {
    U1, U2
  };

  std::string id;
  std::variant<Size, Constant, RawType> data;
};

class Sequence {
public:
  auto addChunk(Chunk&& chunk) -> void;
  [[nodiscard]] auto containsChunk(std::string const& chunkId) const -> bool;
  [[nodiscard]] auto chunks() const -> std::vector<Chunk> const&;

private:
  std::vector<Chunk> _chunks{};
};
} // namespace kaitai::detail

