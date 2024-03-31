#ifndef CALCULATOR_OPERAND_H
#define CALCULATOR_OPERAND_H

#include <array>
#include <cstdint>
#include <map>
#include <ostream>
#include <string>

namespace expression {
using std::int8_t;

// Constants
constexpr int8_t n_of_base_units = 7;

const std::map<std::string, int8_t> prefixes = {
    {"Q",   30 },
    {"R",   27 },
    {"Y",   24 },
    {"Z",   21 },
    {"E",   18 },
    {"P",   15 },
    {"T",   12 },
    {"G",   9  },
    {"M",   6  },
    {"k",   3  },
    {"da",  1  },
    {"d",   -1 },
    {"c",   -2 },
    {"m",   -3 },
    {"mic", -6 },
    {"n",   -9 },
    {"p",   -12},
    {"f",   -15},
    {"a",   -18},
    {"z",   -21},
    {"y",   -24},
    {"r",   -27},
    {"q",   -30}
};

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

const std::map<std::string, std::array<float, n_of_base_units>> physical_units =
    {
        {"s",   {1, 0, 0, 0, 0, 0, 0}  },
        {"m",   {0, 1, 0, 0, 0, 0, 0}  },
        {"g",   {0, 0, 1, 0, 0, 0, 0}  },
        {"A",   {0, 0, 0, 1, 0, 0, 0}  },
        {"K",   {0, 0, 0, 0, 1, 0, 0}  },
        {"mol", {0, 0, 0, 0, 0, 1, 0}  },
        {"cd",  {0, 0, 0, 0, 0, 0, 1}  },
        {"lm",  {0, 0, 0, 0, 0, 0, 1}  },
        {"Hz",  {-1, 0, 0, 0, 0, 0, 0} },
        {"Bq",  {-1, 0, 0, 0, 0, 0, 0} },
        {"N",   {-2, 1, 3, 0, 0, 0, 0} },
        {"Pa",  {-2, -1, 3, 0, 0, 0, 0}},
        {"J",   {-2, 2, 3, 0, 0, 0, 0} },
        {"W",   {-3, 2, 3, 0, 0, 0, 0} },
        {"C",   {1, 0, 0, 1, 0, 0, 0}  },
        {"V",   {-3, 2, 3, -1, 0, 0, 0}},
        {"F",   {4, -2, -3, 2, 0, 0, 0}},
        {"ohm", {-3, 2, 3, -2, 0, 0, 0}},
        {"S",   {3, -2, -3, 2, 0, 0, 0}},
        {"Wb",  {-2, 2, 3, -1, 0, 0, 0}},
        {"T",   {-2, 0, 3, -1, 0, 0, 0}},
        {"H",   {-2, 2, 3, -2, 0, 0, 0}},
        {"lx",  {0, -2, 0, 0, 0, 0, 1} },
        {"Gy",  {-2, 2, 0, 0, 0, 0, 0} },
        {"Sv",  {-2, 2, 0, 0, 0, 0, 0} },
        {"kat", {-1, 0, 0, 0, 0, 1, 0} },
        {"m/s", {-1, 1, 0, 0, 0, 0, 0} }
};

// --------------------------------------------

double ten_to_pow(float power);

bool is_equal(std::array<float, n_of_base_units> lhs,
              std::array<float, n_of_base_units> rhs);

std::array<float, n_of_base_units> sum_b_u_powers(
    std::array<float, n_of_base_units> lhs,
    std::array<float, n_of_base_units> rhs);

std::array<float, n_of_base_units> dif_b_u_powers(
    std::array<float, n_of_base_units> lhs,
    std::array<float, n_of_base_units> rhs);

class operand {
   public:
    // explicit operand(const std::string &number) : number(std::stod(number))
    // {}

    explicit operand(std::string token);

    operand() = default;
    operand(const operand &) = default;
    operand(operand &&) = default;

    operand &operator=(const operand &) = default;
    operand &operator=(operand &&) = default;

    operand operator+(const operand &rhs) const;
    operand operator-(const operand &rhs) const;
    operand operator*(const operand &rhs) const;
    operand operator/(const operand &rhs) const;

    [[nodiscard]] operand power(const operand &rhs) const;
    void convert(const operand &rhs);

    [[nodiscard]] std::wstring to_wstring() const;

    [[nodiscard]] double get_number() const;
    [[nodiscard]] const std::array<float, n_of_base_units> &
    get_basic_units_powers() const;

    void sqrt();

   protected:
    operand(double number, int8_t prefix,
            std::array<float, n_of_base_units> b_u_powers)
        : number(number), prefix(prefix), basic_units_powers(b_u_powers) {}

   private:
    double number{0};
    int8_t prefix = 0;
    std::array<float, n_of_base_units> basic_units_powers = {
        0, 0, 0, 0, 0, 0, 0};  // Order: s, m, g, A, K, mol, cd
};

// Errors
class unknown_unit_part_error : public std::exception {
   private:
    std::string message;

   public:
    explicit unknown_unit_part_error(std::string message);
    [[nodiscard]] const char *what() const noexcept override {
        return message.c_str();
    }
};

class unequal_units_err : public std::exception {
   private:
    std::string sys_message;
    std::wstring output_message;

   public:
    const std::wstring &get_output_message() const;
    explicit unequal_units_err(std::string message);
    unequal_units_err(std::string sysMessage, std::wstring outputMessage);
    [[nodiscard]] const char *what() const noexcept override {
        return sys_message.c_str();
    }
};
class function_err : public std::exception {
   private:
    std::string message;

   public:
    explicit function_err(std::string message);
    [[nodiscard]] const char *what() const noexcept override {
        return message.c_str();
    }
};
}  // namespace expression

#endif  // CALCULATOR_OPERAND_H
