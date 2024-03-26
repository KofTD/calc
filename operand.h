#ifndef CALCULATOR_OPERAND_H
#define CALCULATOR_OPERAND_H

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <map>
#include <ostream>
#include <ranges>
#include <string>

namespace expression {
using std::int8_t;

constexpr int8_t n_of_base_units = 7;

const std::map<std::string, int8_t> prefixes = {
    {"Q", 30},  {"R", 27},  {"Y", 24},   {"Z", 21},  {"E", 18},  {"P", 15},
    {"T", 12},  {"G", 9},   {"M", 6},    {"k", 3},   {"da", 1},  {"d", -1},
    {"c", -2},  {"m", -3},  {"mic", -6}, {"n", -9},  {"p", -12}, {"f", -15},
    {"a", -18}, {"z", -21}, {"y", -24},  {"r", -27}, {"q", -30}};

// enum class prefixes : int8_t {
//   quetta = 30,
//   ronna = 27,
//   yotta = 24,
//   zetta = 21,
//   exa = 18,
//   peta = 15,
//   tera = 12,
//   giga = 9,
//   mega = 6,
//   kilo = 3,
//   hecto = 2,
//   deca = 1,
//   null = 0,
//   deci = -1,
//   centi = -2,
//   milli = -3,
//   micro = -6,
//   nano = -9,
//   pico = -12,
//   femto = -15,
//   atto = -18,
//   zepto = -21,
//   yocto = -24,
//   ronto = -27,
//   quecto = -30
// };

const std::map<std::string, std::array<int8_t, n_of_base_units>>
    physical_units = {
        {"s", {1, 0, 0, 0, 0, 0, 0}},   {"m", {0, 1, 0, 0, 0, 0, 0}},
        {"g", {0, 0, 1, 0, 0, 0, 0}},   {"A", {0, 0, 0, 1, 0, 0, 0}},
        {"K", {0, 0, 0, 0, 1, 0, 0}},   {"mol", {0, 0, 0, 0, 0, 1, 0}},
        {"cd", {0, 0, 0, 0, 0, 0, 1}},  {"lm", {0, 0, 0, 0, 0, 0, 1}},
        {"Hz", {-1, 0, 0, 0, 0, 0, 0}}, {"Bq", {-1, 0, 0, 0, 0, 0, 0}},
        {"N", {-2, 1, 3, 0, 0, 0, 0}},  {"Pa", {-2, -1, 3, 0, 0, 0, 0}},
        {"J", {-2, 2, 3, 0, 0, 0, 0}},  {"W", {-3, 2, 3, 0, 0, 0, 0}},
        {"C", {1, 0, 0, 1, 0, 0, 0}},   {"V", {-3, 2, 3, -1, 0, 0, 0}},
        {"F", {4, -2, -3, 2, 0, 0, 0}}, {"ohm", {-3, 2, 3, -2, 0, 0, 0}},
        {"S", {3, -2, -3, 2, 0, 0, 0}}, {"Wb", {-2, 2, 3, -1, 0, 0, 0}},
        {"T", {-2, 0, 3, -1, 0, 0, 0}}, {"H", {-2, 2, 3, -2, 0, 0, 0}},
        {"lx", {0, -2, 0, 0, 0, 0, 1}}, {"Gy", {-2, 2, 0, 0, 0, 0, 0}},
        {"Sv", {-2, 2, 0, 0, 0, 0, 0}}, {"kat", {-1, 0, 0, 0, 0, 1, 0}}};

double ten_to_pow(int8_t power);

std::pair<int8_t, std::string>
split_prefix_and_unit(const std::string &prefix_and_unit);

std::array<int8_t, n_of_base_units>
convert_to_basic_units(const std::string &unit);

bool is_equal_powers(std::array<int8_t, n_of_base_units> lhs,
                     std::array<int8_t, n_of_base_units> rhs);

std::array<int8_t, n_of_base_units>
sum_b_u_powers(std::array<int8_t, n_of_base_units> lhs,
               std::array<int8_t, n_of_base_units> rhs);

std::array<int8_t, n_of_base_units>
dif_b_u_powers(std::array<int8_t, n_of_base_units> lhs,
               std::array<int8_t, n_of_base_units> rhs);

class operand {
public:
  explicit operand(const std::string &number) : number(std::stod(number)) {}

  operand(const std::string &number, const std::string &unit)
      : number(
            std::stod(number)) { // NOLINT(bugprone-easily-swappable-parameters)
    auto prefix_and_unit = split_prefix_and_unit(unit);

    this->prefix = prefix_and_unit.first;
    this->basic_units_powers = convert_to_basic_units(prefix_and_unit.second);
  }

  operand operator+(const operand &rhs) const {
    if (!is_equal_powers(this->basic_units_powers, rhs.basic_units_powers)) {
      throw std::exception();
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
  operand operator-(const operand &rhs) const {
    if (!is_equal_powers(this->basic_units_powers, rhs.basic_units_powers)) {
      throw std::exception();
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
  operand operator*(const operand &rhs) const {
    double n_number = this->number *
                      ten_to_pow(static_cast<int8_t>(this->prefix)) *
                      rhs.number * ten_to_pow(static_cast<int8_t>(rhs.prefix));
    int8_t n_prefix = std::min(this->prefix, rhs.prefix);

    return {n_number, n_prefix,
            sum_b_u_powers(this->basic_units_powers, rhs.basic_units_powers)};
  }
  operand operator/(const operand &rhs) const {
    double n_number = this->number *
                      ten_to_pow(static_cast<int8_t>(this->prefix)) /
                      rhs.number * ten_to_pow(static_cast<int8_t>(rhs.prefix));
    int8_t n_prefix = std::min(this->prefix, rhs.prefix);

    return {n_number, n_prefix,
            dif_b_u_powers(this->basic_units_powers, rhs.basic_units_powers)};
  }

  [[nodiscard]] operand power(const operand &rhs) const {
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

  friend std::ostream &operator<<(std::ostream &out, const operand &oper) {
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

protected:
  operand(double number, int8_t prefix,
          std::array<int8_t, n_of_base_units> b_u_powers)
      : number(number), prefix(prefix), basic_units_powers(b_u_powers) {}

private:
  double number;
  int8_t prefix = 0;
  std::array<int8_t, n_of_base_units> basic_units_powers = {
      0, 0, 0, 0, 0, 0, 0}; // s, m, g, A, K, mol, cd
};

} // namespace expression

#endif // CALCULATOR_OPERAND_H
