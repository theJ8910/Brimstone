/*
types.hpp
-----------------------
Copyright (c) 2014, theJ89

Description:
    This file defines several types the engine uses for the purpose of readability,
    portability, and/or fast refactoring.

    This header should be included anywhere these types are needed.
*/

#ifndef BS_TYPES_HPP
#define BS_TYPES_HPP




//Includes
#include <string>       //std::string, std::wstring
#include <sstream>      //ostream, istream, stringstream, istringstream, ostringstream, etc
#include <stdint.h>     //int8_t, int16_t, etc




namespace Brimstone {

//Typedefs
typedef unsigned char       ubyte;
typedef unsigned short      ushort;
typedef unsigned int        uint;
typedef unsigned long       ulong;

typedef int8_t              int8;
typedef int16_t             int16;
typedef int32_t             int32;
typedef int64_t             int64;
typedef uint8_t             uint8;
typedef uint16_t            uint16;
typedef uint32_t            uint32;
typedef uint64_t            uint64;

#ifdef BS_BUILD_64BIT

typedef int64               intN;
typedef uint64              uintN;

#else

typedef int32               intN;
typedef uint32              uintN;

#endif

//Bytes are represented with characters
typedef char                byte;
typedef unsigned char       ubyte;

//UTF-8 strings are encoded as a sequence of bytes
typedef byte                uchar;
typedef std::string         ustring;
typedef std::ostream        uostream;
typedef std::istream        uistream;
typedef std::stringstream   usstream;
typedef std::ostringstream  uosstream;
typedef std::istringstream  uisstream;

//UTF-16 strings (such as the ones used by Windows) are represented with wide characters (wchar_t)
typedef wchar_t             wchar;
typedef std::wstring        wstring;
typedef std::wostream       wostream;
typedef std::wistream       wistream;
typedef std::wstringstream  wsstream;
typedef std::wostringstream wosstream;
typedef std::wistringstream wisstream;

//This is a pretty important typedef. Vectors and matrices (among other things) use this datatype for their components.
//Like the name suggests, this should be something that can represent reals, like float or double.
//floats are faster, but doubles are more accurate.
typedef float real;


//Constants
#if defined( BS_BUILD_WINDOWS )

#define ASCII_NEWLINE       "\r\n";
#define UNICODE_NEWLINE     L"\r\n";

#elif defined( BS_BUILD_LINUX ) || defined( BS_BUILD_MAC )

#define ASCII_NEWLINE       "\n";
#define UNICODE_NEWLINE     L"\n";

#endif

}




#endif //BS_TYPES_HPP