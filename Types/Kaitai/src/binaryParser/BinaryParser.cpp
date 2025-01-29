//
// Created by stefan on 1/27/25.
//

#include "BinaryParser.hpp"

namespace kaitai::detail {

auto BinaryParser::operator()(std::istream& istream) const -> ParsedBinary {
  ParsedBinary result{};
  for (auto const& header : _format.getSequence()) {
    result.fields.emplace_back(
      parseHeader(istream, std::get<0>(header), _format.getTypes().at(std::get<1>(header)))
    );
  }
  return result;
}

auto BinaryParser::parseHeader(InputStream& istream, std::string_view name, CompoundType const& type) const
  -> ParsedField {
  auto const& [_, sequence] =  type;

  ParsedField field {std::string{name}, std::vector<ParsedField>{}};

  for (auto const& [id, data] : sequence.chunks()) {
    const auto visitor = overloadSet {
      [this, &id, &istream](Chunk::Size size) -> ParsedField {
        return {id, readBuffer(istream, size, _endian)};
      },
      [this, &id, &istream](Chunk::Constant const& constant) -> ParsedField {
        return {id, parseConstant(istream, constant)};
      },
      [this, &id, &istream](Chunk::RawType type) -> ParsedField {
        return {id, parsedTypeField(istream, type)};
      }
    };
    std::visit([&field, &visitor](auto&& item) {
      std::get<ParsedField::CompoundData>(field.value)
          .emplace_back(visitor(std::forward<decltype(item)>(item)));
    }, data);
  }

  return field;
}

auto BinaryParser::parseConstant(InputStream& istream, Chunk::Constant const& data) const -> ParsedField::SignatureData {
  auto size = data.size();
  auto buffer = readBuffer(istream, size, _endian);

  for (auto idx = 0; idx < data.size(); ++idx) {
    if (data[idx] != buffer[idx]) {
      return false;
    }
  }
  return true;
}

auto BinaryParser::parsedTypeField(InputStream& istream, Chunk::RawType type) const -> ParsedField::IntData {
  size_t size;
  switch (type) {
    using enum Chunk::RawType;
    case U1: {
      size = 1;
      break;
    }
    case U2: {
      size = 2;
      break;
    }
  }

  auto buffer = readBuffer(istream, size, _endian);
  ParsedField::IntData result {0};
  for (auto chr : buffer) {
    result = result << 8 | chr;
  }
  return result;
}

} // namespace kaitai::detail

