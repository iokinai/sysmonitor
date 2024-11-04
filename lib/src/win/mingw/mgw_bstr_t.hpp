#ifndef MINGW_MGW_BSTR_T
#define MINGW_MGW_BSTR_T

#include <string>
#include <wtypes.h>

namespace sysmonitor {

class mgw_bstr_t {
public:
  mgw_bstr_t( const char *char_string );

  mgw_bstr_t &operator= ( const mgw_bstr_t &other );

  mgw_bstr_t &operator= ( mgw_bstr_t &&other ) noexcept;

  mgw_bstr_t operator+ ( const mgw_bstr_t &other ) const noexcept;

  inline mgw_bstr_t( BSTR str ) : bstr_( str ) { }

  inline mgw_bstr_t( const std::string &str ) : mgw_bstr_t( str.c_str() ) { }

  inline mgw_bstr_t( const mgw_bstr_t &other ) : bstr_( nullptr ) {
    if ( other.bstr_ ) {
      bstr_ = SysAllocStringLen( other.bstr_, SysStringLen( other.bstr_ ) );
    }
  }

  inline mgw_bstr_t( mgw_bstr_t &&other ) noexcept : bstr_( other.bstr_ ) {
    other.bstr_ = nullptr;
  }

  inline ~mgw_bstr_t() {
    SysFreeString( bstr_ );
  }

  constexpr inline BSTR get() const {
    return bstr_;
  }

  constexpr inline operator BSTR () const {
    return bstr_;
  }

  inline size_t size() const noexcept {
    return SysStringLen( bstr_ );
  }

  inline bool empty() const {
    return bstr_ == nullptr || SysStringLen( bstr_ ) == 0;
  }

private:
  BSTR bstr_;
};

} // namespace sysmonitor

#endif
