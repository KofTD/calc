#ifndef CALCULATOR_VARIABLES_H
#define CALCULATOR_VARIABLES_H

#include <map>

#include "calculations.h"
#include "notation_operations.h"
#include "operand.h"

std::map<std::string, expression::operand> load_variables();

#endif  // CALCULATOR_VARIABLES_H
