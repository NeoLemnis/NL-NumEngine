#ifndef PARSER_H
#define PARSER_H

#include "Matrix.h"
#include "Token.h"
#include <memory>
#include <vector>

class Expression {
public:
  virtual ~Expression() = default;
};

class BinaryExpr : public Expression {
public:
  std::shared_ptr<Expression> left;
  Token op;
  std::shared_ptr<Expression> right;

  BinaryExpr(std::shared_ptr<Expression> left, Token op,
             std::shared_ptr<Expression> right)
      : left(std::move(left)), op(std::move(op)), right(std::move(right)) {}
};

class LiteralExpr : public Expression {
public:
  Matrix<double> value;
  explicit LiteralExpr(Matrix<double> value) : value(std::move(value)) {}
};

class VariableExpr : public Expression {
public:
  Token name;
  explicit VariableExpr(Token name) : name(std::move(name)) {}
};

class AssignExpr : public Expression {
public:
  Token name;
  std::shared_ptr<Expression> value;
  AssignExpr(Token name, std::shared_ptr<Expression> value)
      : name(std::move(name)), value(std::move(value)) {}
};

class Parser {
private:
  std::vector<Token> tokens;
  size_t current = 0;

  auto expression() -> std::shared_ptr<Expression>;
  auto assignment() -> std::shared_ptr<Expression>;
  auto term() -> std::shared_ptr<Expression>;
  auto factor() -> std::shared_ptr<Expression>;
  auto primary() -> std::shared_ptr<Expression>;
  auto parseMatrix() -> Matrix<double>;

  auto match(TokenType type) -> bool;
  [[nodiscard]] auto check(TokenType type) const -> bool;
  auto advance() -> Token;
  [[nodiscard]] auto peek() const -> Token;
  [[nodiscard]] auto previous() const -> Token;
  [[nodiscard]] auto isAtEnd() const -> bool;
  void consume(TokenType type, const std::string &message);

public:
  explicit Parser(std::vector<Token> tokens) : tokens(std::move(tokens)) {}
  auto parse() -> std::shared_ptr<Expression>;
};

#endif // PARSER_H
