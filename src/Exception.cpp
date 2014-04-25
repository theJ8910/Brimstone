/*
Exception.cpp
-----------------------
Copyright (c) 2014, theJ89

Description:
    See Exception.hpp for more information.
*/

//Includes
#include <brimstone/Exception.hpp>
//#include "../Language.hpp"

namespace Brimstone {

Exception::Exception() {
}

Exception::Exception( const uchar* pszDescription )
    : m_strDescription( pszDescription ) {
}

Exception::Exception( const ustring& strDescription )
    : m_strDescription( strDescription ) {
}

ustring Exception::getDescription() const {
    return m_strDescription;
};

ustring DivideByZeroException::getDescription() const {
    return "DivideByZeroException";
    //return Language::get( "DivideByZeroException_Description" );
}

ustring NullPointerException::getDescription() const {
    return "NullPointerException";
    //return Language::get( "NullPointerException_Description" );
}

ustring SizeException::getDescription() const {
    return "SizeException";
    //return Language::get( "SizeException_Description" );
}

ustring OutOfBoundsException::getDescription() const {
    return "OutOfBoundsException";
    //return Language::get( "OutOfBoundsException_Description" );
}

ustring NoSuchElementException::getDescription() const {
    return "NoSuchElementException";
    //return Language::get( "NoSuchElementException_Description" );
}

ustring UnexpectedResultException::getDescription() const {
    return "UnexpectedResultException";
    //return Language::get( "UnexpectedResultException_Description" );
}

ustring FormatException::getDescription() const {
    return "FormatException";
    //return Language::get( "FormatException_Description" );
}

ustring IOException::getDescription() const {
    return "IOException";
    //return Language::get( "IOException_Description" );
}

ustring CircularDependencyException::getDescription() const {
    return "CircularDependencyException";
    //return Language::get( "CircularDependencyException_Description" );
}

ustring NotImplementedException::getDescription() const {
    return "NotImplementedException";
    //return Language::get( "NotImplementedException_Description" );
}

ustring MalformedStringException::getDescription() const {
    return "MalformedStringException";
    //return Language::get( "MalformedStringException_Description" );
}

}