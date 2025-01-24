//
// Created by stefan on 1/3/2025.
//

#include <kaitai/Parser>
#include <sstream>

using namespace kaitai::detail;

int main() {
  std::stringstream str {R"(meta:
seq:
  - id: t1
    type: t1
types:
  t1:
    seq:
      - id: t1Content
        contents: 'T1'
)"};
  Parser parser {str};
  try {
    auto kstruct = parser();
    auto meta = kstruct.getMeta();
    std::cout << meta.id << '\n';
    std::cout << meta.fileExtension << '\n';
    std::cout << [](Endian endian) {
      switch (endian) {
        case Endian::LITTLE: { return "little"; }
        case Endian::BIG: { return "big"; }
      }
    }(meta.endian) << "\n\n";

    auto seq = kstruct.getSequence();
    std::cout << seq.size() << '\n';
    for (auto const& chunk : seq) {
      std::cout << std::get<1>(chunk).name << '\n';
    }
  } catch (kaitai::exceptions::detail::Exception const& e) {
    std::cout << e.what() << '\n';
  }
  return 0;
}