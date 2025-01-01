#ifndef LEXER_H
#define LEXER_H

#include "Token.h"
#include <string>
#include <vector>

class Lexer {
private:
  std::string input;
  size_t current = 0;
  size_t start = 0;

  [[nodiscard]] auto isAtEnd() const -> bool;
  auto advance() -> char;
  [[nodiscard]] auto peek() const -> char;
  [[nodiscard]] auto peekNext() const -> char;
  auto match(char expected) -> bool;
  void skipWhitespace();
  auto number() -> Token;
  auto identifier() -> Token;
  [[nodiscard]] static auto isDigit(char c) -> bool;
  [[nodiscard]] static auto isAlpha(char c) -> bool;
  [[nodiscard]] static auto isAlphaNumeric(char c) -> bool;

public:
  explicit Lexer(std::string input) : input(std::move(input)) {}
  auto scanTokens() -> std::vector<Token>;
  auto scanToken() -> Token;
};

#endif // LEXER_H
