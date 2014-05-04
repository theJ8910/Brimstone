/*
Exception.hpp
-----------------------
Copyright (c) 2014, theJ89

Description:
    Defines an interface for exceptions, IException.
    Provides several implementations of common exception types:
        * Exception (generic)
        * DivideByZero
        * NullPointerException
        * SizeException
        * OutOfBoundsException
        * NoSuchElementException
        * UnexpectedResultException
        * FormatException
        * IOException
        * CircularDependencyException
        * NotImplementedException
        * MalformedStringException
        * LuaException
*/

#ifndef BS_EXCEPTION_HPP
#define BS_EXCEPTION_HPP




//Includes
#include "types.hpp"    //uchar, ustring




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
    Exception( const uchar* pszDescription );
    Exception( const ustring& strDescription );

    virtual ustring getDescription() const;
private:
     ustring m_strDescription;
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
BS_DEFINE_BASIC_EXCEPTION

Macro to automatically define a basic exception type, without a message
*/
#define BS_DEFINE_BASIC_EXCEPTION( exceptionName ) \
    ustring exceptionName::getDescription() const { \
        return #exceptionName; \
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
        exceptionName( const uchar* pszDescription ); \
        exceptionName( const ustring& strDescription ); \
    }

/* 
BS_DEFINE_MESSAGE_EXCEPTION

Macro to automatically define an exception type deriving from Exception.
Any exceptions using this macro take a message, which is returned when
getDescription() is called.
*/
#define BS_DEFINE_MESSAGE_EXCEPTION( exceptionName ) \
    exceptionName::exceptionName() { \
    } \
    exceptionName::exceptionName( const uchar* pszDescription ) : Exception( pszDescription ) { \
    } \
    exceptionName::exceptionName( const ustring& strDescription ) : Exception( strDescription ) { \
    }

/*
DivideByZeroException

Thrown when a divide by zero would have resulted.
*/
BS_DECLARE_BASIC_EXCEPTION( DivideByZeroException );

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
OutOfBoundsException

Thrown when trying to get/set something outside of the bounds of a container.
*/
BS_DECLARE_BASIC_EXCEPTION( OutOfBoundsException );

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
CircularDependencyException

Thrown when adding a system and a circular dependency (A->B->C->...->A) is detected.
*/
BS_DECLARE_BASIC_EXCEPTION( CircularDependencyException );

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
LuaException

Thrown when an error related to Lua occurs (e.g. loading a script or calling a function fails).
*/
BS_DECLARE_MESSAGE_EXCEPTION( LuaException );

}




#endif //BS_EXCEPTION_HPP