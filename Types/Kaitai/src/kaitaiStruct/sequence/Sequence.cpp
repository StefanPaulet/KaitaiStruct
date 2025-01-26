//
// Created by stefan on 1/23/2025.
//

#include "Sequence.hpp"

namespace kaitai::detail {
auto Sequence::addChunk(Chunk&& chunk) -> void {
  _chunks.push_back(std::move(chunk));
}

auto Sequence::containsChunk(std::string const& chunkId) const -> bool {
  if (auto it = std::ranges::find_if(_chunks, [chunkId](Chunk const& chunk) { return chunkId == chunk.id; });
      it != _chunks.end()) {
    return true;
  }
  return false;
}

[[nodiscard]] auto Sequence::chunks() const -> std::vector<Chunk> const& { return _chunks; }

} // namespace kaitai::detail