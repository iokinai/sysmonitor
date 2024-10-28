#ifndef WIN_WMI_PARSER_HPP
#define WIN_WMI_PARSER_HPP

#include <cstdint>
#include <oaidl.h>
#include <wtypes.h>

namespace __windows__details__ {

template <class T> T parse_t_result( VARIANT vtProp ) = delete;

template <> BSTR parse_t_result<BSTR>( VARIANT vtProp );
template <> uint32_t parse_t_result<uint32_t>( VARIANT vtProp );

} // namespace __windows__details__

#endif