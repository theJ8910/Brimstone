/*
WindowsUtil.hpp
-----------------------
Copyright (c) 2014, theJ89

Description:
    Utility functions for Windows.

    This file contains several functions for converting from the UTF-8 encoded strings
    the engine uses to/from the UTF-16 encoded strings Windows uses.
*/

#ifndef BS_WINDOWS_UTIL_HPP
#define BS_WINDOWS_UTIL_HPP




//Includes
#include "../types.hpp"        //uchar, wchar, int32




namespace Brimstone {
namespace Private {

wstring utf8to16( const ustring& strUTF8Bytes );
wstring utf8to16( const uchar* const pszUTF8Bytes, const int32 iUTF8ByteCount );
int32   utf8to16( const uchar* const pszUTF8Bytes, const int32 iUTF8ByteCount, wchar* const& pszUTF16CharsOut, const int32 iUTF16CharCount );
int32   utf8to16( const uchar* const pszUTF8Bytes, const int32 iUTF8ByteCount, wchar*& pszUTF16CharsOut );

ustring utf16to8( const wstring& strUTF16Chars );
ustring utf16to8( const wchar* const pszUTF16Chars, const int32 iUTF16CharCount );
int32   utf16to8( const wchar* const pszUTF16Chars, const int32 iUTF16CharCount, uchar* const& pszUTF8BytesOut, const int32 iUTF8ByteCount );
int32   utf16to8( const wchar* const pszUTF16Chars, const int32 iUTF16CharCount, uchar*& pszUTF8BytesOut );

}
}




#endif //BS_WINDOWS_UTIL_HPP