/*
windows/WindowsUtil.cpp
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

wstring utf8to16( const ustring& utf8Bytes ) {
    //Determine how many characters (wchar_t) we'll need to encode this string as UTF-16
    int32 charCount;
    if( ( charCount = MultiByteToWideChar( CP_UTF8, 0, utf8Bytes.c_str(), -1, nullptr, 0 ) ) == 0 )
        throwWindowsException();

    wstring utf16Chars( charCount - 1, L'\0' );

    //HACK: You're really not supposed to muck about with the internal buffer of the string...
    //we can probably get away with this, though. If this causes problems, change.
    if( MultiByteToWideChar( CP_UTF8, 0, utf8Bytes.c_str(), -1, &utf16Chars[0], charCount ) == 0 )
        throwWindowsException();

    return utf16Chars;
}

wstring utf8to16( const uchar* const utf8Bytes, const int32 utf8ByteCount ) {
    //Determine how many characters (wchar_t) we'll need to encode this string as UTF-16
    int32 charCount;
    if( ( charCount = MultiByteToWideChar( CP_UTF8, 0, utf8Bytes, utf8ByteCount, nullptr, 0 ) ) == 0 )
        throwWindowsException();

    wstring utf16Chars( charCount - 1, L'\0' );

    //HACK: You're really not supposed to muck about with the internal buffer of the string...
    //we can probably get away with this, though. If this causes problems, change.
    if( MultiByteToWideChar( CP_UTF8, 0, utf8Bytes, utf8ByteCount, &utf16Chars[0], charCount ) == 0 )
        throwWindowsException();

    return utf16Chars;
}

int32 utf8to16( const uchar* const utf8Bytes, const int32 utf8ByteCount, wchar* const& utf16CharsOut, const int32 utf16CharCount ) {
    int32 charCount;
    if( ( charCount = MultiByteToWideChar( CP_UTF8, 0, utf8Bytes, utf8ByteCount, utf16CharsOut, utf16CharCount ) ) == 0 )
        throwWindowsException();

    return charCount;
}

int32 utf8to16( const uchar* const utf8Bytes, const int32 utf8ByteCount, wchar*& utf16CharsOut ) {
    int32 charCount;
    if( ( charCount = MultiByteToWideChar( CP_UTF8, 0, utf8Bytes, utf8ByteCount, nullptr, 0 ) ) == 0 )
        throwWindowsException();

    std::unique_ptr< wchar[] > utf16CharBuffer( new wchar[ charCount ] );

    if( ( charCount = MultiByteToWideChar( CP_UTF8, 0, utf8Bytes, utf8ByteCount, utf16CharBuffer.get(), charCount ) ) == 0 )
        throwWindowsException();

    utf16CharsOut = utf16CharBuffer.release();
    return charCount;
}

/*
utf16to8{1}
-----------------------
Description:
    Converts from Windows' wchar_t-based little-endian UTF-16 encoding ("Unicode")
    to char-based UTF-8 encoding ("MultiByte").

    This version of the function takes a UTF-16 encoded string and returns a UTF-8 encoded string.
    This version is generally the safest to call, since STL strings ensure the strings are null-terminated,
    and handle the task of allocating / deleting appropriately sized buffers automatically.

Arguments:
    utf16Chars:         Wide character string (std::wstring) holding the UTF-16 encoded string to convert.
    
Returns:
    ustring:            UTF-8 encoded string converted from the given string.
*/
ustring utf16to8( const wstring& utf16Chars ) {
    //Determine how many bytes we'll need to encode this string as UTF-8
    int32 byteCount;
    if( ( byteCount = WideCharToMultiByte( CP_UTF8, 0, utf16Chars.c_str(), -1, nullptr, 0, nullptr, nullptr ) ) == 0 )
        throwWindowsException();

    ustring utf8Bytes( byteCount - 1, '\0' );

    //HACK: You're really not supposed to muck about with the internal buffer of the string...
    //we can probably get away with this, though. If this causes problems, change.
    if( WideCharToMultiByte( CP_UTF8, 0, utf16Chars.c_str(), -1, &utf8Bytes[0], byteCount, nullptr, nullptr ) == 0 )
        throwWindowsException();

    return utf8Bytes;
}

