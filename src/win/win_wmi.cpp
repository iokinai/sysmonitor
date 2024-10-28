#include "win_wmi.hpp"
#include <stdexcept>

namespace __windows__details__ {
void wmi::init_com() {
  hr = CoInitializeEx( 0, COINIT_MULTITHREADED );
  if ( FAILED( hr ) ) {
    throw std::runtime_error( "failed to initialize WMI" );
  }
}

void wmi::set_security() {
  hr = CoInitializeSecurity(
      nullptr, -1, nullptr, nullptr, RPC_C_AUTHN_LEVEL_DEFAULT,
      RPC_C_IMP_LEVEL_IMPERSONATE, nullptr, EOAC_NONE, nullptr );

  if ( FAILED( hr ) ) {
    throw std::runtime_error( "error on setting security settings" );
  }
}

void wmi::create_connection_object() {
  hr = CoCreateInstance( CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER,
                         IID_IWbemLocator, (LPVOID *) &pLoc );

  if ( FAILED( hr ) ) {
    throw std::runtime_error( "error on creating connection object" );
  }
}

void wmi::connect_to_wmi() {
  hr = pLoc->ConnectServer( _bstr_t( L"ROOT\\CIMV2" ), nullptr, nullptr, 0, 0,
                            0, 0, &pSvc );

  if ( FAILED( hr ) ) {
    throw std::runtime_error( "failed to connect to wmi" );
  }
}

void wmi::set_proxy_security() {
  hr = CoSetProxyBlanket( pSvc, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, nullptr,
                          RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE,
                          nullptr, EOAC_NONE );

  if ( FAILED( hr ) ) {
    throw std::runtime_error( "error on setting proxy security" );
  }
}

IEnumWbemClassObject *wmi::query( const BSTR strQueryLanguage,
                                  const BSTR strQuery, long lFlags,
                                  IWbemContext *pCtx ) {
  IEnumWbemClassObject *pEnumerator = nullptr;
  hr =
      pSvc->ExecQuery( strQueryLanguage, strQuery, lFlags, pCtx, &pEnumerator );

  if ( FAILED( hr ) ) {
    throw std::runtime_error( "error on sending query" );
    cleanup();
  }

  return pEnumerator;
}
} // namespace __windows__details__