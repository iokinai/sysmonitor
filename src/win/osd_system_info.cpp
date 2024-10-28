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

static inline std::string
wstring_to_string( const std::wstring &wstr ) noexcept {
  if ( wstr.empty() )
    return std::string();

  int size_needed = WideCharToMultiByte(
      CP_UTF8, 0, &wstr[0], (int) wstr.size(), NULL, 0, NULL, NULL );
  std::string strTo( size_needed, 0 );
  WideCharToMultiByte( CP_UTF8, 0, wstr.data(), (int) wstr.size(), strTo.data(),
                       size_needed, nullptr, nullptr );

  rtrim( strTo );
  return strTo;
}

template <class TR, class TI> using I_to_R_parser = std::function<TR( TI )>;

static inline IWbemClassObject *wmi_send_query( BSTR lang, BSTR query,
                                                long flags ) {
  auto q = dwmi->query( lang, query, flags, nullptr );
  return dwmi->get_single_result( q );
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
static TR wmi_return_result( IWbemClassObject *em, BSTR field,
                             I_to_R_parser<TR, TI> parser ) {
  return parser( wmi_get_result<TI>( em, field ) );
}

template <class TR>
static TR wmi_return_result( IWbemClassObject *em, BSTR field ) {
  return wmi_get_result<TR>( em, field );
}

static IWbemClassObject *wmi_default_query( BSTR table ) {
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

std::string load_cpu_arch() {
  _bstr_t table( "Win32_Processor" );
  _bstr_t field( "Architecture" );

  auto en      = wmi_default_query( table );
  uint32_t rel = wmi_return_result<uint32_t>( en, field );
  en->Release();

  return arc_map.at( rel );
}

std::string load_cpu_name() {
  _bstr_t table( "Win32_Processor" );
  _bstr_t field( "Name" );

  auto en = wmi_default_query( table );
  std::string rel =
      wmi_return_result<std::string, BSTR>( en, field, wstring_to_string );
  en->Release();

  return rel;
}

std::string load_cpu_description() {
  _bstr_t table( "Win32_Processor" );
  _bstr_t field( "Description" );

  auto en = wmi_default_query( table );
  std::string rel =
      wmi_return_result<std::string, BSTR>( en, field, wstring_to_string );
  en->Release();

  return rel;
}

uint32_t load_cpu_l2_size() {
  _bstr_t table( "Win32_Processor" );
  _bstr_t field( "L2CacheSize" );

  auto en      = wmi_default_query( table );
  uint32_t rel = wmi_return_result<uint32_t>( en, field );
  en->Release();

  return rel;
}

uint32_t load_cpu_l3_size() {
  _bstr_t table( "Win32_Processor" );
  _bstr_t field( "L3CacheSize" );

  auto en      = wmi_default_query( table );
  uint32_t rel = wmi_return_result<uint32_t>( en, field );
  en->Release();

  return rel;
}

uint32_t load_cpu_max_speed() {
  _bstr_t table( "Win32_Processor" );
  _bstr_t field( "MaxClockSpeed" );

  auto en      = wmi_default_query( table );
  uint32_t rel = wmi_return_result<uint32_t>( en, field );
  en->Release();

  return rel;
}

uint16_t load_cpu_load_percentage() {
  _bstr_t table( "Win32_Processor" );
  _bstr_t field( "LoadPercentage" );

  auto en      = wmi_default_query( table );
  uint32_t rel = wmi_return_result<uint32_t>( en, field );
  en->Release();

  return rel;
}

uint32_t load_cpu_cores() {
  _bstr_t table( "Win32_Processor" );
  _bstr_t field( "NumberOfCores" );

  auto en      = wmi_default_query( table );
  uint32_t rel = wmi_return_result<uint32_t>( en, field );
  en->Release();

  return rel;
}

uint32_t load_cpu_threads() {
  _bstr_t table( "Win32_Processor" );
  _bstr_t field( "ThreadCount" );

  auto en      = wmi_default_query( table );
  uint32_t rel = wmi_return_result<uint32_t>( en, field );
  en->Release();

  return rel;
}

std::string load_cpu_socket() {
  _bstr_t table( "Win32_Processor" );
  _bstr_t field( "SocketDesignation" );

  auto en = wmi_default_query( table );
  std::string rel =
      wmi_return_result<std::string, BSTR>( en, field, wstring_to_string );
  en->Release();

  return rel;
}

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

  info.arch     = arc_map.at( wmi_return_result<uint32_t>( res, arch_field ) );
  info.cpu_name = wmi_return_result<std::string, BSTR>( res, name_field,
                                                        wstring_to_string );
  info.description     = wmi_return_result<std::string, BSTR>( res, descr_field,
                                                               wstring_to_string );
  info.l2_size         = wmi_return_result<uint32_t>( res, l2size_field );
  info.l3_size         = wmi_return_result<uint32_t>( res, l3size_field );
  info.load_percentage = wmi_return_result<uint32_t>( res, load_perc_field );
  info.max_speed       = wmi_return_result<uint32_t>( res, max_speed_field );
  info.cores           = wmi_return_result<uint32_t>( res, number_of_cores );
  info.threads         = wmi_return_result<uint32_t>( res, thread_count );
  info.socket =
      wmi_return_result<std::string, BSTR>( res, field, wstring_to_string );

  res->Release();

  return info;
}

struct gpu_info_ {
  std::string name;
  uint32_t vram;
  std::string description;
  std::string driver_version;
  std::string system_name;
  uint32_t video_arch;
  uint32_t vram_type;
  std::string gpu;
};

gpu_info load_gpu_info() {
  gpu_info info;

  _bstr_t table( "Win32_VideoController" );
  _bstr_t name_field( "Name" );
  _bstr_t vram_field( "AdapterRAM" );
  _bstr_t description_field( "Description" );
  _bstr_t driver_ver_field( "DriverVersion" );
  _bstr_t system_name_field( "SystemName" );
  _bstr_t video_arch_field( "VideoArchitecture" );
  _bstr_t vram_type_field( "VideoMemoryType" );
  _bstr_t gpu_field( "VideoProcessor" );

  auto res = wmi_default_query( table );

  info.name        = wmi_return_result<std::string, BSTR>( res, name_field,
                                                           wstring_to_string );
  info.description = wmi_return_result<std::string, BSTR>(
      res, description_field, wstring_to_string );

  info.driver_version = wmi_return_result<std::string, BSTR>(
      res, driver_ver_field, wstring_to_string );

  info.video_arch =
      video_arc_map.at( wmi_return_result<uint32_t>( res, video_arch_field ) );
  info.vram_type =
      vram_type_map.at( wmi_return_result<uint32_t>( res, vram_type_field ) );

  return info;
}

/*
    uint32   L2CacheSize;
    uint32   L2CacheSpeed;
    uint32   L3CacheSize;
    uint32   L3CacheSpeed;
    uint32   MaxClockSpeed
*/

} // namespace sysmonitor