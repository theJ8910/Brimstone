/*
types.hpp
---------
Copyright (c) 2024, theJ89

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
#include <cstdint>      //std::int8_t, std::int16_t, etc




namespace Brimstone {

//Typedefs

//Unsigned variants of standard integral types
typedef unsigned short      ushort;
typedef unsigned int        uint;
typedef unsigned long       ulong;
typedef unsigned long long  ulonglong;

//Signed and unsigned fixed width integral types
typedef std::int8_t         int8;
typedef std::int16_t        int16;
typedef std::int32_t        int32;
typedef std::int64_t        int64;
typedef std::uint8_t        uint8;
typedef std::uint16_t       uint16;
typedef std::uint32_t       uint32;
typedef std::uint64_t       uint64;

//Signed and unsigned "native" integers
//These are 32-bits on a 32-bit build,
//and 64-bits on a 64-bit build.
#ifdef BS_BUILD_64BIT

typedef int64               intN;
typedef uint64              uintN;

#else //BS_BUILD_64BIT

typedef int32               intN;
typedef uint32              uintN;

#endif //BS_BUILD_64BIT

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

}




#endif //BS_TYPES_HPP
