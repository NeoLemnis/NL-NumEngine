#include "Lexer.h"

auto Lexer::isAtEnd() const -> bool { return current >= input.length(); }

auto Lexer::advance() -> char { return input[current++]; }

auto Lexer::peek() const -> char {
  if (isAtEnd()) {
    return '\0';
  }
  return input[current];
}

auto Lexer::peekNext() const -> char {
  if (current + 1 >= input.length()) {
    return '\0';
  }
  return input[current + 1];
}

auto Lexer::match(char expected) -> bool {
  if (isAtEnd()) {
    return false;
  }
  if (input[current] != expected) {
    return false;
  }
  current++;
  return true;
}

void Lexer::skipWhitespace() {
  while (true) {
    char c = peek();
    switch (c) {
    case ' ':
    case '\r':
    case '\t':
      advance();
      break;
    default:
      return;
    }
  }
}

auto Lexer::number() -> Token {
  while (isDigit(peek())) {
    advance();
  }

  if (peek() == '.' && isDigit(peekNext())) {
    advance();
    while (isDigit(peek())) {
      advance();
    }
  }

  return {TokenType::NUMBER, input.substr(start, current - start)};
}

auto Lexer::identifier() -> Token {
  while (isAlphaNumeric(peek())) {
    advance();
  }
  return {TokenType::IDENTIFIER, input.substr(start, current - start)};
}

auto Lexer::isDigit(char c) -> bool { return c >= '0' && c <= '9'; }

auto Lexer::isAlpha(char c) -> bool {
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

auto Lexer::isAlphaNumeric(char c) -> bool { return isAlpha(c) || isDigit(c); }

auto Lexer::scanToken() -> Token {
  skipWhitespace();
  start = current;

  if (isAtEnd()) {
    return {TokenType::END, ""};
  }

  char c = advance();

  if (isDigit(c)) {
    return number();
  }
  if (isAlpha(c)) {
    return identifier();
  }

  switch (c) {
  case '(':
    return {TokenType::LPAREN, "("};
  case ')':
    return {TokenType::RPAREN, ")"};
  case '[':
    return {TokenType::LBRACKET, "["};
  case ']':
    return {TokenType::RBRACKET, "]"};
  case ',':
    return {TokenType::COMMA, ","};
  case '+':
    return {TokenType::PLUS, "+"};
  case '-':
    return {TokenType::MINUS, "-"};
  case '*':
    return {TokenType::MULTIPLY, "*"};
  case '=':
    return {TokenType::ASSIGN, "="};
  case ';':
    return {TokenType::SEMICOLON, ";"};
  case '\n':
    return {TokenType::EOL, "\n"};
  }

  throw std::runtime_error("Unexpected character");
}

auto Lexer::scanTokens() -> std::vector<Token> {
  std::vector<Token> tokens;
  while (!isAtEnd()) {
    tokens.push_back(scanToken());
  }
  tokens.emplace_back(TokenType::END, "");
  return tokens;
}
