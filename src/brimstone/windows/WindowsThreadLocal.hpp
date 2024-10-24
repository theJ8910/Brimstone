/*
windows/WindowsThreadLocal.hpp
------------------------------
Copyright (c) 2024, theJ89

Description:
    WindowsThreadLocal is defined here.
*/
#ifndef BS_WINDOWS_WINDOWSTHREADLOCAL_HPP
#define BS_WINDOWS_WINDOWSTHREADLOCAL_HPP




//Includes
#include "WindowsHeader.hpp"  //DWORD, TlsAlloc, TlsFree, TlsSetValue, TlsGetValue




namespace Brimstone::Private {




class WindowsThreadLocal {
public:
    WindowsThreadLocal();
    ~WindowsThreadLocal();
    void  set( void* const value );
    void* get() const;
private:
    DWORD m_index;
};




} //namespace Brimstone::Private




#endif //BS_WINDOWS_WINDOWSTHREADLOCAL_HPP
