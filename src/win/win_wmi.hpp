#ifndef WIN_WMI_HPP
#define WIN_WMI_HPP

#include "win_wmi_parser.hpp"
#include <Windows.h>
#include <combaseapi.h>
#include <comdef.h>
#include <osd_system_info.hpp>
#include <stdexcept>
#include <wbemcli.h>
#include <winnt.h>

namespace __windows__details__ {

class wmi {

  HRESULT hr;
  IWbemLocator *pLoc;
  IWbemServices *pSvc;

  void init_com();

  void set_security();

  void create_connection_object();

  void connect_to_wmi();

  void set_proxy_security();

public:
  wmi() {
    init_and_connect();
  }

  wmi( const wmi & ) = delete;
  wmi( wmi && )      = delete;

  inline void init_and_connect() {
    try {

      init_com();
      set_security();
      create_connection_object();
      connect_to_wmi();
      set_proxy_security();

    } catch ( ... ) {
      cleanup();
      throw;
    }
  }

  IEnumWbemClassObject *query( const BSTR strQueryLanguage, const BSTR strQuery,
                               long lFlags, IWbemContext *pCtx );

  inline IWbemClassObject *
  get_single_result( IEnumWbemClassObject *pEnumerator ) {
    IWbemClassObject *pClsObj = nullptr;
    ULONG uReturn             = 0;

    hr = pEnumerator->Next( WBEM_INFINITE, 1, &pClsObj, &uReturn );

    if ( 0 == uReturn )
      throw std::runtime_error( "no data available" );

    return pClsObj;
  }

  template <class T>
  inline decltype( auto ) get_field( IWbemClassObject *pClsObj, BSTR field ) {
    VARIANT vtProp;
    hr       = pClsObj->Get( field, 0, &vtProp, 0, 0 );
    T result = parse_t_result<T>( vtProp );
    VariantClear( &vtProp );

    return result;
  }

  inline void cleanup() noexcept {
    if ( pLoc )
      pLoc->Release();
    if ( pSvc )
      pSvc->Release();

    CoUninitialize();
  }

  inline ~wmi() {
    cleanup();
  }
};

} // namespace __windows__details__

#endif