#ifndef CALCULATOR_OPERAND_H
#define CALCULATOR_OPERAND_H

#include <array>
#include <cstdint>
#include <map>
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

    std::pair<prefixes, std::string> split_prefix_and_unit(std::string prefix_and_unit);

    std::array<int8_t, 7> convert_to_basic_units(std::string unit) {
        // s, m, g, A, K, mol, cd
        if (unit == "Hz")
            return {-1, 0, 0, 0, 0, 0, 0};
        else if (unit == "N")
            return {-2, 1, 3, 0, 0, 0, 0};
        else if (unit == "Pa")
            return {-2, -1, 3, 0, 0, 0, 0};
        else if (unit == "J")
            return {-2, 2, 3, 0, 0, 0, 0};
        else if (unit == "W")
            return {-3, 2, 3, 0, 0, 0, 0};
        else if (unit == "C")
            return {1, 0, 0, 1, 0, 0, 0};
        else if (unit == "V")
            return {-3, 2, 3, -1, 0, 0, 0};
        else if (unit == "F")
            return {4, -2, -3, 2, 0, 0, 0};
        else if (unit == "ohm")
            return {-3, 2, 3, -2, 0, 0, 0};
        else if (unit == "S")
            return {3, -2, -3, 2, 0, 0, 0};
        else if (unit == "Wb")
            return {-2, 2, 3, -1, 0, 0, 0};
        else if (unit == "T")
            return {-2, 0, 3, -1, 0, 0, 0};
        else if (unit == "H")
            return {-2, 2, 3, -2, 0, 0, 0};
        else if (unit == "lm")
            return {0, 0, 0, 0, 0, 0, 1};
        else if (unit == "lx")
            return {0, -2, 0, 0, 0, 0, 1};
        else if (unit == "Bq")
            return {-1, 0, 0, 0, 0, 0, 0};
        else if (unit == "Gy" || unit == "Sv")
            return {-2, 2, 0, 0, 0, 0, 0};
        else if (unit == "kat")
            return {-1, 0, 0, 0, 0, 1, 0};
        else
            throw std::exception();
    }

    class operand {
    public:
        operand(const std::string &number) {
            this->number = std::stod(number);
            this->prefix = prefixes::null;
            this->basic_units_powers.fill(0);
        }
        operand(const std::string &number, std::string unit) {
            this->number = std::stod(number);

            auto prefix_and_unit = split_prefix_and_unit(unit);

            this->prefix = prefix_and_unit.first;
            this->basic_units_powers = convert_to_basic_units(prefix_and_unit.second);
        }

    private:
        double number;
        prefixes prefix;
        std::array<int8_t, 7> basic_units_powers;// s, m, g, A, K, mol, cd
    };

}// namespace expression

#endif//CALCULATOR_OPERAND_H
