#include <algorithm>
#include <cctype>
#include <iostream>
#include <stack>
#include <vector>

#include "notation_operations.h"
#include "operand.h"
using std::string;
using std::vector;

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
            if (start_of_unit_part == notation_part.begin()) {
                calculations.emplace("1", notation_part);
            } else {
                calculations.emplace(
                    notation_part.substr(0, std::distance(notation_part.begin(),
                                                          start_of_unit_part)),
                    notation_part.substr(std::distance(notation_part.begin(),
                                                       start_of_unit_part)));
            }
        }
    }

    return calculations.top();
}

int main() {
    while (true) {
        std::cout << ">>> ";
        std::string input;
        std::getline(std::cin, input);

        if (input == "q") {
            break;
        }

        try {
            auto res = to_postfix_notation(input);
            for (const auto &part : res) {
                std::cout << part << '\n';
            }
            std::cout << '\n';

            auto calc_res = calculate(res);

            std::cout << calc_res << '\n';
        } catch (const std::exception &err) {
            std::cout << err.what() << '\n';
        }
    }

    return 0;
}
