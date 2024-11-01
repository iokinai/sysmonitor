#include "formatter.hpp"
#include "converters.hpp"
#include "params_sizes.hpp"
#include "params_t.hpp"
#include <cstdlib>
#include <iostream>
#include <ranges>
#include <string>

namespace sysmonitor_cli {

std::vector<params_t> params {};

void init_cpu( const sysmonitor::cpu_info &info ) noexcept {
  params.emplace_back( "CPU INFO" );
  params.emplace_back( params_pair { "Name", info.cpu_name } );
  params.emplace_back( params_pair { "Arch", info.arch } );
  params.emplace_back( params_pair {
      "L2 Cache Size", sysmonitor::bytes_converter( info.l2_size, 2 ) } );
  params.emplace_back( params_pair {
      "L3 Cache Size", sysmonitor::bytes_converter( info.l3_size, 2 ) } );
  params.emplace_back(
      params_pair { "Load Percentage",
                    sysmonitor::percent_converter( info.load_percentage ) } );
  params.emplace_back( params_pair {
      "Max Speed", sysmonitor::hertz_converter( info.max_speed, 2 ) } );
  params.emplace_back( params_pair { "Cores", std::to_string( info.cores ) } );
  params.emplace_back(
      params_pair { "Threads", std::to_string( info.threads ) } );
  params.emplace_back( params_pair { "Socket", info.socket } );
}

void init_gpu( const sysmonitor::gpu_info &info ) noexcept {
  params.emplace_back( "GPU INFO" );
  params.emplace_back( params_pair { "Name", info.name } );
  params.emplace_back( params_pair { "Arch", info.video_arch } );
  params.emplace_back( params_pair { "Description", info.description } );
  params.emplace_back( params_pair { "Driver Version", info.driver_version } );
  params.emplace_back( params_pair { "VRAM Type", info.vram_type } );
  params.emplace_back( params_pair {
      "Refresh Rate", std::to_string( info.refresh_rate ) + "Hz" } );
  params.emplace_back( params_pair {
      "Horizontal Resolution", std::to_string( info.horizontal_resolution ) } );
  params.emplace_back( params_pair {
      "Vertical Resolution", std::to_string( info.vertical_resolution ) } );
}

void init_ram(
    const std::vector<sysmonitor::ram_stick_info> &info_v ) noexcept {
  params.emplace_back( "RAM INFO" );

  for ( const auto &[i, info] : std::views::enumerate( info_v ) ) {
    params.emplace_back( params_pair { "", "STICK " + std::to_string( i ) } );
    params.emplace_back( params_pair { "", "" } );
    params.emplace_back( params_pair { "Name", info.name } );
    params.emplace_back( params_pair { "Model", info.model } );
    params.emplace_back( params_pair { "Description", info.description } );
    params.emplace_back( params_pair { "Version", info.version } );
    params.emplace_back( params_pair {
        "Capacity",
        sysmonitor::bytes_converter( std::stoull( info.capacity ), 2 ) } );
    params.emplace_back(
        params_pair { "Speed", sysmonitor::hertz_converter( info.speed, 2 ) } );
    params.emplace_back( params_pair { "Memory Type", info.memory_type } );
  }
}

void init_os( const sysmonitor::os_info &info ) noexcept {
  params.emplace_back( "OS INFO" );

  params.emplace_back( params_pair { "Name", info.name } );
  params.emplace_back( params_pair { "System Name", info.sys_name } );
  params.emplace_back( params_pair { "Version", info.version } );
  params.emplace_back( params_pair { "Country Code", info.country_code } );
  params.emplace_back( params_pair { "Locale", info.locale } );
  params.emplace_back( params_pair { "System Drive", info.system_drive } );
  params.emplace_back(
      params_pair { "Windows Directory", info.windows_directory } );
  params.emplace_back( params_pair { "Boot Device", info.boot_device } );
}

static void _print_header( const std::string &name,
                           uint32_t total_width ) noexcept {
  std::cout << "+" << std::string( total_width - 2, '-' ) << "+\n";

  std::cout << "|" << std::string( total_width - 2, ' ' ) << "|\n";

  std::string title   = name;
  size_t titlePadding = ( total_width - 2 - title.size() ) / 2;
  std::cout << "|" << std::string( titlePadding, ' ' ) << title
            << std::string( total_width - 2 - titlePadding - title.size(), ' ' )
            << "|\n";

  std::cout << "|" << std::string( total_width - 2, ' ' ) << "|\n";

  std::cout << "+" << std::string( total_width - 2, '-' ) << "+\n";
}

static void _print_data( const params_pair &p, uint32_t max_key_len,
                         uint32_t max_value_len ) noexcept {
  std::cout << "| " << std::left << std::setw( max_key_len )
            << ( p.first.length() > max_key_len
                     ? p.first.substr( 0, max_key_len - 3 ) + "..."
                     : p.first )
            << " | " << std::left << std::setw( max_value_len )
            << ( p.second.length() > max_value_len
                     ? p.second.substr( 0, max_value_len - 3 ) + "..."
                     : p.second )
            << " |\n";
}

static params_sizes _init_params_p() noexcept {
  params_sizes par { 0, 0, 0 };

  for ( params_t &param : params ) {
    params_pair p;
    try {
      p = std::get<params_pair>( param.params );
    } catch ( const std::bad_variant_access & ) {
      continue;
    }

    par.max_key_length   = std::max( par.max_key_length, p.first.length() );
    par.max_value_length = std::max( par.max_value_length, p.second.length() );
  }

  par.max_key_length =
      std::min( par.max_key_length, static_cast<size_t>( 20 ) );
  par.max_value_length =
      std::min( par.max_value_length, static_cast<size_t>( 40 ) );
  par.totalWidth = par.max_key_length + par.max_value_length + 7;

  return par;
}

void print() noexcept {
  if ( params.empty() ) {
    return;
  }

  params_sizes par = _init_params_p();

  for ( const auto &param : params ) {
    try {
      params_pair p = std::get<params_pair>( param.params );
      _print_data( p, par.max_key_length, par.max_value_length );
      continue;
    } catch ( const std::bad_variant_access & ) {
    }

    try {
      std::string s = std::get<std::string>( param.params );
      _print_header( s, par.totalWidth );
      continue;
    } catch ( const std::bad_variant_access & ) {
    }
  }

  std::cout << "+" << std::string( par.totalWidth - 2, '-' ) << "+\n";
}

} // namespace sysmonitor_cli