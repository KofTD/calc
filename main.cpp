#include <algorithm>
#include <cctype>
#include <iostream>
#include <stack>
#include <vector>

#include "calculations.h"
#include "notation_operations.h"
#include "operand.h"
#include "variables.h"

using std::cout;
using std::string;
using std::vector;

int main() {
    auto variables = load_variables();
    while (true) {
        cout << ">>> ";
        std::string input;
        std::getline(std::cin, input);

        if (input == "q" || input == "exit") {
            break;
        }

        try {
            auto tokens = split_into_tokens(input);

            if (tokens[0].substr(0, 3) == "let") {
                auto var_name = tokens[0].substr(3);
                std::vector<std::string> expr_tokens{tokens.begin() + 2,
                                                     tokens.end()};

                variables[var_name] =
                    calculate(to_postfix_notation(expr_tokens), variables);
                continue;
            }
            if (tokens[0] == "list") {
                for (const auto &[var_name, var_value] : variables) {
                    std::wcout << std::wstring(var_name.begin(), var_name.end())
                               << '\t' << var_value.to_wstring() << std::endl;
                }
                continue;
            }

            auto postfix_notation = to_postfix_notation(tokens);

            // for (const auto &token : postfix_notation) cout << token << '\n';

            auto calculation_result = calculate(postfix_notation, variables);
            variables["ans"] = calculation_result;
            std::wcout << calculation_result.to_wstring() << '\n';
        } catch (const expression::unequal_units_err &err) {
            const auto &output_message = err.get_output_message();
            if (output_message.empty()) {
                cout << err.what() << '\n';
            } else {
                cout << err.what() << '\n';
                std::wcout << output_message << '\n';
            }
        } catch (const std::exception &err) {
            cout << err.what() << '\n';
        }
    }

    return 0;
}
