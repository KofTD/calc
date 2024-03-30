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

std::vector<std::string> split_into_tokens(const std::string& infix_notation);

std::vector<std::string> to_postfix_notation(std::vector<std::string> tokens);

class input_error : public std::exception {
   private:
    std::string message;

   public:
    explicit input_error(std::string message);
    [[nodiscard]] const char* what() const noexcept override {
        return message.c_str();
    };
};

const std::vector<std::string> prefix_functions = {
    "sin", "cos", "sqrt", "tan", "cot", "asin", "acos", "atan"};

#endif  // CALCULATOR_NOTATION_OPERATIONS_H
