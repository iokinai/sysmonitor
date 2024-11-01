#include "cpu_info.hpp"
#include "win_wmi.hpp"
#include <Windows.h>
#include <combaseapi.h>
#include <comdef.h>
#include <cstdint>
#include <functional>
#include <memory>
#include <osd_system_info.hpp>
#include <unordered_map>
#include <wbemcli.h>
#include <winnt.h>
#include <wtypes.h>

#define dwmi     __wmi__singleton__detail__::wmi
#define wmi_init __wmi__default__init__data__::__wmi__default__init__

namespace sysmonitor {

namespace __wmi__singleton__detail__ {
static auto wmi = std::make_shared<__windows__details__::wmi>();
}

namespace __wmi__default__init__data__ {

struct __wmi__default__init__ {
  _bstr_t lang, fquery;
  long flags;
};

} // namespace __wmi__default__init__data__

static inline void rtrim( std::string &s ) {
  s.erase(
      std::find_if( s.rbegin(), s.rend(),
                    []( unsigned char ch ) { return !std::isspace( ch ); } )
          .base(),
      s.end() );
}

static inline std::string wstring_to_string( BSTR bstr ) noexcept {
  if ( bstr == nullptr ) {
    return "NULL";
  }

  std::wstring wstr( bstr );

  int size_needed = WideCharToMultiByte(
      CP_UTF8, 0, &wstr[0], (int) wstr.size(), NULL, 0, NULL, NULL );
  std::string str_to( size_needed, 0 );
  WideCharToMultiByte( CP_UTF8, 0, wstr.data(), (int) wstr.size(),
                       str_to.data(), size_needed, nullptr, nullptr );

  rtrim( str_to );
  return str_to;
}

template <class TR, class TI> using I_to_R_parser = std::function<TR( TI )>;

static inline IEnumWbemClassObject *wmi_send_query( BSTR lang, BSTR query,
                                                    long flags ) {
  return dwmi->query( lang, query, flags, nullptr );
}

template <class TI>
static inline TI wmi_get_result( IWbemClassObject *obj, BSTR field ) {
  TI result = dwmi->get_field<TI>( obj, field );
  return result;
}

static inline wmi_init default_init_data( BSTR table ) noexcept {
  _bstr_t btable( table );
  _bstr_t query( "SELECT * FROM " );
  _bstr_t fquery = query + btable;

  _bstr_t lang( "WQL" );
  long flags = WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY;

  return { lang, fquery, flags };
}

template <class TR, class TI>
static TR wmi_return_current_result( IWbemClassObject *cl, BSTR field,
                                     I_to_R_parser<TR, TI> parser ) {
  return parser( wmi_get_result<TI>( cl, field ) );
}

template <class TR>
static TR wmi_return_current_result( IWbemClassObject *cl, BSTR field ) {
  return wmi_get_result<TR>( cl, field );
}

template <class TR, class TI>
static TR wmi_return_next_result( IEnumWbemClassObject *cl, BSTR field,
                                  I_to_R_parser<TR, TI> parser ) {
  auto em = dwmi->get_single_result( cl );
  return wmi_return_current_result<TR, TI>( em, field, parser );
}

template <class TR>
static TR wmi_return_next_result( IEnumWbemClassObject *cl, BSTR field ) {
  auto em = dwmi->get_single_result( cl );
  return wmi_return_current_result<TR>( em, field );
}

static IEnumWbemClassObject *wmi_default_query( BSTR table ) {
  auto data = default_init_data( table );
  return wmi_send_query( data.lang, data.fquery, data.flags );
}

static inline const std::unordered_map<uint32_t, std::string> arc_map = {
    { 0, "x86" },     { 1, "MIPS" },   { 2, "Alpha" },
    { 3, "PowerPC" }, { 5, "ARM" },    { 6, "ia64" },
    { 9, "x64" },     { 12, "ARM64" }, { 0xffff, "UNKNOWN" } };

static inline const std::unordered_map<uint32_t, std::string> vram_type_map = {
    { 2, "DRAM" }, { 3, "VRAM" }, { 4, "SDRAM" }, { 5, "SGRAM" } };

static inline const std::unordered_map<uint32_t, std::string> video_arc_map {
    { 1, "Other" }, { 2, "Unknown" }, { 5, "VGA" },
    { 6, "XGA" },   { 7, "SGVA" },    { 8, "MDA" },
};

static const std::unordered_map<uint32_t, std::string> memory_type_map = {
    { 0, "Unknown" },       { 1, "Other" },
    { 2, "DRAM" },          { 3, "Synchronous DRAM" },
    { 4, "Cache DRAM" },    { 5, "EDO" },
    { 6, "EDRAM" },         { 7, "VRAM" },
    { 8, "SRAM" },          { 9, "RAM" },
    { 10, "ROM" },          { 11, "Flash" },
    { 12, "EEPROM" },       { 13, "FEPROM" },
    { 14, "EPROM" },        { 15, "CDRAM" },
    { 16, "3DRAM" },        { 17, "SDRAM" },
    { 18, "SGRAM" },        { 19, "RDRAM" },
    { 20, "DDR" },          { 21, "DDR2" },
    { 22, "DDR2 FB-DIMM" }, { 23, "DDR3" },
    { 24, "FBD2" },         { 25, "DDR4" } };

cpu_info load_cpu_info() {
  cpu_info info;

  _bstr_t table( "Win32_Processor" );
  _bstr_t arch_field( "Architecture" );
  _bstr_t name_field( "Name" );
  _bstr_t descr_field( "Description" );
  _bstr_t l2size_field( "L2CacheSize" );
  _bstr_t l3size_field( "L3CacheSize" );
  _bstr_t load_perc_field( "LoadPercentage" );
  _bstr_t max_speed_field( "MaxClockSpeed" );
  _bstr_t number_of_cores( "NumberOfCores" );
  _bstr_t thread_count( "ThreadCount" );
  _bstr_t field( "SocketDesignation" );

  auto res = wmi_default_query( table );
  auto en  = dwmi->get_single_result( res );

  info.arch =
      arc_map.at( wmi_return_current_result<uint32_t>( en, arch_field ) );
  info.cpu_name = wmi_return_current_result<std::string, BSTR>(
      en, name_field, wstring_to_string );
  info.description = wmi_return_current_result<std::string, BSTR>(
      en, descr_field, wstring_to_string );
  info.l2_size = wmi_return_current_result<uint32_t>( en, l2size_field );
  info.l3_size = wmi_return_current_result<uint32_t>( en, l3size_field );
  info.load_percentage =
      wmi_return_current_result<uint32_t>( en, load_perc_field );
  info.max_speed = wmi_return_current_result<uint32_t>( en, max_speed_field );
  info.cores     = wmi_return_current_result<uint32_t>( en, number_of_cores );
  info.threads   = wmi_return_current_result<uint32_t>( en, thread_count );
  info.socket    = wmi_return_current_result<std::string, BSTR>(
      en, field, wstring_to_string );

  res->Release();

  return info;
}

gpu_info load_gpu_info() {
  gpu_info info;

  _bstr_t table( "Win32_VideoController" );
  _bstr_t name_field( "Name" );
  _bstr_t description_field( "Description" );
  _bstr_t driver_ver_field( "DriverVersion" );
  _bstr_t video_arch_field( "VideoArchitecture" );
  _bstr_t vram_type_field( "VideoMemoryType" );
  _bstr_t refresh_rate_field( "CurrentRefreshRate" );
  _bstr_t hor_res_field( "CurrentHorizontalResolution" );
  _bstr_t ver_res_field( "CurrentVerticalResolution" );

  auto res = wmi_default_query( table );
  auto en  = dwmi->get_single_result( res );

  info.name = wmi_return_current_result<std::string, BSTR>( en, name_field,
                                                            wstring_to_string );
  info.description = wmi_return_current_result<std::string, BSTR>(
      en, description_field, wstring_to_string );

  info.driver_version = wmi_return_current_result<std::string, BSTR>(
      en, driver_ver_field, wstring_to_string );

  info.video_arch = video_arc_map.at(
      wmi_return_current_result<uint32_t>( en, video_arch_field ) );
  info.vram_type = vram_type_map.at(
      wmi_return_current_result<uint32_t>( en, vram_type_field ) );
  info.refresh_rate =
      wmi_return_current_result<uint32_t>( en, refresh_rate_field );
  info.horizontal_resolution =
      wmi_return_current_result<uint32_t>( en, hor_res_field );
  info.vertical_resolution =
      wmi_return_current_result<uint32_t>( en, ver_res_field );

  return info;
}

std::vector<ram_stick_info> load_ram_info() {
  std::vector<ram_stick_info> info;

  _bstr_t table( "Win32_PhysicalMemory" );
  _bstr_t name_field( "Name" );
  _bstr_t model_field( "Model" );
  _bstr_t version_field( "Version" );
  _bstr_t capacity_field( "Capacity" );
  _bstr_t speed_field( "Speed" );
  _bstr_t description_field( "Description" );
  _bstr_t memory_type_field( "MemoryType" );

  auto default_data = default_init_data( table );
  auto res          = wmi_send_query( default_data.lang, default_data.fquery,
                                      default_data.flags );
  auto sresult      = dwmi->get_single_result( res );

  while ( sresult ) {

    std::string name = wmi_return_current_result<std::string, BSTR>(
        sresult, name_field, wstring_to_string );
    std::string description = wmi_return_current_result<std::string, BSTR>(
        sresult, description_field, wstring_to_string );

    std::string model = wmi_return_current_result<std::string, BSTR>(
        sresult, model_field, wstring_to_string );

    std::string version = std::to_string(
        wmi_return_current_result<uint32_t>( sresult, version_field ) );
    std::string capacity = wmi_return_current_result<std::string, BSTR>(
        sresult, capacity_field, wstring_to_string );
    uint32_t speed =
        wmi_return_current_result<uint32_t>( sresult, speed_field );
    uint32_t memory_type =
        wmi_return_current_result<uint32_t>( sresult, memory_type_field );

    info.emplace_back( name, model, version, capacity, speed, description,
                       memory_type_map.at( memory_type ) );

    sresult = dwmi->get_single_result( res );
  }

  return info;
}

os_info load_os_info() {
  os_info info;

  _bstr_t table( "Win32_OperatingSystem" );
  _bstr_t name_field( "Name" );
  _bstr_t sys_name_field( "CSName" );
  _bstr_t version_field( "Version" );
  _bstr_t country_code_field( "CountryCode" );
  _bstr_t locale_field( "Locale" );
  _bstr_t system_drive_field( "SystemDrive" );
  _bstr_t windows_directory_field( "WindowsDirectory" );
  _bstr_t boot_device_field( "BootDevice" );

  auto res = wmi_default_query( table );
  auto en  = dwmi->get_single_result( res );

  info.name     = wmi_return_current_result<std::string, BSTR>( en, name_field,
                                                                wstring_to_string );
  info.sys_name = wmi_return_current_result<std::string, BSTR>(
      en, sys_name_field, wstring_to_string );
  info.version = wmi_return_current_result<std::string, BSTR>(
      en, version_field, wstring_to_string );

  info.country_code = wmi_return_current_result<std::string, BSTR>(
      en, country_code_field, wstring_to_string );

  info.locale = wmi_return_current_result<std::string, BSTR>(
      en, locale_field, wstring_to_string );
  info.system_drive = wmi_return_current_result<std::string, BSTR>(
      en, system_drive_field, wstring_to_string );

  info.windows_directory = wmi_return_current_result<std::string, BSTR>(
      en, windows_directory_field, wstring_to_string );
  info.boot_device = wmi_return_current_result<std::string, BSTR>(
      en, boot_device_field, wstring_to_string );

  return info;
}

std::string load_cpu_arch() {
  return load_cpu_info().arch;
}

std::string load_cpu_name() {
  return load_cpu_info().cpu_name;
}

std::string load_cpu_description() {
  return load_cpu_info().description;
}

uint32_t load_cpu_l2_size() {
  return load_cpu_info().l2_size;
}

uint32_t load_cpu_l3_size() {
  return load_cpu_info().l3_size;
}

uint32_t load_cpu_max_speed() {
  return load_cpu_info().max_speed;
}

uint16_t load_cpu_load_percentage() {
  return load_cpu_info().load_percentage;
}

uint32_t load_cpu_cores() {
  return load_cpu_info().cores;
}

uint32_t load_cpu_threads() {
  return load_cpu_info().threads;
}

std::string load_cpu_socket() {
  return load_cpu_info().socket;
}

/*
    uint32   L2CacheSize;
    uint32   L2CacheSpeed;
    uint32   L3CacheSize;
    uint32   L3CacheSpeed;
    uint32   MaxClockSpeed
*/

} // namespace sysmonitor