#ifndef CALCULATOR_NOTATION_OPERATIONS_H
#define CALCULATOR_NOTATION_OPERATIONS_H

#include <cstdint>
#include <map>
#include <string>
#include <vector>

const std::map<std::string, std::int8_t> operation_priority = {
    {"(",  0},
    {"->", 1},
    {"+",  1},
    {"-",  1},
    {"*",  2},
    {"/",  2},
    {"**", 3},
};

std::vector<std::string> to_postfix_notation(std::string infix_notation);

#endif  // CALCULATOR_NOTATION_OPERATIONS_H
