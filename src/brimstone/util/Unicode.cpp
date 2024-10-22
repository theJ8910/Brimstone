/*
util/Unicode.cpp
----------------
Copyright (c) 2024, theJ89

Description:
    See Unicode.hpp for more information.
*/




//Includes
#include <brimstone/util/Unicode.hpp>   //Header
#include <brimstone/Exception.hpp>      //Brimstone::SizeException




namespace Brimstone {




/*
utf8ToCodePoint{1}
------------------

Description:
    Returns the code point (an integral representation) of the first unicode character encoded in utf8Buffer.
    If additional unicode characters are encoded in the given buffer, they are ignored.

Arguments:
    utf8Buffer:                 Pointer to a buffer of UTF-8 encoded characters.
                                    Because it's a buffer and not a C-style string,
                                    it does NOT need to be null-terminated.
    byteCount:                  The number of bytes in the buffer.

Returns:
    uint32:                     The code point.

Throws:
    SizeException:              If byteCount is 0, or if a character is encoded with "N" bytes and byteCount < N.
    MalformedStringException:   If the UTF-8 string is not properly encoded.
*/
uint32 utf8ToCodePoint( const uchar* utf8Buffer, const std::size_t byteCount ) {
    //Byte count of 0
    if( byteCount == 0 )
        throw SizeException();

    uchar firstByte = *utf8Buffer;

    //Single byte sequence
    if( ( firstByte & 0x80 ) == 0 ) {
        return (uint32)firstByte;
    //Two-byte sequence
    } else if( ( firstByte & 0xE0 ) == 0xC0 ) {
        if( byteCount < 2 )
            throw SizeException();

        //Verify that the next byte is a continuation byte.
        if( ( (*( utf8Buffer + 1)) & 0xC0 ) != 0x80 ) {
            throw MalformedStringException();
        }

        return uint32( firstByte           & 0x1F ) <<  6 |
               uint32( (*(utf8Buffer + 1)) & 0x3F );
    //Three-byte sequence
    } else if( ( firstByte & 0xF0 ) == 0xE0 ) {
        if( byteCount < 3 )
            throw SizeException();

        //Verify that the next two bytes are continuation bytes.
        if( ( (*( utf8Buffer + 1)) & 0xC0 ) != 0x80 ||
            ( (*( utf8Buffer + 2)) & 0xC0 ) != 0x80    ) {
            throw MalformedStringException();
        }

        return uint32( firstByte           & 0x0F ) << 12 |
               uint32( (*(utf8Buffer + 1)) & 0x3F ) <<  6 |
               uint32( (*(utf8Buffer + 2)) & 0x3F );
    //Four-byte sequence
    } else if( ( firstByte & 0xF8 ) == 0xF0 ) {
        if( byteCount < 4 )
            throw SizeException();

        //Verify that the next three bytes are continuation bytes.
        if( ( (*( utf8Buffer + 1)) & 0xC0 ) != 0x80 ||
            ( (*( utf8Buffer + 2)) & 0xC0 ) != 0x80 ||
            ( (*( utf8Buffer + 3)) & 0xC0 ) != 0x80    ) {
            throw MalformedStringException();
        }

        return uint32( firstByte           & 0x07 ) << 18 |
               uint32( (*(utf8Buffer + 1)) & 0x3F ) << 12 |
               uint32( (*(utf8Buffer + 2)) & 0x3F ) <<  6 |
               uint32( (*(utf8Buffer + 3)) & 0x3F );
    //Malformed sequence
    } else {
        throw MalformedStringException();
    }
}

/*
utf8ToCodePoint{2}
------------------

Description:
    Same as utf8ToCodePoint{1}, but takes an STL string instead.
    Merely calls utf8ToCodePoint{1}, passing the .c_str() and .size() of the string.

Arguments:
    utf8:                       Pointer to a buffer of UTF-8 encoded characters.
                                    Because it's a buffer and not a C-style string,
                                    it does NOT need to be null-terminated.

Returns:
    uint32:                     The code point.

Throws:
    SizeException:              If byteCount is 0, or if a character is encoded with "N" bytes and byteCount < N.
    MalformedStringException:   If the UTF-8 string is not properly encoded.
*/
uint32 utf8ToCodePoint( const ustring& utf8 ) {
    return utf8ToCodePoint( utf8.c_str(), utf8.size() );
}




} //namespace Brimstone
