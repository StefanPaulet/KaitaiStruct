//
// Created by stefan on 1/27/25.
//

#include <string>

namespace kaitai::detail {

struct ParsedField {
  using IntData = unsigned long long int;
  using StringData = std::string;
  using SignatureData = bool;
  using CompoundData = std::vector<ParsedField>;

  std::string name;
  std::variant<IntData, StringData, SignatureData, CompoundData> value;
};

struct ParsedBinary {
  std::vector<ParsedField> fields;
};

} // namespace kaitai::detail