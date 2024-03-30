#ifndef CALCULATOR_CALCULATIONS_H
#define CALCULATOR_CALCULATIONS_H

#include <stack>
#include <string>
#include <vector>

#include "operand.h"

expression::operand mfunction_call(const std::string &function,
                                   expression::operand oper);

expression::operand calculate(
    const std::vector<std::string> &postfix_notation,
    std::map<std::string, expression::operand> variables);

#endif  // CALCULATOR_CALCULATIONS_H
