#ifndef GPU_INFO_HPP
#define GPU_INFO_HPP

#include <cstdint>
#include <string>


namespace sysmonitor {

struct gpu_info {
  std::string name;
  std::string description;
  std::string driver_version;
  std::string video_arch;
  std::string vram_type;
  uint32_t refresh_rate;
  uint32_t horizontal_resolution;
  uint32_t vertical_resolution;
};

} // namespace sysmonitor

#endif