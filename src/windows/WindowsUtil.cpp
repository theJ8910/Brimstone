/*
WindowsUtil.cpp
-----------------------
Copyright (c) 2014, theJ89
Assistance by Phillip Larkson.

Description:
    See WindowsUtil.hpp for more information.
*/




//Includes
#include <brimstone/windows/WindowsHeader.hpp>      //MultiByteToWideChar, WideCharToMultiByte

#include <brimstone/windows/WindowsUtil.hpp>        //Header file
#include <brimstone/windows/WindowsException.hpp>   //throwWindowsException




namespace Brimstone {
namespace Private {

wstring utf8to16( const ustring& strUTF8Bytes ) {
    //Determine how many characters (wchar_t) we'll need to encode this string as UTF-16
    int32 iChars;
    if( ( iChars = MultiByteToWideChar( CP_UTF8, 0, strUTF8Bytes.c_str(), -1, NULL, 0 ) ) == 0 )
        throwWindowsException();

    wstring strUTF16Chars( iChars - 1, L'\0' );

    //HACK: You're really not supposed to muck about with the internal buffer of the string...
    //we can probably get away with this, though. If this causes problems, change.
    if( MultiByteToWideChar( CP_UTF8, 0, strUTF8Bytes.c_str(), -1, &strUTF16Chars[0], iChars ) == 0 )
        throwWindowsException();

    return strUTF16Chars;
}

wstring utf8to16( const uchar* const pszUTF8Bytes, const int32 iUTF8ByteCount ) {
    //Determine how many characters (wchar_t) we'll need to encode this string as UTF-16
    int32 iChars;
    if( ( iChars = MultiByteToWideChar( CP_UTF8, 0, pszUTF8Bytes, iUTF8ByteCount, NULL, 0 ) ) == 0 )
        throwWindowsException();

    wstring strUTF16Chars( iChars - 1, L'\0' );

    //HACK: You're really not supposed to muck about with the internal buffer of the string...
    //we can probably get away with this, though. If this causes problems, change.
    if( MultiByteToWideChar( CP_UTF8, 0, pszUTF8Bytes, iUTF8ByteCount, &strUTF16Chars[0], iChars ) == 0 )
        throwWindowsException();

    return strUTF16Chars;
}

int32 utf8to16( const uchar* const pszUTF8Bytes, const int32 iUTF8ByteCount, wchar* const& pszUTF16CharsOut, const int32 iUTF16CharCount ) {
    int32 iChars;
    if( ( iChars = MultiByteToWideChar( CP_UTF8, 0, pszUTF8Bytes, iUTF8ByteCount, pszUTF16CharsOut, iUTF16CharCount ) ) == 0 )
        throwWindowsException();

    return iChars;
}

int32 utf8to16( const uchar* const pszUTF8Bytes, const int32 iUTF8ByteCount, wchar*& pszUTF16CharsOut ) {
    int32 iChars;
    if( ( iChars = MultiByteToWideChar( CP_UTF8, 0, pszUTF8Bytes, iUTF8ByteCount, NULL, 0 ) ) == 0 )
        throwWindowsException();

    std::unique_ptr< wchar > pszUTF16CharBuffer( new wchar[ iChars ] );

    if( ( iChars = MultiByteToWideChar( CP_UTF8, 0, pszUTF8Bytes, iUTF8ByteCount, pszUTF16CharBuffer.get(), iChars ) ) == 0 )
        throwWindowsException();

    pszUTF16CharsOut = pszUTF16CharBuffer.release();
    return iChars;
}

/*
utf16to8
-----------------------
Description:
	Converts from Windows' wchar_t-based little-endian UTF-16 encoding ("Unicode")
    to char-based UTF-8 encoding ("MultiByte").

    This version of the function takes a UTF-16 encoded string and returns a UTF-8 encoded string.
    This version is generally the safest to call, since STL strings ensure the strings are null-terminated,
    and handle the task of allocating / deleting appropriately sized buffers automatically.

Arguments:
    pszUTF16Chars:      Wide character string (std::wstring) holding the UTF-16 encoded string to convert.
    
Returns:
	ustring:            UTF-8 encoded string converted from the given string.
*/
ustring utf16to8( const wstring& strUTF16Chars ) {
    //Determine how many bytes we'll need to encode this string as UTF-8
    int32 iBytes;
    if( ( iBytes = WideCharToMultiByte( CP_UTF8, 0, strUTF16Chars.c_str(), -1, NULL, 0, NULL, NULL ) ) == 0 )
        throwWindowsException();

    ustring strUTF8Bytes( iBytes - 1, '\0' );

    //HACK: You're really not supposed to muck about with the internal buffer of the string...
    //we can probably get away with this, though. If this causes problems, change.
    if( WideCharToMultiByte( CP_UTF8, 0, strUTF16Chars.c_str(), -1, &strUTF8Bytes[0], iBytes, NULL, NULL ) == 0 )
        throwWindowsException();

    return strUTF8Bytes;
}

/*
utf16to8
-----------------------
Description:
	Converts from Windows' wchar_t-based little-endian UTF-16 encoding ("Unicode")
    to char-based UTF-8 encoding ("MultiByte").

    This version of the function returns a UTF-8 encoded string.

    WARNING:
        If the given string is not null terminated, the output will not be null terminated either.

Arguments:
    pszUTF16Chars:      Pointer to a buffer of wide characters (wchar_t) holding the UTF-16 encoded characters to convert.
    iUTF16CharCount:    The number of CHARACTERS (not bytes) to convert from pszUTF16.
                        Can be -1 if pszUTF16Chars is a null terminated string.
    pszUTF8BytesOut:    Pointer to a buffer of bytes (char) that will hold the UTF-8 encoded characters.
    
Returns:
	ustring:            UTF-8 encoded string converted from the given string.
*/
ustring utf16to8( const wchar* const pszUTF16Chars, const int32 iUTF16CharCount ) {
    //Determine how many bytes we'll need to encode these characters as UTF-8
    int32 iBytes;
    if( ( iBytes = WideCharToMultiByte( CP_UTF8, 0, pszUTF16Chars, iUTF16CharCount, NULL, 0, NULL, NULL ) ) == 0 )
        throwWindowsException();

    ustring strUTF8Buffer( iBytes - 1, '\0' );

    //HACK: You're really not supposed to muck about with the internal buffer of the string...
    //we can probably get away with this, though. If this causes problems, change.
    if( ( iBytes = WideCharToMultiByte( CP_UTF8, 0, pszUTF16Chars, iUTF16CharCount, &strUTF8Buffer[0], iBytes, NULL, NULL ) ) == 0 )
        throwWindowsException();

    return strUTF8Buffer;
}

/*
utf16to8
-----------------------
Description:
	Converts from Windows' wchar_t-based little-endian UTF-16 encoding ("Unicode")
    to char-based UTF-8 encoding ("MultiByte").

    This version of the function outputs the UTF-8 encoded characters to a fixed size buffer.

    WARNING:
        If the given string is not null terminated, the output will not be null terminated either.

    WARNING:
        pszUTF8BytesOut needs to be large enough to contain the output.
        If it isn't, bytes are still be written to it.

Arguments:
    pszUTF16Chars:      Pointer to a buffer of wide characters (wchar_t) holding the UTF-16 encoded characters to convert.
    iUTF16CharCount:    The number of CHARACTERS (wchar_t, not bytes) to convert from pszUTF16Chars.
                        Can be -1 if pszUTF16Chars is a null terminated string.
    pszUTF8BytesOut:    Pointer to a buffer of bytes (char) that will hold the UTF-8 encoded characters.
    iUTF8ByteCount:     The size of pszUTF8BytesOut in BYTES.
Returns:
	int:                The number of bytes written to the pszUTF8BytesOut.
*/
int32 utf16to8( const wchar* const pszUTF16Chars, const int32 iUTF16CharCount, uchar* const& pszUTF8BytesOut, const int32 iUTF8ByteCount ) {
    int32 iBytes;
    if( ( iBytes = WideCharToMultiByte( CP_UTF8, 0, pszUTF16Chars, iUTF16CharCount, pszUTF8BytesOut, iUTF8ByteCount, NULL, NULL ) ) == 0 )
        throwWindowsException();

    return iBytes;
}

/*
utf16to8
-----------------------
Description:
	Converts from Windows' wchar_t-based little-endian UTF-16 encoding ("Unicode")
    to char-based UTF-8 encoding ("MultiByte").

    This version of the function allocates a new buffer of the appropriate size and
    sets the given pointer to the new buffer.

    WARNING:
        If the given string is not null terminated, the output will not be null terminated either.

    WARNING:
        The buffer must be manaully freed with delete[] when no longer needed.

Arguments:
    pszUTF16Chars:      Pointer to a buffer of wide characters (wchar_t) holding the UTF-16 encoded characters to convert.
    iUTF16CharCount:    The number of CHARACTERS (not bytes) to convert from pszUTF16.
    pszUTF8BytesOut:    Pointer to a buffer of bytes (char) that will hold the UTF-8 encoded characters.
    
Returns:
	int:                The number of bytes written to the pszUTF8BytesOut.
*/
int32 utf16to8( const wchar* const pszUTF16Chars, const int32 iUTF16CharCount, uchar*& pszUTF8BytesOut ) {
    int32 iBytes;
    if( ( iBytes = WideCharToMultiByte( CP_UTF8, 0, pszUTF16Chars, iUTF16CharCount, NULL, 0, NULL, NULL ) ) == 0 )
        throwWindowsException();

    std::unique_ptr< uchar > pszUTF8ByteBuffer( new char[ iBytes ] );

    if( ( iBytes = WideCharToMultiByte( CP_UTF8, 0, pszUTF16Chars, iUTF16CharCount, pszUTF8ByteBuffer.get(), iBytes, NULL, NULL ) ) == 0 )
        throwWindowsException();

    pszUTF8BytesOut = pszUTF8ByteBuffer.release();
    return iBytes;
}

}
}