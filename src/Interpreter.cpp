#include "Interpreter.h"
#include <stdexcept>

auto Interpreter::interpret(const std::shared_ptr<Expression> &expression,
                            bool printResult) -> Matrix<double> {
  shouldPrint = printResult;
  lastResult = evaluate(expression);
  variables["ans"] = lastResult;
  return lastResult;
}

auto Interpreter::evaluate(const std::shared_ptr<Expression> &expr)
    -> Matrix<double> {
  if (auto *literalExpr = dynamic_cast<LiteralExpr *>(expr.get())) {
    return evaluateLiteral(literalExpr);
  }
  if (auto *binaryExpr = dynamic_cast<BinaryExpr *>(expr.get())) {
    return evaluateBinary(binaryExpr);
  }
  if (auto *variableExpr = dynamic_cast<VariableExpr *>(expr.get())) {
    return evaluateVariable(variableExpr);
  }
  if (auto *assignExpr = dynamic_cast<AssignExpr *>(expr.get())) {
    return evaluateAssign(assignExpr);
  }
  throw std::runtime_error("Unknown expression type.");
}

auto Interpreter::evaluateBinary(const BinaryExpr *expr) -> Matrix<double> {
  Matrix<double> left = evaluate(expr->left);
  Matrix<double> right = evaluate(expr->right);

  switch (expr->op.type) {
  case TokenType::PLUS:
    return left + right;
  case TokenType::MINUS:
    return left - right;
  case TokenType::MULTIPLY:
    return left * right;
  default:
    throw std::runtime_error("Unknown operator.");
  }
}

auto Interpreter::evaluateLiteral(const LiteralExpr *expr) -> Matrix<double> {
  return expr->value;
}

auto Interpreter::evaluateVariable(const VariableExpr *expr) -> Matrix<double> {
  auto it = variables.find(expr->name.lexeme);
  if (it != variables.end()) {
    return it->second;
  }
  throw std::runtime_error("Undefined variable '" + expr->name.lexeme + "'.");
}

auto Interpreter::evaluateAssign(const AssignExpr *expr) -> Matrix<double> {
  Matrix<double> value = evaluate(expr->value);
  variables[expr->name.lexeme] = value;
  return value;
}

void Interpreter::setVariable(const std::string &name,
                              const Matrix<double> &value) {
  variables[name] = value;
}

auto Interpreter::getVariable(const std::string &name) -> Matrix<double> {
  auto it = variables.find(name);
  if (it != variables.end()) {
    return it->second;
  }
  throw std::runtime_error("Undefined variable '" + name + "'.");
}
