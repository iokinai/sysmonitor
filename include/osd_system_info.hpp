#ifndef OSD_CPU_INFO_HPP
#define OSD_CPU_INFO_HPP

#include "os_info.hpp"
#include <cpu_info.hpp>
#include <cstdint>
#include <gpu_info.hpp>
#include <ram_stick_info.hpp>
#include <string>
#include <vector>

namespace sysmonitor {

std::string load_cpu_arch();
std::string load_cpu_name();
std::string load_cpu_description();
uint32_t load_cpu_l2_size();
uint32_t load_cpu_l3_size();
uint32_t load_cpu_max_speed();
uint16_t load_cpu_load_percentage();
uint32_t load_cpu_cores();
uint32_t load_cpu_threads();
std::string load_cpu_socket();

cpu_info load_cpu_info();
gpu_info load_gpu_info();
std::vector<ram_stick_info> load_ram_info();
os_info load_os_info();

} // namespace sysmonitor

#endif