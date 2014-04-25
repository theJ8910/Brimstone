/*
Unicode.hpp
-----------------------
Copyright (c) 2014, theJ89

Description:
    Contains functions for working with Unicode.
*/
#ifndef BS_UNICODE_HPP
#define BS_UNICODE_HPP




//Includes
#include <brimstone/types.hpp>

namespace Brimstone {

uint32 utf8ToCodePoint( const uchar* pszUTF8Buffer, const size_t uiByteCount );
uint32 utf8ToCodePoint( const ustring& strUTF8 );

}


#endif //BS_UNICODE_HPP