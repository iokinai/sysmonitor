#ifndef OS_INFO_HPP
#define OS_INFO_HPP

#include <string>

namespace sysmonitor {

struct os_info {
  std::string name;
  std::string sys_name;
  std::string version;
  std::string country_code;
  std::string locale;
  std::string system_drive;
  std::string windows_directory;
  std::string boot_device;
};

} // namespace sysmonitor

#endif