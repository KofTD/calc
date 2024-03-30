#include "calculations.h"

#include <cmath>

#include "notation_operations.h"
#include "operand.h"
expression::operand calculate(
    const std::vector<std::string> &postfix_notation,
    std::map<std::string, expression::operand> variables) {
    using namespace std::literals;
    namespace sr = std::ranges;
    std::stack<expression::operand> calculations;
    for (const std::string &token : postfix_notation) {
        if (operation_priority.contains(token)) {
            if (token == "->") {
                auto op1 = calculations.top();
                calculations.pop();
                auto op2 = calculations.top();
                calculations.pop();

                op2.convert(op1);
                calculations.push(op2);
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

        } else if (auto ptr = sr::find(prefix_functions, token);
                   ptr != sr::end(prefix_functions)) {
            auto op = calculations.top();
            calculations.pop();
            calculations.push(mfunction_call(*ptr, op));
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
expression::operand mfunction_call(const std::string &function,
                                   expression::operand oper) {
    using expression::operand;
    using std::to_string;
    if (function != "sqrt") {
        if (!expression::is_equal(oper.get_basic_units_powers(),
                                  {0, 0, 0, 0, 0, 0, 0})) {
            throw expression::function_err("Unit must be a scalar");
        }
    }
    if (function == "sin") {
        return operand(to_string(std::sin(oper.get_number())));
    }
    if (function == "cos") {
        return operand(to_string(std::cos(oper.get_number())));
    }
    if (function == "tan") {
        return operand(to_string(std::tan(oper.get_number())));
    }
    if (function == "cot") {
        return operand(to_string(1 / std::tan(oper.get_number())));
    }
    if (function == "sqrt") {
        oper.sqrt();
        return oper;
    }
    if (function == "asin") {
        return operand(to_string(std::asin(oper.get_number())));
    }
    if (function == "acos") {
        return operand(to_string(std::acos(oper.get_number())));
    }
    if (function == "atan") {
        return operand(to_string(std::atan(oper.get_number())));
    }
}
