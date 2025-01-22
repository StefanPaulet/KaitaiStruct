//
// Created by stefan on 1/3/2025.
//

#include <kaitai/Parser>
#include <sstream>;
using namespace kaitai::detail;

int main() {
  std::stringstream str {R"(meta:
  id: gif
)"};
  Parser parser {str};
  try {
    auto meta = parser().getMeta();
    std::cout << meta.id << '\n';
  } catch (kaitai::exceptions::detail::Exception const& e) {
    std::cout << e.what() << '\n';
  }
  return 0;
}