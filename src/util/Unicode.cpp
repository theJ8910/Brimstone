/*
Unicode.cpp
-----------------------
Copyright (c) 2014, theJ89

Description:
    See Unicode.hpp for more information.
*/

//Includes
#include <brimstone/util/Unicode.hpp>   //Header file
#include <brimstone/Exception.hpp>      //SizeException

namespace Brimstone {

uint32 utf8ToCodePoint( const uchar* pszUTF8Buffer, const size_t uiByteCount ) {
    //Byte count of 0
    if( uiByteCount == 0 )
        return 0;

    uchar cByte = *pszUTF8Buffer;

    //Single byte sequence
    if( ( cByte & 0x80 ) == 0 ) {
        return (uint32)cByte;
    //Two-byte sequence
    } else if( ( cByte & 0xE0 ) == 0xC0 ) {
        if( uiByteCount < 2 )
            throw SizeException();

        //Verify that the next byte is a continuation byte. 
        if( ( (*( pszUTF8Buffer + 1)) & 0xC0 ) != 0x80 ) {
            throw MalformedStringException();
        }

        return uint32( cByte                  & 0x1F ) <<  6 |
               uint32( (*(pszUTF8Buffer + 1)) & 0x3F );
    //Three-byte sequence
    } else if( ( cByte & 0xF0 ) == 0xE0 ) {
        if( uiByteCount < 3 )
            throw SizeException();

        //Verify that the next two bytes are continuation bytes.
        if( ( (*( pszUTF8Buffer + 1)) & 0xC0 ) != 0x80 ||
            ( (*( pszUTF8Buffer + 2)) & 0xC0 ) != 0x80    ) {
            throw MalformedStringException();
        }

        return uint32( cByte                  & 0x0F ) << 12 |
               uint32( (*(pszUTF8Buffer + 1)) & 0x3F ) <<  6 |
               uint32( (*(pszUTF8Buffer + 2)) & 0x3F );
    //Four-byte sequence
    } else if( ( cByte & 0xF8 ) == 0xF0 ) {
        if( uiByteCount < 4 )
            throw SizeException();

        //Verify that the next three bytes are continuation bytes.
        if( ( (*( pszUTF8Buffer + 1)) & 0xC0 ) != 0x80 ||
            ( (*( pszUTF8Buffer + 2)) & 0xC0 ) != 0x80 ||
            ( (*( pszUTF8Buffer + 3)) & 0xC0 ) != 0x80    ) {
            throw MalformedStringException();
        }

        return uint32( cByte                  & 0x07 ) << 18 |
               uint32( (*(pszUTF8Buffer + 1)) & 0x3F ) << 12 |
               uint32( (*(pszUTF8Buffer + 2)) & 0x3F ) <<  6 |
               uint32( (*(pszUTF8Buffer + 3)) & 0x3F );
    //Malformed sequence
    } else {
        throw MalformedStringException();
    }
}

uint32 utf8ToCodePoint( const ustring& strUTF8 ) {
    //TODO: size_t is uint64 on x64, uint32 on x86. Handle this
    return utf8ToCodePoint( strUTF8.c_str(), strUTF8.size() );
}

}