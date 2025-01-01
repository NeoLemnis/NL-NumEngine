#include "Lexer.h"
#include "Parser.h"
#include <gtest/gtest.h>

TEST(ParserTest, SimpleExpression) {
  Lexer lexer("1 + 2");
  auto tokens = lexer.scanTokens();
  Parser parser(tokens);

  auto expr = parser.parse();
  EXPECT_NE(dynamic_cast<BinaryExpr *>(expr.get()), nullptr);
}

TEST(ParserTest, MatrixLiteral) {
  Lexer lexer("[1, 2; 3, 4]");
  auto tokens = lexer.scanTokens();
  Parser parser(tokens);

  auto expr = parser.parse();
  EXPECT_NE(dynamic_cast<LiteralExpr *>(expr.get()), nullptr);
}

TEST(ParserTest, Assignment) {
  Lexer lexer("A = [1, 2; 3, 4]");
  auto tokens = lexer.scanTokens();
  Parser parser(tokens);

  auto expr = parser.parse();
  EXPECT_NE(dynamic_cast<AssignExpr *>(expr.get()), nullptr);
}

TEST(ParserTest, ComplexExpression) {
  Lexer lexer("(A + B) * C");
  auto tokens = lexer.scanTokens();
  Parser parser(tokens);

  auto expr = parser.parse();
  EXPECT_NE(dynamic_cast<BinaryExpr *>(expr.get()), nullptr);
}