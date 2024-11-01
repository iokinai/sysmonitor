#include "formatter.hpp"
#include "osd_system_info.hpp"
#include <boost/program_options/errors.hpp>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>
#include <iostream>

namespace po = boost::program_options;

int main( int argc, char **argv ) {
  po::options_description desc( "Allowed options" );
  desc.add_options()( "help,h", "Display help message" )(
      "cpu", "Display CPU info" )( "gpu", "Display GPU info" )(
      "ram", "Display RAM info" )( "os", "Display OS info" );

  if ( argc == 1 ) {
    std::cout << desc << std::endl;
    return -1;
  }

  po::variables_map vm;

  try {
    po::store( po::parse_command_line( argc, argv, desc ), vm );
  } catch ( const po::error &e ) {
    std::cout << e.what() << std::endl;
    return -1;
  }
  po::notify( vm );

  if ( vm.count( "help" ) ) {
    std::cout << desc << std::endl;
  }

  if ( vm.count( "cpu" ) ) {
    sysmonitor_cli::init_cpu( sysmonitor::load_cpu_info() );
  }

  if ( vm.count( "gpu" ) ) {
    sysmonitor_cli::init_gpu( sysmonitor::load_gpu_info() );
  }

  if ( vm.count( "ram" ) ) {
    sysmonitor_cli::init_ram( sysmonitor::load_ram_info() );
  }

  if ( vm.count( "os" ) ) {
    sysmonitor_cli::init_os( sysmonitor::load_os_info() );
  }

  sysmonitor_cli::print();
}