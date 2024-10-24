/*
linux/LinuxException.hpp
------------------------
Copyright (c) 2024, theJ89

Description:
    Defines the LinuxException class and a function to throw LinuxExceptions.

    The LinuxException class stores an error code (errno).
    Calling getDescription() generates the appropriate error message corresponding to this code.

    throwLinuxException() throws a LinuxException with the given code.
    If a code is not given, then the code of the last error that occurred on this thread (stored in errno) supplied.
*/
#ifndef BS_LINUX_LINUXEXCEPTION_HPP
#define BS_LINUX_LINUXEXCEPTION_HPP




//Includes
#include <brimstone/Exception.hpp>  //Brimstone::IException




namespace Brimstone::Private {




//Forward declarations
void throwLinuxException( const int errorCode );
void throwLinuxException();




class LinuxException : public IException {
public:
    LinuxException( const int errorCode );

            int     getErrorCode() const;
    virtual ustring getDescription() const;
private:
    int m_errorCode;
};




}




#endif //BS_LINUX_LINUXEXCEPTION_HPP
