/*
Unicode.hpp
-----------
Copyright (c) 2024, theJ89

Description:
    Contains functions for working with Unicode.
*/
#ifndef BS_UTIL_UNICODE_HPP
#define BS_UTIL_UNICODE_HPP




//Includes
#include <cstddef>              //std::size_t

#include <brimstone/types.hpp>  //Brimstone::uchar, Brimstone::ustring




namespace Brimstone {




uint32 utf8ToCodePoint( const uchar* utf8Buffer, const std::size_t byteCount );
uint32 utf8ToCodePoint( const ustring& utf8 );




} //namespace Brimstone




#endif //BS_UTIL_UNICODE_HPP
