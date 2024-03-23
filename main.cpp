#include "operand.h"
#include <iostream>
#include <map>
#include <stack>
//#include <string>
//#include <string_view>
#include <vector>
using std::string;
using std::vector;

std::map<string, int8_t> operation_priority = {
        {"(", 0},
        {"->", 1},
        {"+", 1},
        {"-", 1},
        {"*", 2},
        {"/", 2},
        {"**", 3},
};

vector<string> to_postfix_notation(std::string infix_notation) {
    vector<string> result;
    std::stack<string> stack;
    string operand;

    infix_notation += ' ';

    for (size_t i = 0; i < infix_notation.length() - 1; i++) {
        char symbol = infix_notation[i];
        if (symbol == ' ')
            continue;
        if (isdigit(symbol) || isalpha(symbol) || symbol == '.' || symbol == ',' /*|| postfix_function */) {
            operand += symbol;
        } else {
            if (!operand.empty()) {
                result.push_back(operand);
                operand.clear();
            }
            if (false /*Prefix function*/) {
            } else if (symbol == '(') {
                stack.push("(");
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

                while (!stack.empty() && (operation_priority[stack.top()] >= operation_priority[oper])) {
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

        auto res = expression::split_prefix_and_unit("Qg");

        std::cout << int(res.first) << ' ' << res.second << std::endl;

        //        auto res = to_postfix_notation(input);
        //
        //        for (const auto a: res) {
        //            std::cout << a << '\n';
        //        }
        //        std::cout << std::endl;

        if (input == "q") {
            break;
        }
    }

    return 0;
}
