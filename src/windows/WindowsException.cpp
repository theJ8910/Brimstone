/*
WindowsException.cpp
-----------------------
Copyright (c) 2014, theJ89

Description:
    See WindowsException.hpp for more information.
*/




//Includes
#include <brimstone/windows/WindowsException.hpp>   //Header
#include <brimstone/windows/WindowsUtil.hpp>        //utf16to8




namespace Brimstone {
namespace Private {

const ustring WindowsException::strErrorMessage = "Error retrieving Windows exception description.";

WindowsException::WindowsException( DWORD uiErrorCode ) :
    m_uiErrorCode( uiErrorCode ) {
}

DWORD WindowsException::getErrorCode() const {
    return m_uiErrorCode;
}

ustring WindowsException::getDescription() const {
    LPWSTR pszMessage = nullptr;
        
    DWORD uiMessageSize = FormatMessageW(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM     |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        nullptr,
        getErrorCode(),
        MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ),
        (LPWSTR) &pszMessage,
        0, nullptr
    );

    if( uiMessageSize == 0 )
        throwWindowsException();

    //We use a unique_ptr here to let RAII handle cleanup
    //it uses the Windows function LocalFree() to clean up the message,
    //since we requested that FormatMessage() allocate the buffer for us.
    //If pszMessage is null, LocalFree() is never called
    std::unique_ptr< WCHAR, HLOCAL (__stdcall*)( HLOCAL ) >( pszMessage, &LocalFree );

    return utf16to8( pszMessage, uiMessageSize + 1 );
}

/*
Throws the windows exception corresponding to the given code
*/
void throwWindowsException( const DWORD uiErrorCode ) {
    //ERROR_SUCCESS codes shouldn't be ignored
    //if this function is being called its because something went wrong
    /*
    if( code == ERROR_SUCCESS )
        return;
    */

    throw WindowsException( uiErrorCode );
}

/*
Throws a windows exception determined by the error code returned GetLastError()
*/
void throwWindowsException() {
    throwWindowsException( GetLastError() );
}

}
}