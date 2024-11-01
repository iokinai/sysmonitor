#ifndef SYSMONITOR_CLI_FORMATTER_HPP
#define SYSMONITOR_CLI_FORMATTER_HPP

#include "gpu_info.hpp"
#include "os_info.hpp"
#include "ram_stick_info.hpp"
#include <converters.hpp>
#include <cpu_info.hpp>

namespace sysmonitor_cli {

void init_cpu( const sysmonitor::cpu_info &info ) noexcept;
void init_gpu( const sysmonitor::gpu_info &info ) noexcept;
void init_ram( const std::vector<sysmonitor::ram_stick_info> &info ) noexcept;
void init_os( const sysmonitor::os_info &info ) noexcept;
void print() noexcept;

} // namespace sysmonitor_cli

#endif