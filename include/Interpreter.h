#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "Parser.h"
#include <string>
#include <unordered_map>

class Interpreter {
private:
  std::unordered_map<std::string, Matrix<double>> variables;
  Matrix<double> lastResult;
  bool shouldPrint = true;

  auto evaluate(const std::shared_ptr<Expression> &expr) -> Matrix<double>;
  auto evaluateBinary(const BinaryExpr *expr) -> Matrix<double>;
  static auto evaluateLiteral(const LiteralExpr *expr) -> Matrix<double>;
  auto evaluateVariable(const VariableExpr *expr) -> Matrix<double>;
  auto evaluateAssign(const AssignExpr *expr) -> Matrix<double>;

public:
  Interpreter() { variables["ans"] = Matrix<double>(1, 1); }

  auto interpret(const std::shared_ptr<Expression> &expression,
                 bool printResult = true) -> Matrix<double>;
  void setVariable(const std::string &name, const Matrix<double> &value);
  auto getVariable(const std::string &name) -> Matrix<double>;
  auto getLastResult() const -> Matrix<double> { return lastResult; }
};

#endif // INTERPRETER_H
