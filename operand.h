#ifndef CALCULATOR_OPERAND_H
#define CALCULATOR_OPERAND_H

#include <array>
#include <cmath>
#include <cstdint>
#include <map>
#include <ostream>
#include <string>

namespace expression {
    using std::int8_t;

    enum class prefixes : int8_t {
        quetta = 30,
        ronna = 27,
        yotta = 24,
        zetta = 21,
        exa = 18,
        peta = 15,
        tera = 12,
        giga = 9,
        mega = 6,
        kilo = 3,
        hecto = 2,
        deca = 1,
        null = 0,
        deci = -1,
        centi = -2,
        milli = -3,
        micro = -6,
        nano = -9,
        pico = -12,
        femto = -15,
        atto = -18,
        zepto = -21,
        yocto = -24,
        ronto = -27,
        quecto = -30
    };

    std::pair<prefixes, std::string> split_prefix_and_unit(const std::string &prefix_and_unit);

    std::array<int8_t, 7> convert_to_basic_units(const std::string &unit);

    bool is_equal_powers(std::array<int8_t, 7> lhs, std::array<int8_t, 7> rhs);

    std::array<int8_t, 7> sum_b_u_powers(std::array<int8_t, 7> lhs, std::array<int8_t, 7> rhs);

    std::array<int8_t, 7> dif_b_u_powers(std::array<int8_t, 7> lhs, std::array<int8_t, 7> rhs);

    class operand {
    public:
        explicit operand(const std::string &number) {
            this->number = std::stod(number);
        }
        operand(const std::string &number, const std::string &unit) {
            this->number = std::stod(number);

            auto prefix_and_unit = split_prefix_and_unit(unit);

            this->prefix = prefix_and_unit.first;
            this->basic_units_powers = convert_to_basic_units(prefix_and_unit.second);
        }

        operand operator+(const operand &rhs) const {
            if (!is_equal_powers(this->basic_units_powers, rhs.basic_units_powers))
                throw std::exception();

            double n_number = (this->number * int8_t(this->prefix)) + (rhs.number * int8_t(rhs.prefix));
            prefixes n_prefix = std::min(this->prefix, rhs.prefix);

            if (static_cast<int8_t>(n_prefix) != 0)
                n_number /= static_cast<double>(n_prefix);

            return {n_number, n_prefix, this->basic_units_powers};
        }
        operand operator-(const operand &rhs) const {
            if (!is_equal_powers(this->basic_units_powers, rhs.basic_units_powers))
                throw std::exception();

            double n_number = (this->number * int8_t(this->prefix)) - (rhs.number * int8_t(rhs.prefix));
            prefixes n_prefix = std::min(this->prefix, rhs.prefix);

            if (static_cast<int8_t>(n_prefix) != 0)
                n_number /= static_cast<double>(n_prefix);

            return {n_number, n_prefix, this->basic_units_powers};
        }
        operand operator*(const operand &rhs) const {
            double n_number = this->number * int8_t(this->prefix) * rhs.number * int8_t(rhs.prefix);
            prefixes n_prefix = std::min(this->prefix, rhs.prefix);

            return {n_number, n_prefix, sum_b_u_powers(this->basic_units_powers, rhs.basic_units_powers)};
        }
        operand operator/(const operand &rhs) const {
            double n_number = this->number * int8_t(this->prefix) / rhs.number * int8_t(rhs.prefix);
            prefixes n_prefix = std::min(this->prefix, rhs.prefix);

            return {n_number, n_prefix, dif_b_u_powers(this->basic_units_powers, rhs.basic_units_powers)};
        }

        [[nodiscard]] operand power(const operand &rhs) const {
            if (!is_equal_powers(rhs.basic_units_powers, {0, 0, 0, 0, 0, 0, 0}))
                throw std::exception();

            double n_number = std::pow(this->number, rhs.number);

            std::array<int8_t, 7> n_powers = {0, 0, 0, 0, 0, 0, 0};

            for (size_t i = 0; i < n_powers.size(); ++i) {
                n_powers[i] = static_cast<int8_t>(basic_units_powers[i] * static_cast<int8_t>(rhs.number));
            }

            return {n_number, this->prefix, n_powers};
        }

        friend std::ostream &operator<<(std::ostream &out, const operand &op) {
            out << op.number;

            return out;
        }


    protected:
        operand(double number, prefixes prefix, std::array<int8_t, 7> b_u_powers) {
            this->number = number;
            this->prefix = prefix;
            this->basic_units_powers = b_u_powers;
        }

    private:
        double number;
        prefixes prefix = prefixes::null;
        std::array<int8_t, 7> basic_units_powers = {0, 0, 0, 0, 0, 0, 0};// s, m, g, A, K, mol, cd
    };


}// namespace expression

#endif//CALCULATOR_OPERAND_H
