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
#include <string>   //std::string, std::wstring
#include <sstream>  //std::ostream, std::istream, std::stringstream, std::istringstream, std::ostringstream, etc.
#include <cstdint>  //std::int8_t, std::int16_t, etc.




namespace Brimstone {




//Types

//Unsigned variants of standard integral types
using ushort    = unsigned short;
using uint      = unsigned int;
using ulong     = unsigned long;
using ulonglong = unsigned long long;

//Signed and unsigned fixed width integral types
using int8   = std::int8_t;
using int16  = std::int16_t;
using int32  = std::int32_t;
using int64  = std::int64_t;
using uint8  = std::uint8_t;
using uint16 = std::uint16_t;
using uint32 = std::uint32_t;
using uint64 = std::uint64_t;

//Signed and unsigned "native" integers
//These are 32-bits on a 32-bit build,
//and 64-bits on a 64-bit build.
#ifdef BS_BUILD_64BIT

using intN  = int64;
using uintN = uint64;

#else //BS_BUILD_64BIT

using intN  = int32;
using uintN = uint32;

#endif //BS_BUILD_64BIT

//Bytes are represented with characters
using byte  = char;
using ubyte = unsigned char;

//UTF-8 strings are encoded as a sequence of bytes
using uchar     = byte;
using ustring   = std::string;
using uostream  = std::ostream;
using uistream  = std::istream;
using usstream  = std::stringstream;
using uosstream = std::ostringstream;
using uisstream = std::istringstream;

//UTF-16 strings (such as the ones used by Windows) are represented with wide characters (wchar_t)
using wchar     = wchar_t;
using wstring   = std::wstring;
using wostream  = std::wostream;
using wistream  = std::wistream;
using wsstream  = std::wstringstream;
using wosstream = std::wostringstream;
using wisstream = std::wistringstream;

//This is a pretty important alias. Vectors and matrices (among other things) use this datatype for their components.
//Like the name suggests, this should be something that can represent reals, like float or double.
//floats are faster, but doubles are more accurate.
using real = float;




} //namespace Brimstone




#endif //BS_TYPES_HPP
