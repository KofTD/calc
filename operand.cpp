#include "operand.h"
#include <map>

namespace expression {
    prefixes str_to_preffix(char preffix){
        using enum expression::prefixes;
        if (preffix == 'Q')
            return quetta;
        else if (preffix == 'R')
            return ronna;
        else if(preffix=='Y')
            return yotta;
        else if(preffix =='Z')
            return zetta;
        else if(preffix == 'E')
            return exa;
        else if(preffix == 'P')
            return peta;
        else if(preffix == 'T')
            return tera;
        else if(preffix == 'G')
            return giga;
        else if(preffix == 'M')
            return mega;
        else if(preffix == 'k')
            return kilo;
        else if(preffix =='h')
            return hecto;
        else if(preffix == 'd')
            return deci;
        else if(preffix == 'c')
            return centi;
        else if(preffix == 'm')
            return milli;
        else if(preffix == 'n')
            return nano;
        else if(preffix == 'p')
            return pico;
        else if(preffix == 'f')
            return femto;
        else if(preffix == 'a')
            return atto;
        else if(preffix == 'z')
            return zepto;
        else if(preffix == 'y')
            return yocto;
        else if(preffix == 'r')
            return ronto;
        else if(preffix == 'q')
            return quecto;
        else
            throw std::exception();
    }


    std::pair<prefixes, std::string> split_prefix_and_unit(const std::string& prefix_and_unit) {
        std::pair<prefixes, std::string> result;
        const std::string prefixes_string = "QRYZEPTGMkhdcmnpfazyrq";

        if (prefix_and_unit.length() >= 2) {
            auto sample = prefix_and_unit.substr(0, 3);

            if (sample == "mic") {
                result.first = prefixes::micro;
                result.second = prefix_and_unit.substr(3);
            } else if (sample.substr(0, 2) == "da") {
                result.first = prefixes::deca;
                result.second = prefix_and_unit.substr(2);
            } else if(prefixes_string.find(sample[0]) != std::string::npos){
                result.first = str_to_preffix(sample[0]);
                result.second = prefix_and_unit.substr(1);
            }else{
                result.first = prefixes::null;
                result.second = prefix_and_unit;
            }
        } else {
            result.first = prefixes::null;
            result.second = prefix_and_unit;
        }

        return result;
    }
    std::array<int8_t, 7> convert_to_basic_units(const std::string &unit) {
        // s, m, g, A, K, mol, cd
        if (unit == "s")
            return {1, 0, 0, 0, 0, 0, 0};
        else if (unit == "m")
            return {0, 1, 0, 0, 0, 0, 0};
        else if (unit == "g")
            return {0, 0, 1, 0, 0, 0, 0};
        else if (unit == "A")
            return {0, 0, 0, 1, 0, 0, 0};
        else if (unit == "K")
            return {0, 0, 0, 0, 1, 0, 0};
        else if (unit == "mol")
            return {0, 0, 0, 0, 0, 1, 0};
        else if (unit == "cd" || unit == "lm")
            return {0, 0, 0, 0, 0, 0, 1};
        else if (unit == "Hz" || unit == "Bq")
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
        else if (unit == "lx")
            return {0, -2, 0, 0, 0, 0, 1};
        else if (unit == "Gy" || unit == "Sv")
            return {-2, 2, 0, 0, 0, 0, 0};
        else if (unit == "kat")
            return {-1, 0, 0, 0, 0, 1, 0};
        else
            throw std::exception();
    }
    std::array<int8_t, 7> dif_b_u_powers(std::array<int8_t, 7> lhs, std::array<int8_t, 7> rhs) {
        std::array<int8_t, 7> result = {0, 0, 0, 0, 0, 0, 0};

        for (size_t i = 0; i < lhs.size(); ++i) {
            result[i] = static_cast<int8_t>(lhs[i] - rhs[i]);
        }

        return result;
    }
    std::array<int8_t, 7> sum_b_u_powers(std::array<int8_t, 7> lhs, std::array<int8_t, 7> rhs) {
        std::array<int8_t, 7> result = {0, 0, 0, 0, 0, 0, 0};

        for (size_t i = 0; i < lhs.size(); ++i) {
            result[i] = static_cast<int8_t>(lhs[i] + rhs[i]);
        }

        return result;
    }
    bool is_equal_powers(std::array<int8_t, 7> lhs, std::array<int8_t, 7> rhs) {
        for (size_t i = 0; i < lhs.size(); i++) {
            if (lhs[i] != rhs[i])
                return false;
        }
        return true;
    }
}// namespace expression