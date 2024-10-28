#ifndef GPU_INFO_HPP
#define GPU_INFO_HPP

#include <string>

namespace sysmonitor {

struct gpu_info {
  std::string name;
  std::string description;
  std::string driver_version;
  std::string video_arch;
  std::string vram_type;
};

} // namespace sysmonitor

#endif