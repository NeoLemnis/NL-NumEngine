#include "Interpreter.h"
#include "Lexer.h"
#include "Parser.h"
#include <gtest/gtest.h>

class InterpreterTest : public ::testing::Test {
protected:
  Interpreter interpreter;

  Matrix<double> evaluate(const std::string &input) {
    Lexer lexer(input);
    auto tokens = lexer.scanTokens();
    Parser parser(tokens);
    auto expr = parser.parse();
    return interpreter.interpret(expr);
  }
};

TEST_F(InterpreterTest, SimpleAddition) {
  Matrix<double> result = evaluate("[1, 2; 3, 4] + [5, 6; 7, 8]");
  EXPECT_EQ(result(0, 0), 6);
  EXPECT_EQ(result(0, 1), 8);
  EXPECT_EQ(result(1, 0), 10);
  EXPECT_EQ(result(1, 1), 12);
}

TEST_F(InterpreterTest, MatrixMultiplication) {
  Matrix<double> result = evaluate("[1, 2; 3, 4] * [5, 6; 7, 8]");
  EXPECT_EQ(result(0, 0), 19);
  EXPECT_EQ(result(0, 1), 22);
  EXPECT_EQ(result(1, 0), 43);
  EXPECT_EQ(result(1, 1), 50);
}

TEST_F(InterpreterTest, VariableAssignment) {
  evaluate("A = [1, 2; 3, 4]");
  Matrix<double> result = evaluate("A * 2");
  EXPECT_EQ(result(0, 0), 2);
  EXPECT_EQ(result(0, 1), 4);
  EXPECT_EQ(result(1, 0), 6);
  EXPECT_EQ(result(1, 1), 8);
}

TEST_F(InterpreterTest, ComplexExpression) {
  evaluate("A = [1, 2; 3, 4]");
  evaluate("B = [5, 6; 7, 8]");
  Matrix<double> result = evaluate("(A + B) * 2");
  EXPECT_EQ(result(0, 0), 12);
  EXPECT_EQ(result(0, 1), 16);
  EXPECT_EQ(result(1, 0), 20);
  EXPECT_EQ(result(1, 1), 24);
}

TEST_F(InterpreterTest, LastResult) {
  evaluate("2 * 3");
  Matrix<double> result = evaluate("ans * 2");
  EXPECT_EQ(result(0, 0), 12);
}