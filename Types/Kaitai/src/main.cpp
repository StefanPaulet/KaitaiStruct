//
// Created by stefan on 1/3/2025.
//

#include <kaitai/Parser>
#include <sstream>

using namespace kaitai::detail;

int main() {
  std::stringstream str {R"(meta:
  id: gif
  file-extension: gif
  endian: le
seq:
  - id: header
    type: header
  - id: logical_screen
    type: logical_screen
types:
  header:
    seq:
      - id: magic
        contents: 'GIF'
      - id: version
        size: 3
  logical_screen:
    seq:
      - id: image_width
        type: u2
      - id: image_height
        type: u2
      - id: flags
        type: u1
      - id: bg_color_index
        type: u1
      - id: pixel_aspect_ratio
        type: u1
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
      std::cout << chunk.name << '\n';
    }
  } catch (kaitai::exceptions::detail::Exception const& e) {
    std::cout << e.what() << '\n';
  }
  return 0;
}