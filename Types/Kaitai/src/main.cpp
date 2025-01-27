//
// Created by stefan on 1/3/2025.
//

#include <kaitai/Parser>
#include <kaitai/BinaryParser>
#include <sstream>

using namespace kaitai::detail;

int main() {
  std::stringstream str {R"(meta:
  endian: be
seq:
  - id: t1
    type: t1
types:
  t1:
    seq:
      - id: t1Content
        contents: 'T1'
      - id: t2Size
        size: 3
      - id: t3Type
        type: u2
)"};
  Parser parser {str};
  try {
    auto kstruct = parser();
    BinaryParser binaryParser{std::move(kstruct)};
    std::stringstream test{"T1abcde"};
    auto result = binaryParser(test);
    auto field = std::get<ParsedField::CompoundData>(result.fields.front().value);
    std::cout << std::get<bool>(field[0].value) << '\n';
    std::cout << std::get<std::string>(field[1].value) << '\n';
    std::cout << std::get<unsigned long long>(field[2].value) << '\n';
  } catch (kaitai::exceptions::detail::Exception const& e) {
    std::cout << e.what() << '\n';
  }
  return 0;
}