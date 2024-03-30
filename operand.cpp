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
        throw expression::unknown_prefix_err("Unknown prefix: " + prefix);
    }

    return prefixes.at(prefix);
}

std::array<int8_t, n_of_base_units> convert_to_basic_units(
    const std::string &unit) {
    if (!physical_units.contains(unit)) {
        throw expression::unknown_unit_err("Unknown unit: " + unit);
    }

    return physical_units.at(unit);
}

std::pair<int8_t, std::array<int8_t, n_of_base_units>> split_prefix_and_unit(
    const std::string &prefix_and_unit) {
    std::pair<int8_t, std::array<int8_t, n_of_base_units>> result;

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

std::array<int8_t, n_of_base_units> dif_b_u_powers(
    std::array<int8_t, n_of_base_units> lhs,
    std::array<int8_t, n_of_base_units> rhs) {
    std::array<int8_t, n_of_base_units> result = {0, 0, 0, 0, 0, 0, 0};

    for (size_t i = 0; i < lhs.size(); ++i) {
        result.at(i) = static_cast<int8_t>(lhs.at(i) - rhs.at(i));
    }

    return result;
}

std::array<int8_t, n_of_base_units> sum_b_u_powers(
    std::array<int8_t, n_of_base_units> lhs,
    std::array<int8_t, n_of_base_units> rhs) {
    std::array<int8_t, n_of_base_units> result = {0, 0, 0, 0, 0, 0, 0};

    for (size_t i = 0; i < lhs.size(); ++i) {
        result.at(i) = static_cast<int8_t>(lhs.at(i) + rhs.at(i));
    }

    return result;
}

bool is_equal(std::array<int8_t, n_of_base_units> lhs,
              std::array<int8_t, n_of_base_units> rhs) {
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

unknown_unit_err::unknown_unit_err(std::string message)
    : message(std::move(message)) {}
unknown_prefix_err::unknown_prefix_err(std::string message)
    : message(std::move(message)) {}
non_additive_units_err::non_additive_units_err(std::string message)
    : message(std::move(message)) {}

operand operand::operator+(const operand &rhs) const {
    if (!is_equal(this->basic_units_powers, rhs.basic_units_powers)) {
        throw non_additive_units_err("Non-additive units");
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
        throw non_additive_units_err("Non-additive units");
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
    double n_number = this->number *
                      ten_to_pow(static_cast<int8_t>(this->prefix)) *
                      rhs.number * ten_to_pow(static_cast<int8_t>(rhs.prefix));
    int8_t n_prefix = std::min(this->prefix, rhs.prefix);

    return {n_number, n_prefix,
            sum_b_u_powers(this->basic_units_powers, rhs.basic_units_powers)};
}

operand operand::operator/(const operand &rhs) const {
    double n_number = this->number *
                      ten_to_pow(static_cast<int8_t>(this->prefix)) /
                      rhs.number * ten_to_pow(static_cast<int8_t>(rhs.prefix));
    int8_t n_prefix = std::min(this->prefix, rhs.prefix);

    return {n_number, n_prefix,
            dif_b_u_powers(this->basic_units_powers, rhs.basic_units_powers)};
}

operand operand::power(const operand &rhs) const {
    if (!is_equal(rhs.basic_units_powers, {0, 0, 0, 0, 0, 0, 0})) {
        throw std::exception();
    }

    double n_number = std::pow(this->number, rhs.number);

    std::array<int8_t, n_of_base_units> n_powers = {0, 0, 0, 0, 0, 0, 0};

    for (size_t i = 0; i < n_powers.size(); ++i) {
        n_powers.at(i) = static_cast<int8_t>(basic_units_powers.at(i) *
                                             static_cast<int8_t>(rhs.number));
    }

    return {n_number, this->prefix, n_powers};
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
        } else {
            auto prefix_and_unit = split_prefix_and_unit(unit_part);

            this->prefix = prefix_and_unit.first;
            this->basic_units_powers = prefix_and_unit.second;
        }
    }
}

std::string operand::to_string() {
    std::stringstream result;

    result << this->number << ' ';

    std::string unit;
    for (const auto &[ph_unit, unit_powers] : physical_units) {
        if (is_equal(unit_powers, this->basic_units_powers)) {
            unit = ph_unit;
        }
    }

    if (this->prefix != 0 && !unit.empty()) {
        for (const auto &[prefix, prefix_power] : prefixes) {
            if (prefix_power == this->prefix) {
                result << prefix;
                break;
            }
        }
    }

    if (unit.empty()) {
        for (size_t i = 0; i < this->basic_units_powers.size(); i++) {
            if (this->basic_units_powers.at(i) == 0) {
                continue;
            }
            switch (i) {
                case 0:
                    result << "s";
                    break;
                case 1:
                    result << "m";
                    break;
                case 2:
                    result << "g";
                    break;
                case 3:
                    result << "A";
                    break;
                case 4:
                    result << "K";
                    break;
                case 5:
                    result << "mol";
                    break;
                case 6:
                    result << "cd";
                    break;
            }

            result << "^" << static_cast<int>(basic_units_powers.at(i)) << '*';
        }
    } else {
        result << unit;
    }

    return result.str().substr(0, result.str().length() - 1);
};
}  // namespace expression
