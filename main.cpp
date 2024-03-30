#include <algorithm>
#include <cctype>
#include <iostream>
#include <stack>
#include <vector>

#include "notation_operations.h"
#include "operand.h"
using std::cout;
using std::string;
using std::vector;

expression::operand calculate(const vector<string> &postfix_notation) {
    using namespace std::literals;
    std::stack<expression::operand> calculations;
    for (const std::string &token : postfix_notation) {
        if (operation_priority.contains(token)) {
            if (token == "->") {
                auto oper = calculations.top();
                calculations.pop();
                // convert
            } else {
                auto op1 = calculations.top();
                calculations.pop();
                auto op2 = calculations.top();
                calculations.pop();

                if (token == "+") {
                    calculations.push(op1 + op2);
                } else if (token == "-") {
                    calculations.push(op2 - op1);
                } else if (token == "*") {
                    calculations.push(op1 * op2);
                } else if (token == "/") {
                    calculations.push(op2 / op1);
                } else if (token == "**") {
                    calculations.push(op2.power(op1));
                }
            }
        } else {
            auto has_digits = [](std::string token) {
                namespace sr = std::ranges;
                return sr::find_if(token, [](char sym) {
                           return isdigit(sym) != 0;
                       }) != sr::end(token);
            };
            if (!has_digits(token)) {
                std::string buf = "1";
                buf.append(token);
                calculations.emplace(buf);
            } else {
                calculations.emplace(token);
            }
        }
    }

    return calculations.top();
}

int main() {
    while (true) {
        cout << ">>> ";
        std::string input;
        std::getline(std::cin, input);

        if (input == "q") {
            break;
        }

        auto postfix_notation = to_postfix_notation(input);

        auto calculation_result = calculate(postfix_notation);

        cout << calculation_result.to_string() << '\n';
    }

    return 0;
}
