//
// Created by stefan on 1/23/2025.
//

#pragma once

#include <format>
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

  auto operator<=>(Chunk const& other) const = default;

  std::string id;
  std::variant<Size, Constant, RawType> data;
};


class Sequence {
public:
  auto addChunk(Chunk&& chunk) -> void;
  [[nodiscard]] auto containsChunk(std::string const& chunkId) const -> bool;
  [[nodiscard]] auto chunks() const -> std::vector<Chunk> const&;
  auto operator<=>(Sequence const& other) const = default;

private:
  std::vector<Chunk> _chunks{};
};

struct CompoundType {
  std::string name;
  Sequence seq;
};
} // namespace kaitai::detail


template <> struct std::formatter<kaitai::detail::Chunk::RawType> {
  using Type = kaitai::detail::Chunk::RawType;

  constexpr auto parse(auto& ctx) {
    auto it = ctx.begin();
    if (it == ctx.end()) {
      return it;
    }
    if (it != ctx.end() && *it != '}') {
      throw std::format_error("Invalid format args for RawType.");
    }

    return it;
  }

  auto format(Type type, format_context& ctx) const {
    using namespace kaitai::detail;
    auto toString = [](Type t) -> std::string {
      switch (t) {
        using enum Type;
        case U1: { return "unsigned char"; }
        case U2: { return "unsigned short int"; }
      }
    };
    return formatter<std::string>{}.format(toString(type), ctx);
  }
};

