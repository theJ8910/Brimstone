/*
windows/WindowsException.hpp
-----------------------
Copyright (c) 2014, theJ89

Description:
    Defines the WindowsException class and a function to throw Windows exceptions.

    The WindowsException class stores an error code.
    Calling getDescription() generates the appropriate error message corresponding to this code.

    throwWindowsException() throws a WindowsException with the given code.
    If a code is not given, GetLastError() is called to supply the code of the last error that occured.
*/
#ifndef BS_WINDOWS_WINDOWSEXCEPTION_HPP
#define BS_WINDOWS_WINDOWSEXCEPTION_HPP




//Includes
#include "WindowsHeader.hpp"                //FormatMessage
#include <memory>                           //std::unique_ptr

#include "../Exception.hpp"                 //IException




namespace Brimstone {
namespace Private {

void throwWindowsException();
void throwWindowsException( const DWORD uiErrorCode );

class WindowsException : public IException {
private:
    static const std::string strErrorMessage;
public:
    WindowsException( DWORD uiErrorCode );

    DWORD getErrorCode() const;
    virtual ustring getDescription() const;
private:
    WindowsException& operator =( const WindowsException& );
private:
    const DWORD m_uiErrorCode;
};




}
}

#endif //BS_WINDOWS_WINDOWSEXCEPTION_HPP