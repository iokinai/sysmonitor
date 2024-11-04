#ifndef CPU_INFO_HPP
#define CPU_INFO_HPP

#include <cstdint>
#include <string>

namespace sysmonitor {

struct cpu_info {
  std::string arch;
  std::string cpu_name;
  uint32_t l2_size;
  uint32_t l3_size;
  uint32_t max_speed;
  uint16_t load_percentage;
  std::string description;
  uint32_t cores;
  uint32_t threads;
  std::string socket;
};

} // namespace sysmonitor

#endif