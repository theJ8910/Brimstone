/*
Exception.cpp
-----------------------
Copyright (c) 2014, theJ89

Description:
    See Exception.hpp for more information.
*/

//Includes
#include <brimstone/Exception.hpp>

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

BS_DEFINE_BASIC_EXCEPTION( DivideByZeroException );
BS_DEFINE_BASIC_EXCEPTION( NullPointerException );
BS_DEFINE_BASIC_EXCEPTION( SizeException );
BS_DEFINE_BASIC_EXCEPTION( OutOfBoundsException );
BS_DEFINE_BASIC_EXCEPTION( NoSuchElementException );
BS_DEFINE_BASIC_EXCEPTION( UnexpectedResultException );
BS_DEFINE_BASIC_EXCEPTION( FormatException );
BS_DEFINE_BASIC_EXCEPTION( IOException );
BS_DEFINE_BASIC_EXCEPTION( CircularDependencyException );
BS_DEFINE_BASIC_EXCEPTION( NotImplementedException );
BS_DEFINE_BASIC_EXCEPTION( MalformedStringException );
BS_DEFINE_MESSAGE_EXCEPTION( LuaException );

}