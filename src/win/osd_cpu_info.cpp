#include "win_wmi.hpp"
#include <Windows.h>
#include <combaseapi.h>
#include <comdef.h>
#include <cstdint>
#include <functional>
#include <memory>
#include <osd_cpu_info.hpp>
#include <stdexcept>
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
  _bstr_t lang, fquery, bsfield;
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

template <class TI>
static inline TI wmi_send_query( BSTR lang, BSTR query, BSTR field,
                                 long flags ) {
  IEnumWbemClassObject *enumerator = dwmi->query( lang, query, flags, nullptr );

  return dwmi->get_single_result<TI>( enumerator, field );
}

static inline wmi_init default_init_data( BSTR table, BSTR field ) noexcept {
  _bstr_t btable( table );
  _bstr_t query( "SELECT * FROM " );
  _bstr_t bsfield( field );
  _bstr_t fquery = query + btable;

  _bstr_t lang( "WQL" );
  long flags = WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY;

  return { lang, fquery, bsfield, flags };
}

template <class TR, class TI>
static TR wmi_query( BSTR lang, BSTR query, BSTR field, long flags,
                     I_to_R_parser<TR, TI> parser ) {
  return parser( wmi_send_query<TI>( lang, query, field, flags ) );
}

template <class TR>
static TR wmi_query( BSTR lang, BSTR query, BSTR field, long flags ) {
  return wmi_send_query<TR>( lang, query, field, flags );
}

template <class TR, class TI>
static TR wmi_default_query( BSTR table, BSTR field,
                             I_to_R_parser<TR, TI> parser ) {
  auto data = default_init_data( table, field );
  return wmi_query<TR, TI>( data.lang, data.fquery, data.bsfield, data.flags,
                            parser );
}

template <class TR> static TR wmi_default_query( BSTR table, BSTR field ) {
  auto data = default_init_data( table, field );
  return wmi_query<TR>( data.lang, data.fquery, data.bsfield, data.flags );
}

static inline const std::unordered_map<int, std::string> arc_map = {
    { 0, "x86" },
    { 6, "x64" },
    { 9, "ARM" },
    { 12, "ARM64" },
    { 0xffff, "UNKNOWN" } };

std::string load_cpu_arch() {
  _bstr_t table( "Win32_Processor" );
  _bstr_t field( "Architecture" );

  return arc_map.at( wmi_default_query<uint32_t>( table, field ) );
}

std::string load_cpu_name() {
  _bstr_t table( "Win32_Processor" );
  _bstr_t field( "Name" );

  return wmi_default_query<std::string, BSTR>( table, field,
                                               wstring_to_string );
}

std::string load_cpu_description() {
  _bstr_t table( "Win32_Processor" );
  _bstr_t field( "Description" );

  return wmi_default_query<std::string, BSTR>( table, field,
                                               wstring_to_string );
}

uint32_t load_cpu_l2_size() {
  _bstr_t table( "Win32_Processor" );
  _bstr_t field( "L2CacheSize" );

  return wmi_default_query<uint32_t>( table, field );
}

uint32_t load_cpu_l3_size() {
  _bstr_t table( "Win32_Processor" );
  _bstr_t field( "L3CacheSize" );

  return wmi_default_query<uint32_t>( table, field );
}

uint32_t load_cpu_max_speed() {
  _bstr_t table( "Win32_Processor" );
  _bstr_t field( "MaxClockSpeed" );

  return wmi_default_query<uint32_t>( table, field );
}

uint16_t load_cpu_load_percentage() {
  _bstr_t table( "Win32_Processor" );
  _bstr_t field( "LoadPercentage" );

  return wmi_default_query<uint32_t>( table, field );
}

cpu_info load_cpu_info() {
  //
}

/*
    uint32   L2CacheSize;
    uint32   L2CacheSpeed;
    uint32   L3CacheSize;
    uint32   L3CacheSpeed;
    uint32   MaxClockSpeed
*/

} // namespace sysmonitor