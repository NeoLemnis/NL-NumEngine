#include "Lexer.h"
#include <gtest/gtest.h>

TEST(LexerTest, Numbers) {
  Lexer lexer("123.456");
  auto tokens = lexer.scanTokens();

  EXPECT_EQ(tokens.size(), 2); // Number token + END token
  EXPECT_EQ(tokens[0].type, TokenType::NUMBER);
  EXPECT_EQ(tokens[0].lexeme, "123.456");
}

TEST(LexerTest, Operators) {
  Lexer lexer("+ - * =");
  auto tokens = lexer.scanTokens();

  EXPECT_EQ(tokens[0].type, TokenType::PLUS);
  EXPECT_EQ(tokens[1].type, TokenType::MINUS);
  EXPECT_EQ(tokens[2].type, TokenType::MULTIPLY);
  EXPECT_EQ(tokens[3].type, TokenType::ASSIGN);
}

TEST(LexerTest, MatrixLiteral) {
  Lexer lexer("[1, 2; 3, 4]");
  auto tokens = lexer.scanTokens();

  EXPECT_EQ(tokens[0].type, TokenType::LBRACKET);
  EXPECT_EQ(tokens[1].type, TokenType::NUMBER);
  EXPECT_EQ(tokens[2].type, TokenType::COMMA);
  EXPECT_EQ(tokens[3].type, TokenType::NUMBER);
  EXPECT_EQ(tokens[4].type, TokenType::SEMICOLON);
}

TEST(LexerTest, Variables) {
  Lexer lexer("A = B + C");
  auto tokens = lexer.scanTokens();

  EXPECT_EQ(tokens[0].type, TokenType::IDENTIFIER);
  EXPECT_EQ(tokens[1].type, TokenType::ASSIGN);
  EXPECT_EQ(tokens[2].type, TokenType::IDENTIFIER);
  EXPECT_EQ(tokens[3].type, TokenType::PLUS);
  EXPECT_EQ(tokens[4].type, TokenType::IDENTIFIER);
}