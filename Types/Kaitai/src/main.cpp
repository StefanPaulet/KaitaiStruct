//
// Created by stefan on 1/3/2025.
//

#include "lexer/Lexer.hpp"

#include <deque>

int main() {
  std::string input = R"(meta:
  id: gif
seq:
  - type: header
)";
  kaitai::detail::Lexer lexer{std::move(input)};

  using enum kaitai::detail::TokenType;
  using kaitai::detail::Token;
  std::deque tokens {
    Token{Meta},
    Token{Colon},
    Token{NewLine},
    Token{Tab},
    Token{Id},
    Token{Colon},
    Token{Space},
    Token{Identifier, "gif"},
    Token{NewLine},
    Token{Seq},
    Token{Colon},
    Token{NewLine},
    Token{Tab},
    Token{Dash},
    Token{Space},
    Token{Type},
    Token{Colon},
    Token{Space},
    Token{Identifier, "header"},
    Token{NewLine}
  };

  for (auto token = lexer(); token.has_value(); token = lexer()) {
    if (tokens.empty()) {
      std::cout << "Expected token: " << token.value() << " but found nothing\n";
      break;
    }
    if (token != tokens.front()) {
      std::cout << "Expected token: " << tokens.front() << " but found: " << token.value() << '\n';
      return -1;
    }
    std::cout << "Matching token " << token.value() << '\n';
    tokens.pop_front();
  }
  if (!tokens.empty()) {
    std::cout << "Expected all tokens to match\n";
    return -1;
  }

  return 0;
}