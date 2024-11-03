#include "formatter.hpp"
#include "osd_system_info.hpp"
#include <boost/program_options/errors.hpp>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/value_semantic.hpp>
#include <boost/program_options/variables_map.hpp>
#include <iostream>

namespace po = boost::program_options;

int main( int argc, char **argv ) {
  po::options_description desc( "Allowed options" );
  desc.add_options()( "help,h", "Display help message" )(
      "cpu", po::bool_switch()->notifier( []( bool value ) {
        if ( value )
          sysmonitor_cli::init_cpu( sysmonitor::load_cpu_info() );
      } ),
      "Display CPU info" )(
      "gpu", po::bool_switch()->notifier( []( bool value ) {
        if ( value )
          sysmonitor_cli::init_gpu( sysmonitor::load_gpu_info() );
      } ),
      "Display GPU info" )(
      "ram", po::bool_switch()->notifier( []( bool value ) {
        if ( value )
          sysmonitor_cli::init_ram( sysmonitor::load_ram_info() );
      } ),
      "Display RAM info" )(
      "os", po::bool_switch()->notifier( []( bool value ) {
        if ( value )
          sysmonitor_cli::init_os( sysmonitor::load_os_info() );
      } ),
      "Display OS info" );

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

  sysmonitor_cli::print();
}