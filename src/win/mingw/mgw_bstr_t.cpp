#include "mgw_bstr_t.hpp"

namespace sysmonitor {

mgw_bstr_t::mgw_bstr_t( const char *char_string ) : bstr_( nullptr ) {
  if ( char_string ) {
    int wchar_size =
        MultiByteToWideChar( CP_UTF8, 0, char_string, -1, NULL, 0 );
    if ( wchar_size > 0 ) {
      bstr_ = SysAllocStringLen( NULL, wchar_size - 1 );
      if ( bstr_ ) {
        MultiByteToWideChar( CP_UTF8, 0, char_string, -1, bstr_, wchar_size );
      }
    }
  }
}

mgw_bstr_t &mgw_bstr_t::operator= ( const mgw_bstr_t &other ) {
  if ( this != &other ) {
    SysFreeString( bstr_ );
    bstr_ = nullptr;
    if ( other.bstr_ ) {
      bstr_ = SysAllocStringLen( other.bstr_, SysStringLen( other.bstr_ ) );
    }
  }
  return *this;
}

mgw_bstr_t &mgw_bstr_t::operator= ( mgw_bstr_t &&other ) noexcept {
  if ( this != &other ) {
    SysFreeString( bstr_ );
    bstr_       = other.bstr_;
    other.bstr_ = nullptr;
  }
  return *this;
}

mgw_bstr_t mgw_bstr_t::operator+ ( const mgw_bstr_t &other ) const noexcept {
  size_t wchar_size = size() + other.size();

  BSTR new_bstr = SysAllocStringLen( NULL, wchar_size );

  if ( new_bstr ) {
    memcpy( new_bstr, bstr_, size() * sizeof( wchar_t ) );
    memcpy( new_bstr + size(), other.bstr_, other.size() * sizeof( wchar_t ) );
  }

  return mgw_bstr_t( new_bstr );
}

} // namespace sysmonitor