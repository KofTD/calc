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


    std::pair<prefixes, std::string> split_prefix_and_unit(std::string prefix_and_unit) {
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
}// namespace expression