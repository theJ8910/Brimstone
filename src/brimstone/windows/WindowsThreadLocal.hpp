/*
windows/WindowsThreadLocal.hpp
-----------------------
Copyright (c) 2014, theJ89

Description:
    WindowsThreadLocal is defined here.
*/
#ifndef BS_WINDOWS_WINDOWSTHREADLOCAL_HPP
#define BS_WINDOWS_WINDOWSTHREADLOCAL_HPP




//Includes
#include <brimstone/windows/WindowsHeader.hpp>  //DWORD, TlsAlloc, TlsFree, TlsSetValue, TlsGetValue




namespace Brimstone {
namespace Private {

class WindowsThreadLocal {
public:
    WindowsThreadLocal();
    ~WindowsThreadLocal();
    void  set( void* const value );
    void* get() const;
private:
    DWORD m_index;
};

}
}

#endif //BS_WINDOWS_WINDOWSTHREADLOCAL_HPP