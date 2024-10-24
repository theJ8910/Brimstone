/*
windows/WindowsUtil.hpp
-----------------------
Copyright (c) 2024, theJ89

Description:
    Utility functions for Windows.

    This file contains several functions for converting from the UTF-8 encoded strings
    the engine uses to/from the UTF-16 encoded strings Windows uses.
*/
#ifndef BS_WINDOWS_WINDOWSUTIL_HPP
#define BS_WINDOWS_WINDOWSUTIL_HPP




//Includes
#include <brimstone/types.hpp>  //Brimstone::ustring, Brimstone::uchar, Brimstone::wchar, Brimstone::int32




namespace Brimstone::Private {




wstring utf8to16( const ustring& utf8Bytes );
wstring utf8to16( const uchar* const utf8Bytes, const int32 utf8ByteCount );
int32   utf8to16( const uchar* const utf8Bytes, const int32 utf8ByteCount, wchar* const& utf16CharsOut, const int32 utf16CharCount );

ustring utf16to8( const wstring& utf16Chars );
ustring utf16to8( const wchar* const utf16Chars, const int32 utf16CharCount );
int32   utf16to8( const wchar* const utf16Chars, const int32 utf16CharCount, uchar* const& utf8BytesOut, const int32 utf8ByteCount );




} //namespace Brimstone::Private




#endif //BS_WINDOWS_WINDOWSUTIL_HPP
