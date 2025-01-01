#include "Parser.h"
#include <stdexcept>

auto Parser::parse() -> std::shared_ptr<Expression> { return expression(); }

auto Parser::expression() -> std::shared_ptr<Expression> {
  return assignment();
}

auto Parser::assignment() -> std::shared_ptr<Expression> {
  auto expr = term();

  if (match(TokenType::ASSIGN)) {
    Token equals = previous();
    auto value = assignment();

    if (auto *varExpr = dynamic_cast<VariableExpr *>(expr.get())) {
      Token name = varExpr->name;
      return std::make_shared<AssignExpr>(name, value);
    }

    throw std::runtime_error("Invalid assignment target.");
  }

  return expr;
}

auto Parser::term() -> std::shared_ptr<Expression> {
  auto expr = factor();

  while (match(TokenType::PLUS) || match(TokenType::MINUS)) {
    Token op = previous();
    auto right = factor();
    expr = std::make_shared<BinaryExpr>(expr, op, right);
  }

  return expr;
}

auto Parser::factor() -> std::shared_ptr<Expression> {
  auto expr = primary();

  while (match(TokenType::MULTIPLY)) {
    Token op = previous();
    auto right = primary();
    expr = std::make_shared<BinaryExpr>(expr, op, right);
  }

  return expr;
}

auto Parser::primary() -> std::shared_ptr<Expression> {
  if (match(TokenType::NUMBER)) {
    Matrix<double> scalar(1, 1);
    scalar(0, 0) = std::stod(previous().lexeme);
    return std::make_shared<LiteralExpr>(scalar);
  }

  if (match(TokenType::IDENTIFIER)) {
    return std::make_shared<VariableExpr>(previous());
  }

  if (match(TokenType::LBRACKET)) {
    auto matrix = parseMatrix();
    consume(TokenType::RBRACKET, "Expect ']' after matrix.");
    return std::make_shared<LiteralExpr>(matrix);
  }

  if (match(TokenType::LPAREN)) {
    auto expr = expression();
    consume(TokenType::RPAREN, "Expect ')' after expression.");
    return expr;
  }

  throw std::runtime_error("Expect expression.");
}

auto Parser::parseMatrix() -> Matrix<double> {
  std::vector<std::vector<double>> rows;
  std::vector<double> currentRow;

  while (!check(TokenType::RBRACKET)) {
    if (match(TokenType::NUMBER)) {
      currentRow.push_back(std::stod(previous().lexeme));
    } else if (match(TokenType::COMMA)) {
      continue;
    } else if (match(TokenType::SEMICOLON)) {
      if (!currentRow.empty()) {
        rows.push_back(currentRow);
        currentRow.clear();
      }
    } else {
      throw std::runtime_error("Invalid matrix format.");
    }
  }

  if (!currentRow.empty()) {
    rows.push_back(currentRow);
  }

  if (rows.empty()) {
    throw std::runtime_error("Empty matrix.");
  }

  size_t cols = rows[0].size();
  for (const auto &row : rows) {
    if (row.size() != cols) {
      throw std::runtime_error("Inconsistent matrix dimensions.");
    }
  }

  Matrix<double> result(rows.size(), cols);
  for (size_t i = 0; i < rows.size(); ++i) {
    for (size_t j = 0; j < cols; ++j) {
      result(i, j) = rows[i][j];
    }
  }
  return result;
}

auto Parser::match(TokenType type) -> bool {
  if (check(type)) {
    advance();
    return true;
  }
  return false;
}

auto Parser::check(TokenType type) const -> bool {
  if (isAtEnd()) {
    return false;
  }
  return peek().type == type;
}

auto Parser::advance() -> Token {
  if (!isAtEnd()) {
    current++;
  }
  return previous();
}

auto Parser::peek() const -> Token { return tokens[current]; }

auto Parser::previous() const -> Token { return tokens[current - 1]; }

auto Parser::isAtEnd() const -> bool { return peek().type == TokenType::END; }

void Parser::consume(TokenType type, const std::string &message) {
  if (check(type)) {
    advance();
    return;
  }
  throw std::runtime_error(message);
}
