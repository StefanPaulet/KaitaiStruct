//
// Created by stefan on 1/27/25.
//

#pragma once

#include "../kaitaiStruct/KaitaiStruct.hpp"
#include "parsedBinary/ParsedBinary.hpp"
#include "BinaryParserExceptions.hpp"
#include <algorithm>
#include <array>
#include <iostream>
#include <type_traits>

namespace kaitai::detail {

template <typename... Types>
struct overloadSet : Types... { using Types::operator()...; };

class BinaryParser {
public:
  using InputStream = std::basic_istream<char>;

  explicit BinaryParser(KaitaiStruct&& format) :
    _format{std::move(format)}, _endian{_format.getMeta().endian} {};

  auto operator()(InputStream& istream) const -> ParsedBinary;

private:
  auto parseHeader(InputStream& istream, std::string_view name, CompoundType const& type) const -> ParsedField;
  auto parseConstant(InputStream& istream, Chunk::Constant const& size) const -> ParsedField::SignatureData;
  auto parsedTypeField(InputStream& istream, Chunk::RawType type) const -> ParsedField::IntData;

  static auto readBuffer(InputStream& istream, std::size_t size, Endian endian) -> std::string {
    std::array<char, sizeof(ParsedField::IntData)> buffer{};
    if (size > buffer.size()) {
      throw exceptions::MaximumSizeExceededException{buffer.size()};
    }
    istream.read(buffer.data(), size);
    std::span view {buffer.data(), size};
    if (endian == Endian::BIG) {
      std::ranges::reverse(view.begin(), view.end());
    }
    return {view.data(), size};
  }

  KaitaiStruct _format;
  Endian _endian;
};

} // namespace kaitai::detail

