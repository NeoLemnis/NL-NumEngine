#ifndef TOKEN_H
#define TOKEN_H

#include "Matrix.h"
#include <memory>
#include <string>
#include <variant>

enum class TokenType {
  NUMBER,     // Numeric literal
  IDENTIFIER, // Variable names
  PLUS,       // +
  MINUS,      // -
  MULTIPLY,   // *
  ASSIGN,     // =
  SEMICOLON,  // ;
  LPAREN,     // (
  RPAREN,     // )
  LBRACKET,   // [
  RBRACKET,   // ]
  COMMA,      // ,
  EOL,        // End of line
  END         // End of input
};

struct Token {
  TokenType type;
  std::string lexeme;

  Token(TokenType type, std::string lexeme)
      : type(type), lexeme(std::move(lexeme)) {}
};

#endif // TOKEN_H