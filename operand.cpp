#include "operand.h"

#include <algorithm>
#include <cmath>
#include <ranges>
#include <sstream>
#include <string>
#include <utility>

namespace expression {
int8_t str_to_prefix(const std::string &prefix) {
    if (!prefixes.contains(prefix)) {
        throw expression::unknown_unit_part_error("Unknown prefix: " + prefix);
    }

    return prefixes.at(prefix);
}

std::array<float, n_of_base_units> convert_to_basic_units(
    const std::string &unit) {
    if (!physical_units.contains(unit)) {
        throw expression::unknown_unit_part_error("Unknown unit: " + unit);
    }

    return physical_units.at(unit);
}

std::pair<int8_t, std::array<float, n_of_base_units>> split_prefix_and_unit(
    const std::string &prefix_and_unit) {
    std::pair<int8_t, std::array<float, n_of_base_units>> result;

    if (prefix_and_unit.length() >= 2) {
        auto sample = prefix_and_unit.substr(0, 3);

        if (sample == "mic") {
            result.first = prefixes.at("mic");
            result.second = convert_to_basic_units(prefix_and_unit.substr(3));
        } else if (sample.substr(0, 2) == "da") {
            result.first = prefixes.at("de");
            result.second = convert_to_basic_units(prefix_and_unit.substr(2));
        } else if (prefixes.contains(sample.substr(0, 1))) {
            result.first = str_to_prefix(sample.substr(0, 1));
            result.second = convert_to_basic_units(prefix_and_unit.substr(1));
        } else {
            result.first = 0;
            result.second = convert_to_basic_units(prefix_and_unit);
        }
    } else {
        result.first = 0;
        result.second = convert_to_basic_units(prefix_and_unit);
    }

    return result;
}

std::array<float, n_of_base_units> dif_b_u_powers(
    std::array<float, n_of_base_units> lhs,
    std::array<float, n_of_base_units> rhs) {
    std::array<float, n_of_base_units> result = {0, 0, 0, 0, 0, 0, 0};

    for (size_t i = 0; i < lhs.size(); ++i) {
        result.at(i) = static_cast<int8_t>(lhs.at(i) - rhs.at(i));
    }

    return result;
}

std::array<float, n_of_base_units> sum_b_u_powers(
    std::array<float, n_of_base_units> lhs,
    std::array<float, n_of_base_units> rhs) {
    std::array<float, n_of_base_units> result = {0, 0, 0, 0, 0, 0, 0};

    for (size_t i = 0; i < lhs.size(); ++i) {
        result.at(i) = static_cast<int8_t>(lhs.at(i) + rhs.at(i));
    }

    return result;
}

bool is_equal(std::array<float, n_of_base_units> lhs,
              std::array<float, n_of_base_units> rhs) {
    return std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

double ten_to_pow(int8_t power) {
    double result = 1;
    if (power > 0) {
        for (int _ = 0; _ < power; _++) {
            result *= 10;  // NOLINT
        }
    } else if (power < 0) {
        for (int _ = 0; _ < abs(power); _++) {
            result /= 10;  // NOLINT
        }
    }
    return result;
}

unequal_units_err::unequal_units_err(std::string message)
    : sys_message(std::move(message)) {}
unequal_units_err::unequal_units_err(std::string sysMessage,
                                     std::wstring outputMessage)
    : sys_message(std::move(sysMessage)),
      output_message(std::move(outputMessage)) {}
const std::wstring &unequal_units_err::get_output_message() const {
    return output_message;
}

operand operand::operator+(const operand &rhs) const {
    if (!is_equal(this->basic_units_powers, rhs.basic_units_powers)) {
        throw unequal_units_err("Non-additive units");
    }

    double n_number = (this->number * ten_to_pow(this->prefix)) +
                      (rhs.number * ten_to_pow(rhs.prefix));
    int8_t n_prefix = std::min(this->prefix, rhs.prefix);

    if (n_prefix != 0) {
        n_number /= static_cast<double>(ten_to_pow(n_prefix));
    }

    return {n_number, n_prefix, this->basic_units_powers};
}

operand operand::operator-(const operand &rhs) const {
    if (!is_equal(this->basic_units_powers, rhs.basic_units_powers)) {
        throw unequal_units_err("Non-additive units");
    }

    double n_number = (this->number * ten_to_pow(this->prefix)) -
                      (rhs.number * ten_to_pow(rhs.prefix));
    int8_t n_prefix = std::min(this->prefix, rhs.prefix);

    if (n_prefix != 0) {
        n_number /= static_cast<double>(ten_to_pow(n_prefix));
    }

    return {n_number, n_prefix, this->basic_units_powers};
}

operand operand::operator*(const operand &rhs) const {
    double n_number = this->number * rhs.number;
    auto n_prefix = static_cast<int8_t>(this->prefix + rhs.prefix);

    return {n_number, n_prefix,
            sum_b_u_powers(this->basic_units_powers, rhs.basic_units_powers)};
}

operand operand::operator/(const operand &rhs) const {
    double n_number = this->number / rhs.number;
    auto n_prefix = static_cast<int8_t>(this->prefix - rhs.prefix);

    return {n_number, n_prefix,
            dif_b_u_powers(this->basic_units_powers, rhs.basic_units_powers)};
}

operand operand::power(const operand &rhs) const {
    if (!is_equal(rhs.basic_units_powers, {0, 0, 0, 0, 0, 0, 0})) {
        throw function_err("Power must be a scalar");
    }

    double n_number = std::pow(this->number, rhs.number);

    std::array<float, n_of_base_units> n_powers = {0, 0, 0, 0, 0, 0, 0};

    for (size_t i = 0; i < n_powers.size(); ++i) {
        n_powers.at(i) = static_cast<int8_t>(basic_units_powers.at(i) *
                                             static_cast<int8_t>(rhs.number));
    }

    return {n_number, static_cast<int8_t>(this->prefix * rhs.number), n_powers};
}

operand::operand(std::string token) {
    auto alpha_start =
        std::ranges::find_if(token, [](char sym) { return isalpha(sym) != 0; });

    if (alpha_start == token.end()) {
        this->number = std::stod(token);
        this->prefix = 0;
        this->basic_units_powers = {0, 0, 0, 0, 0, 0, 0};
    } else {
        this->number = std::stod(
            token.substr(0, std::distance(token.begin(), alpha_start)));

        auto unit_part =
            token.substr(std::distance(token.begin(), alpha_start));

        if (unit_part == "h") {
            this->number *= 3600;  // NOLINT
            this->prefix = 0;
            this->basic_units_powers = {1, 0, 0, 0, 0, 0, 0};
        } else if (unit_part == "l") {
            this->number *= 0.001;
            this->prefix = 0;
            this->basic_units_powers = {0, 3, 0, 0, 0, 0, 0};
        } else if (unit_part == "deg") {
            this->number *= 3.14159265358979323846 / 180;
            this->prefix = 0;
            this->basic_units_powers = {0, 0, 0, 0, 0, 0, 0};
        } else if (unit_part == "mol") {
            this->prefix = 0;
            this->basic_units_powers = {0, 0, 0, 0, 0, 1, 0};
        } else {
            auto prefix_and_unit = split_prefix_and_unit(unit_part);

            this->prefix = prefix_and_unit.first;
            this->basic_units_powers = prefix_and_unit.second;
        }
    }
}

std::wstring unit_powers_to_string(std::array<float, n_of_base_units> powers) {
    std::wstringstream result;

    for (size_t i = 0; i < powers.size(); i++) {
        if (powers.at(i) == 0) {
            continue;
        }
        if (i == 0) {
            result << "s";
        } else if (i == 1) {
            result << "m";
        } else if (i == 2) {
            result << "g";
        } else if (i == 3) {
            result << "A";
        } else if (i == 4) {
            result << "K";
        } else if (i == 5) {
            result << "mol";
        } else {
            result << "cd";
        }

        auto power = std::to_string(static_cast<int>(powers.at(i)));

        for (const auto &chr : power) {
            if (chr == '-') {
                result << "⁻";
            } else if (chr == '1') {
                result << "¹";
            } else if (chr == '2') {
                result << "²";
            } else if (chr == '3') {
                result << "³";
            } else if (chr == '4') {
                result << "⁴";
            } else if (chr == '5') {
                result << "⁵";
            } else if (chr == '6') {
                result << "⁶";
            } else if (chr == '7') {
                result << "⁷";
            } else if (chr == '8') {
                result << "⁸";
            } else if (chr == '9') {
                result << "⁹";
            } else {
                result << "⁰";
            }
        }
        result << "∙";
    }

    return result.str().substr(0, result.str().length() - 3);
}

std::wstring operand::to_wstring() const {
    std::wstringstream result;

    std::string unit;
    for (const auto &[ph_unit, unit_powers] : physical_units) {
        if (is_equal(unit_powers, this->basic_units_powers)) {
            unit = ph_unit;
            break;
        }
    }

    std::string prefix;
    if (this->prefix != 0) {
        for (const auto &[ph_prefix, prefix_power] : prefixes) {
            if (prefix_power == this->prefix) {
                prefix = ph_prefix;
                break;
            }
        }
    }

    if (unit.empty()) {
        result << this->number * ten_to_pow(this->prefix) << ' '
               << unit_powers_to_string(this->basic_units_powers);
    } else {
        if (!prefix.empty()) {
            result << this->number << ' '
                   << std::wstring(prefix.begin(), prefix.end());
        } else {
            result << this->number * ten_to_pow(this->prefix) << ' ';
        }
        result << std::wstring(unit.begin(), unit.end());
    }

    return result.str();
};
bool can_convert(std::array<float, n_of_base_units> lhs,
                 std::array<float, n_of_base_units> rhs) {
    for (size_t i = 0; i < lhs.size(); ++i) {
        if (lhs.at(i) > 0) {
            if (rhs.at(i) == 0) return false;
        }
    }

    return true;
}
void operand::convert(const operand &rhs) {
    if (!can_convert(this->basic_units_powers, rhs.basic_units_powers)) {
        std::wstringstream message;
        message << "Left hand expression: ";

        message << unit_powers_to_string(this->basic_units_powers) << '\n';
        message << "Right hand expression: ";
        message << unit_powers_to_string(rhs.basic_units_powers);

        throw unequal_units_err("Can't convert units", message.str());
    }

    for (size_t i = 0; i < this->basic_units_powers.size(); i++) {
        if (this->basic_units_powers.at(i) == 0) continue;

        auto diff = static_cast<int8_t>(this->basic_units_powers.at(i) -
                                        rhs.basic_units_powers.at(i));

        if (diff != 0) diff -= 1;

        this->basic_units_powers.at(i) = rhs.basic_units_powers.at(i);

        this->number *= ten_to_pow(diff);
    }

    if (this->prefix != rhs.prefix) {
        this->number *=
            ten_to_pow(static_cast<int8_t>(this->prefix - rhs.prefix));
    }
}
double operand::get_number() const { return number; }
const std::array<float, n_of_base_units> &operand::get_basic_units_powers()
    const {
    return basic_units_powers;
}
void operand::sqrt() {
    this->number = std::sqrt(this->number);
    for (auto &power : basic_units_powers) power /= 2;
}
function_err::function_err(std::string message) : message(std::move(message)) {}
unknown_unit_part_error::unknown_unit_part_error(std::string message)
    : message(std::move(message)){};
}  // namespace expression
