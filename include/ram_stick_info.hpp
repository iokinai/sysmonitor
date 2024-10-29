#ifndef RAM_STICK_INFO_HPP
#define RAM_STICK_INFO_HPP

#include <cstdint>
#include <string>

namespace sysmonitor {

struct ram_stick_info {
  std::string name;
  std::string model;
  std::string version;
  std::string capacity;
  uint32_t speed;
  std::string description;
  std::string memory_type;
};

} // namespace sysmonitor

#endif