/*
utf16to8{2}
-----------------------
Description:
    Converts from Windows' wchar_t-based little-endian UTF-16 encoding ("Unicode")
    to char-based UTF-8 encoding ("MultiByte").

    This version of the function returns a UTF-8 encoded string.

    WARNING:
        If the given string is not null terminated, the output will not be null terminated either.

Arguments:
    utf16Chars:         Pointer to a buffer of wide characters (wchar_t) holding the UTF-16 encoded characters to convert.
    utf16CharCount:     The number of CHARACTERS (not bytes) to convert from utf16Chars.
                            Can be -1 if utf16Chars is a null terminated string.
    utf8BytesOut:       Pointer to a buffer of bytes (char) that will hold the UTF-8 encoded characters.
    
Returns:
    ustring:            UTF-8 encoded string converted from the given string.
*/
ustring utf16to8( const wchar* const utf16Chars, const int32 utf16CharCount ) {
    //Determine how many bytes we'll need to encode these characters as UTF-8
    int32 byteCount;
    if( ( byteCount = WideCharToMultiByte( CP_UTF8, 0, utf16Chars, utf16CharCount, nullptr, 0, nullptr, nullptr ) ) == 0 )
        throwWindowsException();

    ustring utf8Buffer( utf16CharCount - 1, '\0' );

    //HACK: You're really not supposed to muck about with the internal buffer of the string...
    //we can probably get away with this, though. If this causes problems, change.
    if( ( byteCount = WideCharToMultiByte( CP_UTF8, 0, utf16Chars, utf16CharCount, &utf8Buffer[0], byteCount, nullptr, nullptr ) ) == 0 )
        throwWindowsException();

    return utf8Buffer;
}

/*
utf16to8{3}
-----------------------
Description:
    Converts from Windows' wchar_t-based little-endian UTF-16 encoding ("Unicode")
    to char-based UTF-8 encoding ("MultiByte").

    This version of the function outputs the UTF-8 encoded characters to a fixed size buffer.

    WARNING:
        If the given string is not null terminated, the output will not be null terminated either.

    WARNING:
        utf8BytesOut needs to be large enough to contain the output.
        If it isn't, bytes are still be written to it.

Arguments:
    utf16Chars:         Pointer to a buffer of wide characters (wchar_t) holding the UTF-16 encoded characters to convert.
    utf16CharCount:     The number of CHARACTERS (wchar_t, not bytes) to convert from utf16Chars.
                            Can be -1 if utf16Chars is a null terminated string.
    utf8BytesOut:       Pointer to a buffer of bytes (char) that will hold the UTF-8 encoded characters.
    utf8ByteCount:      The size of utf8BytesOut in BYTES.
Returns:
    int:                The number of bytes written to the utf8BytesOut.
*/
int32 utf16to8( const wchar* const utf16Chars, const int32 utf16CharCount, uchar* const& utf8BytesOut, const int32 utf8ByteCount ) {
    int32 byteCount;
    if( ( byteCount = WideCharToMultiByte( CP_UTF8, 0, utf16Chars, utf16CharCount, utf8BytesOut, utf8ByteCount, nullptr, nullptr ) ) == 0 )
        throwWindowsException();

    return byteCount;
}

/*
utf16to8{4}
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
    utf16Chars:         Pointer to a buffer of wide characters (wchar_t) holding the UTF-16 encoded characters to convert.
    utf16CharCount:     The number of CHARACTERS (not bytes) to convert from utf16Chars.
    utf8BytesOut:       Pointer to a buffer of bytes (char) that will hold the UTF-8 encoded characters.
    
Returns:
    int:                The number of bytes written to the utf8BytesOut.
*/
int32 utf16to8( const wchar* const utf16Chars, const int32 utf16CharCount, uchar*& utf8BytesOut ) {
    int32 byteCount;
    if( ( byteCount = WideCharToMultiByte( CP_UTF8, 0, utf16Chars, utf16CharCount, nullptr, 0, nullptr, nullptr ) ) == 0 )
        throwWindowsException();

    std::unique_ptr< uchar > utf8ByteBuffer( new char[ byteCount ] );

    if( ( byteCount = WideCharToMultiByte( CP_UTF8, 0, utf16Chars, utf16CharCount, utf8ByteBuffer.get(), byteCount, nullptr, nullptr ) ) == 0 )
        throwWindowsException();

    utf8BytesOut = utf8ByteBuffer.release();
    return byteCount;
}

}
}