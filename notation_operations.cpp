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

std::vector<std::string> recombine_tokens(std::vector<std::string> tokens) {
    std::vector<std::string> new_tokens;

    tokens.emplace_back(" ");

    for (size_t i = 0; i < tokens.size() - 1; i++) {
        if (tokens[i] == "*" and tokens[i + 1] == "*") {
            new_tokens.emplace_back("**");
            i++;
        } else if (tokens[i] == "-" and tokens[i + 1] == ">") {
            new_tokens.emplace_back("->");
            i++;
        } else {
            new_tokens.push_back(tokens[i]);
        }
    }

    return new_tokens;
}

std::vector<std::string> split_into_tokens(const std::string& infix_notation) {
    namespace sr = std::ranges;
    using namespace std::literals;

    std::vector<std::string> tokens;
    std::vector<char> operators = {'(', '+', '-', '/', '*', ')', '>'};

    std::string operand;
    for (auto sym : infix_notation) {
        if (sym == ' ') {
            continue;
        }

        if (sr::find(operators, sym) != sr::end(operators)) {
            if (!operand.empty()) {
                tokens.push_back(operand);
                operand.clear();
            }
            tokens.emplace_back(""s + sym);
        } else {
            operand += sym;
        }
    }

    if (!operand.empty()) {
        tokens.push_back(operand);
        operand.clear();
    }

    return tokens;
}

std::vector<std::string> to_postfix_notation(std::string infix_notation) {
    using std::string;
    namespace sr = std::ranges;

    auto tokens = recombine_tokens(split_into_tokens(infix_notation));

    std::vector<string> result;
    std::stack<string> stack;
    string operand;

    const std::vector<string> prefix_function = {
        "sin", "cos", "sqrt", "tan", "cot", "asin", "acos", "atan", "acot"};

    auto has_digits = [](const std::string& operand) {
        return sr::find_if(operand, [](auto chr) {
                   return isdigit(chr) != 0;
               }) != sr::end(operand);
    };

    infix_notation += ' ';

    for (const auto& token : tokens) {
        if (has_digits(token)) {  // NOLINT(bugprone-branch-clone)
            result.push_back(token);
        } else if (sr::find(prefix_function, token) !=
                       sr::end(prefix_function) ||
                   token == "(") {
            stack.push(token);
        } else if (operation_priority.contains(token)) {
            while (!stack.empty() && (operation_priority.at(stack.top()) >=
                                      operation_priority.at(token))) {
                result.push_back(stack.top());
                stack.pop();
            }
            stack.push(token);
        } else if (token == ")") {
            while (stack.top() != "(") {
                result.push_back(stack.top());
                stack.pop();
            }
            stack.pop();

            if (!stack.empty() && sr::find(prefix_function, stack.top()) !=
                                      sr::end(prefix_function)) {
                result.push_back(stack.top());
                stack.pop();
            }
        } else {
            result.push_back(token);
        }
    }

    while (!stack.empty()) {
        result.push_back(stack.top());
        stack.pop();
    }

    return result;
}
