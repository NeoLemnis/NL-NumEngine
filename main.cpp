#include "Interpreter.h"
#include "Lexer.h"
#include "Parser.h"
#include <iostream>
#include <string>

auto main() -> int {
  Interpreter interpreter;
  std::string line;

  std::cout << "Matrix Calculator (type 'exit' to quit)\n";

  while (true) {
    std::cout << "> ";
    std::getline(std::cin, line);

    if (line == "exit") {
      break;
    }
    if (line.empty()) {
      continue;
    }

    try {
      Lexer lexer(line);
      auto tokens = lexer.scanTokens();

      Parser parser(tokens);
      auto expression = parser.parse();

      bool printResult = true;
      // Don't print if the line ends with semicolon
      if (!tokens.empty() &&
          tokens[tokens.size() - 2].type == TokenType::SEMICOLON) {
        printResult = false;
      }

      Matrix<double> result = interpreter.interpret(expression, printResult);

      if (printResult) {
        std::cout << result << '\n';
      }
    } catch (const std::exception &e) {
      std::cerr << "Error: " << e.what() << '\n';
    }
  }

  return 0;
}
