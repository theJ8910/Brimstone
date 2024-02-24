/*
Exception.hpp
-------------
Copyright (c) 2024, theJ89

Description:
    Defines an interface for exceptions, IException.
    Provides several implementations of common exception types:
        * Exception (generic)
        * DivideByZero
        * DomainException
        * NullPointerException
        * SizeException
        * BoundsException
        * SingletonException
        * NoSuchElementException
        * UnexpectedResultException
        * FormatException
        * IOException
        * NotImplementedException
        * MalformedStringException
        * GraphicsException
        * LuaException
*/

#ifndef BS_EXCEPTION_HPP
#define BS_EXCEPTION_HPP




//Includes
#include <brimstone/types.hpp>  //uchar, ustring




namespace Brimstone {

/*
IException

All exceptions implement this interface
*/
class IException {
public:
    virtual ustring getDescription() const = 0;
};

/*
Exception

Generic exception implementation.
Takes a message. This message is returned when getDescription() is called.
*/
class Exception : public IException {
public:
    Exception();
    Exception( const uchar* description );
    Exception( const ustring& description );

    virtual ustring getDescription() const;
private:
    ustring m_description;
};

/*
BS_DECLARE_BASIC_EXCEPTION

Macro to automatically declare a basic exception type, without a message
*/
#define BS_DECLARE_BASIC_EXCEPTION( exceptionName ) \
    class exceptionName : public IException { \
    public: \
        virtual ustring getDescription() const; \
    }

/*
BS_DECLARE_MESSAGE_EXCEPTION

Macro to automatically declare an exception type deriving from Exception.
Any exceptions using this macro take a message, which is returned when
getDescription() is called.
*/
#define BS_DECLARE_MESSAGE_EXCEPTION( exceptionName ) \
    class exceptionName : public Exception { \
    public: \
        exceptionName(); \
        exceptionName( const uchar* description ); \
        exceptionName( const ustring& description ); \
    }

/*
DivideByZeroException

Thrown when a divide by zero would have resulted.
*/
BS_DECLARE_BASIC_EXCEPTION( DivideByZeroException );

/*
DomainException

Thrown when invalid input is provided to a mathematics function.
*/
BS_DECLARE_BASIC_EXCEPTION( DomainException );

/*
NullPointerException

Thrown when a pointer is unexpectedly nullptr
*/
BS_DECLARE_BASIC_EXCEPTION( NullPointerException );

/*
SizeException

Thrown when something is smaller than expected
*/
BS_DECLARE_BASIC_EXCEPTION( SizeException );

/*
BoundsException

Thrown when trying to get/set something outside of the bounds of a container,
or when a pair of bounds, is specified in the wrong order (i.e. min > max)
*/
BS_DECLARE_BASIC_EXCEPTION( BoundsException );

/*
SingletonException

Thrown when two instances of a singleton class are instantiated at the same time.
*/
BS_DECLARE_BASIC_EXCEPTION( SingletonException );

/*
NoSuchElementException

Thrown when trying to retrieve something that doesn't exist.
*/
BS_DECLARE_BASIC_EXCEPTION( NoSuchElementException );

/*
UnexpectedResultException

Thrown when something that was expected to happen didn't,
or vice versa.
*/
BS_DECLARE_BASIC_EXCEPTION( UnexpectedResultException );

/*
FormatException

Thrown when parsing data that doesn't adhere to the expected format
*/
BS_DECLARE_BASIC_EXCEPTION( FormatException );

/*
IOException

Thrown when I/O fails; e.g. when opening, closing, reading from, or writing to a file.
*/
BS_DECLARE_BASIC_EXCEPTION( IOException );

/*
NotImplementedException

Thrown when attempting to call a function that is not yet implemented.
*/
BS_DECLARE_BASIC_EXCEPTION( NotImplementedException );

/*
MalformedStringException

Thrown when a function that handles a UTF-8 encoded string determines that
the string is malformed.
*/
BS_DECLARE_BASIC_EXCEPTION( MalformedStringException );

/*
GraphicsException

Thrown when an error related to Graphics occurs (e.g. compiling a shader or linking a shader program fails).
*/
BS_DECLARE_MESSAGE_EXCEPTION( GraphicsException );

/*
LuaException

Thrown when an error related to Lua occurs (e.g. loading a script or calling a function fails).
*/
BS_DECLARE_MESSAGE_EXCEPTION( LuaException );

template< typename Signature >
class Delegate;

typedef Delegate<void( const IException& )> UncaughtExceptionHandler;

/*
uncaughtException

Call this function to handle an exception that cannot be
caught by the user. By default, calling this function logs the exception as an error.

Typically you'll want to catch an exception thrown in a destructor,
and pass the thrown exception to uncaughtException.
For example:

MyClass::~MyClass() {
    try {
        //Some method that can throw exceptions
        close();
    } catch( const IException& ex ) {
        uncaughtException( ex );
    }
}

You can set/get the uncaught exception handler with
setUncaughtExceptionHandler and getUncaughtExceptionHandler.
*/
void uncaughtException( const IException& exception );

void setUncaughtExceptionHandler( UncaughtExceptionHandler handler );
UncaughtExceptionHandler getUncaughtExceptionHandler();

}




//Not needed outside of this file
#undef BS_DECLARE_BASIC_EXCEPTION
#undef BS_DECLARE_MESSAGE_EXCEPTION




#endif //BS_EXCEPTION_HPP
