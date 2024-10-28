#include "osd_cpu_info.hpp"
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

void init_( const sysmonitor::cpu_info &info ) noexcept {
  params.push_back( { "Name", info.cpu_name } );
  params.push_back( { "Arch", info.arch } );
  params.push_back(
      { "L2 Cache Size", sysmonitor::bytes_converter( info.l2_size, 2 ) } );
  params.push_back(
      { "L3 Cache Size", sysmonitor::bytes_converter( info.l3_size, 2 ) } );
  params.push_back( { "Load Percentage",
                      sysmonitor::percent_converter( info.load_percentage ) } );
  params.push_back(
      { "Max Speed", sysmonitor::hertz_converter( info.max_speed, 2 ) } );
  params.push_back( { "Cores", std::to_string( info.cores ) } );
  params.push_back( { "Threads", std::to_string( info.threads ) } );
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

  std::string title   = "CPU INFO";
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

  sysmonitor::cpu_info info = sysmonitor::load_cpu_info();

  init_( info );

  print();
}