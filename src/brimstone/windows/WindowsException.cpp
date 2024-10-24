/*
windows/WindowsException.cpp
----------------------------
Copyright (c) 2024, theJ89

Description:
    See WindowsException.hpp for more information.
*/




//Includes
#include <brimstone/windows/WindowsException.hpp>  //Header

#include "WindowsHeader.hpp"                       //LPWSTR, DWORD, FormatMessageW
#include "WindowsUtil.hpp"                         //Brimstone::Private::utf16to8

#include <memory>                                  //std::unique_ptr




namespace {




//NOTE:
//    Not actually used at the moment.
//const ustring ERROR_MESSAGE = "Error retrieving Windows exception description.";




} //namespace




namespace Brimstone::Private {




/*
WindowsException::WindowsException
----------------------------------

Description:
    Constructor. Initializes the WindowsException with the given error code.

Arguments:
    errorCode:  The error code.

Returns:
    N/A
*/
WindowsException::WindowsException( const DWORD errorCode ) :
    m_errorCode( errorCode ) {
}

/*
WindowsException::getErrorCode
------------------------------

Description:
    Returns the error code for this exception.

Arguments:
    N/A

Returns:
    DWORD:  The error code.
*/
DWORD WindowsException::getErrorCode() const {
    return m_errorCode;
}

/*
WindowsException::getDescription
--------------------------------

Description:
    Returns a string describing the error code for this exception.

Arguments:
    N/A

Returns:
    ustring:  A description of the error code.
*/
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

    //We use a unique_ptr here to let RAII handle cleanup.
    //It uses the Windows function LocalFree() to clean up the message, since we requested that FormatMessage() allocate the buffer for us.
    //If message is nullptr, LocalFree() is never called.
    std::unique_ptr< WCHAR, HLOCAL (__stdcall*)( HLOCAL ) > uptr( message, &LocalFree );

    return utf16to8( message, messageSize + 1 );
}

/*
throwWindowsException{1}
------------------------

Description:
    Throws a WindowsException initialized with the given error code.

Arguments:
    N/A

Returns:
    N/A

Throws:
    WindowsException:  A WindowsException initialized with the given error code.
*/
void throwWindowsException( const DWORD errorCode ) {
    throw WindowsException( errorCode );
}

/*
throwWindowsException{2}
------------------------

Description:
    Throws a WindowsException initialized with the given error returned by GetLastError().

Arguments:
    N/A

Returns:
    N/A

Throws:
    WindowsException:  A WindowsException initialized with the error code returned by GetLastError().
*/
void throwWindowsException() {
    throwWindowsException( GetLastError() );
}




} //namespace Brimstone::Private
