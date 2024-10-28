#include "win_wmi_parser.hpp"
#include <wtypes.h>

namespace __windows__details__ {

template <> BSTR parse_t_result<BSTR>( VARIANT vtProp ) {
  if ( vtProp.vt != VT_BSTR || vtProp.bstrVal == nullptr ) {
    return nullptr;
  }

  return SysAllocString( vtProp.bstrVal );
}

template <> uint32_t parse_t_result<uint32_t>( VARIANT vtProp ) {
  if ( vtProp.vt != VT_I4 ) {
    return 0;
  }

  return vtProp.uintVal;
}

} // namespace __windows__details__