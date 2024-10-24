/*
windows/WindowsException.hpp
----------------------------
Copyright (c) 2024, theJ89

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
#include <brimstone/Exception.hpp>  //Brimstone::IException




//Types
//NOTE: We're declaring this here to avoid having to include the Windows header.
using DWORD = unsigned long;




namespace Brimstone::Private {




//Forward declarations
void throwWindowsException();
void throwWindowsException( const DWORD errorCode );




class WindowsException : public IException {
public:
    WindowsException( const DWORD errorCode );

            DWORD     getErrorCode() const;
    virtual ustring   getDescription() const;
private:
    const DWORD m_errorCode;
};




} //namespace Brimstone::Private




#endif //BS_WINDOWS_WINDOWSEXCEPTION_HPP
