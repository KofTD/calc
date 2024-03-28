#include "notation_operations.h"

#include <algorithm>
#include <array>
#include <ranges>
#include <stack>

std::string take_operator(std::array<char, 2> notation_part) {
    if (notation_part[0] == '*' && notation_part[1] == '*') {
        return "**";
    }
    if (notation_part[0] == '-' && notation_part[1] == '>') {
        return "->";
    }

    return {notation_part[0]};
}

std::vector<std::string> to_postfix_notation(std::string infix_notation) {
    using std::string;

    std::vector<string> result;
    std::stack<string> stack;
    string operand;

    const std::vector<string> prefix_function = {
        "sin", "cos", "sqrt", "tan", "cot", "asin", "acos", "atan", "acot"};
    auto empty_operand = [&]() {
        if (!operand.empty()) result.push_back(operand);
        operand.clear();
    };

    infix_notation += ' ';

    for (size_t i = 0; i < infix_notation.length() - 1; i++) {
        char symbol = infix_notation[i];

        if (symbol == ' ') {
            continue;
        }

        if (((isdigit(symbol)) != 0) || (isalpha(symbol) != 0) ||
            symbol == '.' || symbol == ',') {
            operand += symbol;
        } else {
            empty_operand();

            if (/*std::ranges::contains(prefix_function)*/ false) {
                // prefix_function stuff
            } else if (symbol == '(') {
                stack.emplace("(");
            } else if (symbol == ')') {
                while (stack.top() != "(") {
                    result.push_back(stack.top());
                    stack.pop();
                }
                stack.pop();
            } else {
                std::string oper =
                    take_operator({symbol, infix_notation[i + 1]});

                while (!stack.empty() && (operation_priority.at(stack.top()) >=
                                          operation_priority.at(oper))) {
                    result.push_back(stack.top());
                    stack.pop();
                }
                stack.push(oper);
            }
        }
    }

    empty_operand();

    while (!stack.empty()) {
        result.push_back(stack.top());
        stack.pop();
    }

    return result;
}
