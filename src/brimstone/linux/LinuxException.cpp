/*
linux/LinuxException.cpp
------------------------
Copyright (c) 2024, theJ89

Description:
    See LinuxException.hpp for more information.
*/




//Includes
#include <brimstone/linux/LinuxException.hpp>  //Header

#include <errno.h>                             //errno
#include <string.h>                            //strerror_r




namespace {




//Constants
//Maximum number of characters we can retrieve from a call to strerror_r
constexpr int STRERROR_BUFFER_SIZE = 1024;




} //namespace




namespace Brimstone::Private {




/*
LinuxException::LinuxException
------------------------------

Description:
    Constructor. Initializes the LinuxException with the given error code.

Arguments:
    errorCode:  The error code.

Returns:
    N/A
*/
LinuxException::LinuxException( const int errorCode ) :
    m_errorCode( errorCode ) {
}

/*
LinuxException::getErrorCode
----------------------------

Description:
    Returns the error code for this exception.

Arguments:
    N/A

Returns:
    int:  The error code.
*/
int LinuxException::getErrorCode() const {
    return m_errorCode;
}

/*
LinuxException::getDescription
------------------------------

Description:
    Returns a string describing the error code for this exception.

Arguments:
    N/A

Returns:
    ustring:  A description of the error code.
*/
ustring LinuxException::getDescription() const {
    ustring msg( 1024, '\0' );
    //Use POSIX-compliant strerror_r:
#if _POSIX_C_SOURCE >= 200112L && ! _GNU_SOURCE
    int rv = strerror_r( m_errorCode, msg.data(), STRERROR_BUFFER_SIZE );
    if( rv != 0 ) {
        //NOTE:
        //    rv == 0 indicates success, rv != 0 indicates failure.
        //    Prior to glibc 2.13, if rv == -1, then the error code is set in errno.
        //    Since glib 2.13, if rv > 0, then the error code is rv.
        if( rv != -1 )
            throwLinuxException( rv );
        else
            throwLinuxException();
    }
    //Use GNU-specific strerror_r:
#else //_POSIX_C_SOURCE >= 200112L && ! _GNU_SOURCE
    char* data = msg.data();
    char* rv = strerror_r( m_errorCode, data, STRERROR_BUFFER_SIZE );
    
    if( rv == data ) { msg.resize( strlen( data ) ); } //strerror_r used the buffer we gave it.
    else             { msg = rv;                     } //strerror_r returned a static string.
#endif //_POSIX_C_SOURCE >= 200112L && ! _GNU_SOURCE
    return msg;
}

/*
throwLinuxException{1}
----------------------

Description:
    Throws a LinuxException initialized with the given error code.

Arguments:
    errorCode:  The error code.

Returns:
    N/A

Throws:
    LinuxException:  A LinuxException initialized with the given errorCode.
*/
void throwLinuxException( const int errorCode ) {
    throw LinuxException( errorCode );
}

/*
throwLinuxException{2}
----------------------

Description:
    Throws a LinuxException initialized with the current value of errno (set by functions in the C standard library and other libraries) for this thread.

Arguments:
    N/A

Returns:
    N/A

Throws:
    LinuxException:  A LinuxException initialized with errno.
*/
void throwLinuxException() {
    throwLinuxException( errno );
}




} //namespace Brimstone::Private
