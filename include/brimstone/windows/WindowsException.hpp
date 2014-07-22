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
#include <brimstone/windows/WindowsHeader.hpp>  //FormatMessage
#include <memory>                               //std::unique_ptr

#include <brimstone/Exception.hpp>              //IException




namespace Brimstone {
namespace Private {

void throwWindowsException();
void throwWindowsException( const DWORD errorCode );

class WindowsException : public IException {
private:
    static const std::string m_errorMessage;
public:
    WindowsException( DWORD errorCode );

            DWORD       getErrorCode() const;
    virtual ustring     getDescription() const;
private:
    WindowsException&   operator =( const WindowsException& );
private:
    const DWORD m_errorCode;
};




}
}

#endif //BS_WINDOWS_WINDOWSEXCEPTION_HPP