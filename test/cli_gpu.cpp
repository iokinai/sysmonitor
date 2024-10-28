#include "gpu_info.hpp"
#include "osd_system_info.hpp"
#include <converters.hpp>
#include <cpu_info.hpp>
#include <cstdint>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

const uint8_t tabs_per_tabul = 8;

std::vector<std::pair<std::string, std::string>> params;

void init_( const sysmonitor::gpu_info &info ) noexcept {
  params.push_back( { "Name", info.name } );
  params.push_back( { "Arch", info.video_arch } );
  params.push_back( { "Description", info.description } );
  params.push_back( { "Driver Version", info.driver_version } );
  params.push_back( { "VRAM Type", info.vram_type } );
}

void print() noexcept {
  size_t maxKeyLength   = 0;
  size_t maxValueLength = 0;

  for ( const auto &[key, value] : params ) {
    maxKeyLength   = std::max( maxKeyLength, key.length() );
    maxValueLength = std::max( maxValueLength, value.length() );
  }

  maxKeyLength      = std::min( maxKeyLength, static_cast<size_t>( 20 ) );
  maxValueLength    = std::min( maxValueLength, static_cast<size_t>( 40 ) );
  size_t totalWidth = maxKeyLength + maxValueLength + 7;

  std::cout << "+" << std::string( totalWidth - 2, '-' ) << "+\n";

  std::cout << "|" << std::string( totalWidth - 2, ' ' ) << "|\n";

  std::string title   = "GPU INFO";
  size_t titlePadding = ( totalWidth - 2 - title.size() ) / 2;
  std::cout << "|" << std::string( titlePadding, ' ' ) << title
            << std::string( totalWidth - 2 - titlePadding - title.size(), ' ' )
            << "|\n";

  std::cout << "|" << std::string( totalWidth - 2, ' ' ) << "|\n";

  std::cout << "+" << std::string( totalWidth - 2, '-' ) << "+\n";

  for ( const auto &[key, value] : params ) {
    std::cout << "| " << std::left << std::setw( maxKeyLength )
              << ( key.length() > maxKeyLength
                       ? key.substr( 0, maxKeyLength - 3 ) + "..."
                       : key )
              << " | " << std::left << std::setw( maxValueLength )
              << ( value.length() > maxValueLength
                       ? value.substr( 0, maxValueLength - 3 ) + "..."
                       : value )
              << " |\n";
  }

  std::cout << "+" << std::string( totalWidth - 2, '-' ) << "+\n";
}

int main() {
  system( "cls" );

  sysmonitor::gpu_info info = sysmonitor::load_gpu_info();

  init_( info );

  print();
}