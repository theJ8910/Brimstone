/*
windows/WindowsException.cpp
----------------------------
Copyright (c) 2024, theJ89

Description:
    See WindowsException.hpp for more information.
*/




//Includes
#include <brimstone/windows/WindowsException.hpp>   //Header
#include <brimstone/windows/WindowsUtil.hpp>        //utf16to8




namespace Brimstone {
namespace Private {

const ustring WindowsException::m_errorMessage = "Error retrieving Windows exception description.";

WindowsException::WindowsException( DWORD errorCode ) :
    m_errorCode( errorCode ) {
}

DWORD WindowsException::getErrorCode() const {
    return m_errorCode;
}

ustring WindowsException::getDescription() const {
    LPWSTR message = nullptr;

    DWORD messageSize = FormatMessageW(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM     |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        nullptr,
        getErrorCode(),
        MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ),
        (LPWSTR) &message,
        0, nullptr
    );

    if( messageSize == 0 )
        throwWindowsException();

    //We use a unique_ptr here to let RAII handle cleanup
    //it uses the Windows function LocalFree() to clean up the message,
    //since we requested that FormatMessage() allocate the buffer for us.
    //If message is nullptr, LocalFree() is never called
    std::unique_ptr< WCHAR, HLOCAL (__stdcall*)( HLOCAL ) > uptr( message, &LocalFree );

    return utf16to8( message, messageSize + 1 );
}

//Do-nothing private assignment operater
WindowsException& WindowsException::operator =( const WindowsException& ) {
    return *this;
}

/*
Throws the windows exception corresponding to the given code
*/
void throwWindowsException( const DWORD errorCode ) {
    //ERROR_SUCCESS codes shouldn't be ignored
    //if this function is being called its because something went wrong
    /*
    if( code == ERROR_SUCCESS )
        return;
    */

    throw WindowsException( errorCode );
}

/*
Throws a windows exception determined by the error code returned GetLastError()
*/
void throwWindowsException() {
    throwWindowsException( GetLastError() );
}

}
}
