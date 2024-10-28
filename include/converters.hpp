#ifndef CONVERTERS_HPP
#define CONVERTERS_HPP

#include <array>
#include <cmath>
#include <cstdint>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

namespace sysmonitor {

inline std::pair<float, std::string> base_converter(
    uint32_t base_value, uint8_t accuracy,
    const std::vector<std::pair<uint32_t, std::string>> &units ) noexcept {
  float factor = std::pow( 10, accuracy );

  for ( const auto &[divisor, unit] : units ) {
    if ( base_value >= divisor ) {
      float result =
          std::round( ( static_cast<float>( base_value ) / divisor ) *
                      factor ) /
          factor;
      return { result, unit };
    }
  }

  return { static_cast<float>( base_value ), "" };
}

inline std::string
float_converter( uint32_t base_value, uint8_t accuracy,
                 const std::vector<std::pair<uint32_t, std::string>> &units ) {
  auto [result, unit] = base_converter( base_value, accuracy, units );

  std::ostringstream oss;
  oss << std::fixed << std::setprecision( accuracy ) << result;
  std::string str_result = oss.str();

  if ( !unit.empty() ) {
    str_result += unit;
  }

  return str_result;
}

inline std::string bytes_converter( uint32_t base_value,
                                    uint8_t accuracy ) noexcept {

  std::vector<std::pair<uint32_t, std::string>> units = {
      { 1000000, "G" },
      { 1000, "M" },
      { 100, "K" },
  };

  return float_converter( base_value, accuracy, units );
}

inline std::string hertz_converter( uint32_t base_value,
                                    uint8_t accuracy ) noexcept {

  std::vector<std::pair<uint32_t, std::string>> units = {
      { 1000, "GHz" },
      { 100, "MHz" },
  };

  return float_converter( base_value, accuracy, units );
}

inline std::string percent_converter( uint8_t base_value ) noexcept {
  return std::to_string( base_value ) + "%";
}

} // namespace sysmonitor

#endif