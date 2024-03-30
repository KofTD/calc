#include "variables.h"
const std::map<std::string, std::string> premade_vars = {
    {"G", "(6.67430 * 10 ** -11) * m ** 3 * s ** -2 * kg **-1"          },
    {"R", "8.31446261815324 J / (mol * K)"                              },
    {"E", "(8.85418781762039 * 10**-12) * m **-3 * kg**-1 * s**4 * A**2"},
};
std::map<std::string, expression::operand> load_variables() {
    std::map<std::string, expression::operand> variables;
    for (const auto& [var_name, expr] : premade_vars) {
        auto postfix_notation = to_postfix_notation(split_into_tokens(expr));
        variables[var_name] = calculate(postfix_notation, {});
    }

    return variables;
}
