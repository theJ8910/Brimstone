/*
Unicode.hpp
-----------------------
Copyright (c) 2014, theJ89

Description:
    Contains functions for working with Unicode.
*/
#ifndef BS_UTIL_UNICODE_HPP
#define BS_UTIL_UNICODE_HPP




//Includes
#include <brimstone/types.hpp>  //uchar, ustring&




namespace Brimstone {

uint32 utf8ToCodePoint( const uchar* utf8Buffer, const size_t byteCount );
uint32 utf8ToCodePoint( const ustring& utf8 );

}




#endif //BS_UTIL_UNICODE_HPP