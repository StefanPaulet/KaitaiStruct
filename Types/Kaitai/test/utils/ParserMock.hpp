//
// Created by stefan on 1/24/2025.
//

#pragma once

#include <deque>
#include <kaitai/Parser>

namespace kaitai::testing {
using detail::Chunk;
using detail::CompoundType;
using detail::KaitaiStruct;
using detail::Meta;
using detail::Sequence;

inline auto contents(std::string name, std::string content) -> Chunk {
  return Chunk{name, content};
}

inline auto size(std::string name, unsigned int size) -> Chunk {
  return Chunk{name, size};
}

inline auto type(std::string name, Chunk::RawType type) -> Chunk {
  return Chunk{name, type};
}

inline auto compareSequences(KaitaiStruct::MainSequence const& actual, std::deque<std::tuple<std::string, std::string>>&& expected) {
  for (auto const& [name, type] : actual) {
    auto typeName = type.name;
    auto [expName, expTypeName] = expected.front();
    if (name != expName) {
      std::cout << std::format("Item name {} does not match expected {}\n", name, expName);
      return false;
    }
    if (typeName != expTypeName) {
      std::cout << std::format("Type name {} does not match expected {}\n", name, expName);
      return false;
    }
    expected.pop_front();
  }
  return true;
}

inline auto compareTypes(KaitaiStruct::TypesMap const& actual, std::vector<std::tuple<std::string, std::deque<Chunk>>>&& expected) -> bool {
  for (auto&& [name, seq] : std::move(expected)) {
    if (auto it = actual.find(name); it != actual.end()) {
      auto const& actualSeq = it->second;
      if (actualSeq.seq.chunks().size() != seq.size()) {
        std::cout << std::format("Non matching sequence sizes {} and {}\n", actualSeq.seq.chunks().size(), seq.size());
        return false;
      }
      for (auto const& chunk : actualSeq.seq.chunks()) {
        if (chunk != seq.front()) {
          std::cout << std::format("Mismatched chunks: c1={}, c2={}\n", chunk.id, seq.front().id);
          return false;
        }
        seq.pop_front();
      }
    }
  }
  return true;
}
} // namespace kaitai::testing