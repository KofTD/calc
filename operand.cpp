#include "operand.h"

namespace expression {
int8_t str_to_prefix(const std::string &prefix) {
  if (!prefixes.contains(prefix)) {
    throw std::exception();
  }

  return prefixes.at(prefix);
}

std::pair<int8_t, std::string>
split_prefix_and_unit(const std::string &prefix_and_unit) {
  std::pair<int8_t, std::string> result;
  const std::string prefixes_string = "QRYZEPTGMkhdcmnpfazyrq";

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
    const std::string
        &unit) { // NOLINT(readability-function-cognitive-complexity)
  // s, m, g, A, K, mol, cd
  if (!physical_units.contains(unit)) {
    throw std::exception();
  }

  return physical_units.at(unit);
}
std::array<int8_t, n_of_base_units>
dif_b_u_powers(std::array<int8_t, n_of_base_units> lhs,
               std::array<int8_t, n_of_base_units> rhs) {
  std::array<int8_t, n_of_base_units> result = {0, 0, 0, 0, 0, 0, 0};

  for (size_t i = 0; i < lhs.size(); ++i) {
    result.at(i) = static_cast<int8_t>(lhs.at(i) - rhs.at(i));
  }

  return result;
}
std::array<int8_t, n_of_base_units>
sum_b_u_powers(std::array<int8_t, n_of_base_units> lhs,
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
  if (power == 0) {
    return 1;
  }
  double result = 1;
  if (power > 0) {
    for (int8_t _ = 0; _ < power; _++) {
      result *= 10; // NOLINT
    }
  } else {
    for (int8_t _ = 0; _ < power; _++) {
      result /= 10; // NOLINT
    }
  }
  return result;
}
} // namespace expression
