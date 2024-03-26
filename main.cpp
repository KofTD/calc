#include <algorithm>
#include <cctype>
#include <iostream>
#include <map>
#include <stack>
#include <vector>

#include "operand.h"
using std::string;
using std::vector;

const std::map<string, int8_t> operation_priority = {
    {"(", 0}, {"->", 1}, {"+", 1}, {"-", 1}, {"*", 2}, {"/", 2}, {"**", 3},
};

expression::operand calculate(const vector<string> &postfix_notation) {
  std::stack<expression::operand> calculations;
  for (const auto &notation_part : postfix_notation) {
    if (operation_priority.contains(notation_part)) {
      if (notation_part == "->") {
        auto oper = calculations.top();
        calculations.pop();
        // convert
      } else {
        auto op1 = calculations.top();
        calculations.pop();
        auto op2 = calculations.top();
        calculations.pop();

        if (notation_part == "+") {
          calculations.push(op1 + op2);
        } else if (notation_part == "-") {
          calculations.push(op2 - op1);
        } else if (notation_part == "*") {
          calculations.push(op1 * op2);
        } else if (notation_part == "/") {
          calculations.push(op2 / op1);
        } else if (notation_part == "**") {
          calculations.push(op2.power(op1));
        }
      }
    } else {
      auto start_of_unit_part =
          std::find_if(notation_part.begin(), notation_part.end(),
                       [](char chr) { return isalpha(chr) != 0; });
      calculations.emplace(
          notation_part.substr(
              0, std::distance(notation_part.begin(), start_of_unit_part)),
          notation_part.substr(
              std::distance(notation_part.begin(), start_of_unit_part)));
    }
  }

  return calculations.top();
}

vector<string> to_postfix_notation(std::string infix_notation) {
  vector<string> result;
  std::stack<string> stack;
  string operand;

  infix_notation += ' ';

  for (size_t i = 0; i < infix_notation.length() - 1; i++) {
    char symbol = infix_notation[i];
    if (symbol == ' ') {
      continue;
    }
    if ((isdigit(symbol) != 0) || (isalpha(symbol) != 0) || symbol == '.' ||
        symbol == ',' /*|| postfix_function */) {
      operand += symbol;
    } else {
      if (!operand.empty()) {
        result.push_back(operand);
        operand.clear();
      }
      if (false /*Prefix function*/) {
      } else if (symbol == '(') {
        stack.emplace("(");
      } else if (symbol == ')') {
        while (stack.top() != "(") {
          result.push_back(stack.top());
          stack.pop();
        }
        stack.pop();
      } else {
        string oper;
        oper.push_back(symbol);
        if (symbol == '*' && infix_notation[i + 1] == '*') {
          oper = "**";
          i++;
        } else if (symbol == '-' && infix_notation[i + 1] == '>') {
          oper = "->";
          i++;
        }

        while (!stack.empty() && (operation_priority.at(stack.top()) >=
                                  operation_priority.at(oper))) {
          result.push_back(stack.top());
          stack.pop();
        }
        stack.push(oper);
      }
    }
  }

  if (!operand.empty()) {
    result.push_back(operand);
    operand.clear();
  }

  while (!stack.empty()) {
    result.push_back(stack.top());
    stack.pop();
  }

  return result;
}

int main() {
  while (true) {
    std::cout << ">>> ";
    std::string input;
    std::getline(std::cin, input);

    if (input == "q") {
      break;
    }

    auto res = to_postfix_notation(input);
    for (const auto &part : res) {
      std::cout << part << '\n';
    }
    std::cout << '\n';

    auto calc_res = calculate(res);

    std::cout << calc_res << '\n';
  }

  return 0;
}
