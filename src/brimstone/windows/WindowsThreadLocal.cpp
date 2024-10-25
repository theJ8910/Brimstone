/*
windows/WindowsThreadLocal.hpp
------------------------------
Copyright (c) 2024, theJ89

Description:
    See WindowsThreadLocal.hpp for more information.
*/




//Includes
#include "WindowsThreadLocal.hpp"  //Header
#include "WindowsHeader.hpp"       //DWORD, TlsAlloc, TlsFree, TlsSetValue, TlsGetValue





namespace Brimstone::Private {




WindowsThreadLocal::WindowsThreadLocal() :
    m_index( static_cast<DWORD>( -1 ) ) {
    m_index = TlsAlloc();
}

WindowsThreadLocal::~WindowsThreadLocal() {
    m_index = TlsFree( m_index );
}


void WindowsThreadLocal::set( void* const value ) {
    TlsSetValue( m_index, value );
}

void* WindowsThreadLocal::get() const {
    return TlsGetValue( m_index);
}




} //namespace Brimstone::Private
