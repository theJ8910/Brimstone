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
Takes a message. This message is displayed when getDescription() is called.
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
DivideByZeroException

Thrown when a divide by zero would have resulted.
*/
class DivideByZeroException : public IException {
public:
    virtual ustring getDescription() const;
};

/*
NullPointerException

Thrown when a pointer is unexpectedly nullptr
*/
class NullPointerException : public IException {
public:
    virtual ustring getDescription() const;
};

/*
SizeException

Thrown when something is smaller than expected
*/
class SizeException : public IException {
public:
    virtual ustring getDescription() const;
};

/*
OutOfBoundsException

Thrown when trying to get/set something outside of the bounds of a container.
*/
class OutOfBoundsException : public IException {
public:
    virtual ustring getDescription() const;
};

/*
NoSuchElementException

Thrown when trying to retrieve something that doesn't exist.
*/
class NoSuchElementException : public IException {
public:
    virtual ustring getDescription() const;
};

/*
UnexpectedResultException

Thrown when something that was expected to happen didn't.
*/
class UnexpectedResultException : public IException {
public:
    virtual ustring getDescription() const;
};

/*
UnexpectedResultException

Thrown when parsing data that doesn't adhere to the expected format
*/
class FormatException : public IException {
public:
    virtual ustring getDescription() const;
};

/*
IOException

Thrown when I/O fails; e.g. when opening, closing, reading from, or writing to a file.
*/
class IOException : public IException {
public:
    virtual ustring getDescription() const;
};

/*
CircularDependencyException

Thrown when adding a system and a circular dependency (A->B->C->...->A) is detected.
*/
class CircularDependencyException : public IException {
public:
    virtual ustring getDescription() const;
};

/*
NotImplementedException

Thrown when attempting to call a function that is not yet implemented.
*/
class NotImplementedException : public IException {
public:
    virtual ustring getDescription() const;
};

}




#endif //BS_EXCEPTION_HPP