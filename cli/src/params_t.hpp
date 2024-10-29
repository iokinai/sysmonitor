#ifndef SYSMONITOR_CLI_PARAMS_T_HPP
#define SYSMONITOR_CLI_PARAMS_T_HPP

#include <string>
#include <variant>

namespace sysmonitor_cli {

using params_pair = std::pair<std::string, std::string>;

struct params_t {
  constexpr inline params_t( const std::variant<params_pair, std::string> &v )
      : params( v ) { }
  std::variant<params_pair, std::string> params;
};

} // namespace sysmonitor_cli

#endif