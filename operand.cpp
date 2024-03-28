#include "operand.h"

#include <utility>

namespace expression {
int8_t str_to_prefix(const std::string &prefix) {
    if (!prefixes.contains(prefix)) {
        throw expression::unknown_prefix_err("Unknown prefix: " + prefix);
    }

    return prefixes.at(prefix);
}

std::pair<int8_t, std::string> split_prefix_and_unit(
    const std::string &prefix_and_unit) {
    std::pair<int8_t, std::string> result;

    if (prefix_and_unit.length() >= 2) {
        auto sample = prefix_and_unit.substr(0, 3);

        if (sample == "mic") {
            result.first = prefixes.at("mic");
            result.second = prefix_and_unit.substr(3);
        } else if (sample.substr(0, 2) == "da") {
            result.first = prefixes.at("de");
            result.second = prefix_and_unit.substr(2);
        } else if (prefixes.contains(sample.substr(0, 1))) {
            result.first = str_to_prefix(sample.substr(0, 1));
            result.second = prefix_and_unit.substr(1);
        } else {
            result.first = 0;
            result.second = prefix_and_unit;
        }
    } else {
        result.first = 0;
        result.second = prefix_and_unit;
    }

    return result;
}
std::array<int8_t, n_of_base_units> convert_to_basic_units(
    const std::string &unit) {
    if (!physical_units.contains(unit)) {
        throw expression::unknown_unit_err("Unknown unit: " + unit);
    }

    return physical_units.at(unit);
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
bool is_equal_powers(std::array<int8_t, n_of_base_units> lhs,
                     std::array<int8_t, n_of_base_units> rhs) {
    for (size_t i = 0; i < lhs.size(); i++) {
        if (lhs.at(i) != rhs.at(i)) {
            return false;
        }
    }
    return true;
}

double ten_to_pow(int8_t power) {
    double result = 1;
    if (power > 0) {
        for (int8_t _ = 0; _ < power; _++) {
            result *= 10;  // NOLINT
        }
    } else if (power < 0) {
        for (int8_t _ = 0; _ < power; _++) {
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
operand::operand(const std::string &number, const std::string &unit)
    : number(
          std::stod(number)) {  // NOLINT(bugprone-easily-swappable-parameters)
    auto prefix_and_unit = split_prefix_and_unit(unit);

    this->prefix = prefix_and_unit.first;
    this->basic_units_powers = convert_to_basic_units(prefix_and_unit.second);
}
operand operand::operator+(const operand &rhs) const {
    if (!is_equal_powers(this->basic_units_powers, rhs.basic_units_powers)) {
        throw non_additive_units_err("Non-additive units");
    }

    double n_number =
        (this->number * ten_to_pow(static_cast<int8_t>(this->prefix))) +
        (rhs.number * ten_to_pow(static_cast<int8_t>(rhs.prefix)));
    int8_t n_prefix = std::min(this->prefix, rhs.prefix);

    if (static_cast<int8_t>(n_prefix) != 0) {
        n_number /= static_cast<double>(n_prefix);
    }

    return {n_number, n_prefix, this->basic_units_powers};
}
operand operand::operator-(const operand &rhs) const {
    if (!is_equal_powers(this->basic_units_powers, rhs.basic_units_powers)) {
        throw non_additive_units_err("Non-additive units");
    }

    double n_number =
        (this->number * ten_to_pow(static_cast<int8_t>(this->prefix))) -
        (rhs.number * ten_to_pow(static_cast<int8_t>(rhs.prefix)));
    int8_t n_prefix = std::min(this->prefix, rhs.prefix);

    if (static_cast<int8_t>(n_prefix) != 0) {
        n_number /= static_cast<double>(n_prefix);
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
    if (!is_equal_powers(rhs.basic_units_powers, {0, 0, 0, 0, 0, 0, 0})) {
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
std::ostream &operator<<(std::ostream &out, const operand &oper) {
    using std::ranges::views::keys;

    out << oper.number;

    for (const auto &[prefix, prefix_power] : prefixes) {
        if (prefix_power == oper.prefix) {
            out << prefix;
            break;
        }
    }

    for (const auto &[unit, unit_powers] : physical_units) {
        if (is_equal_powers(unit_powers, oper.basic_units_powers)) {
            out << unit;
            break;
        }
    }

    return out;
}
}  // namespace expression
