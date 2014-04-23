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




namespace Brimstone {

//Typedefs
typedef unsigned char       ubyte;
typedef unsigned short      ushort;
typedef unsigned int        uint;
typedef unsigned long       ulong;

#if defined( BS_BUILD_WINDOWS )

typedef __int8              int8;
typedef __int16             int16;
typedef __int32             int32;
typedef __int64             int64;
typedef unsigned __int8     uint8;
typedef unsigned __int16    uint16;
typedef unsigned __int32    uint32;
typedef unsigned __int64    uint64;

#elif defined( BS_BUILD_LINUX ) || defined( BS_BUILD_MAC )

typedef char                int8;
typedef short               int16;
typedef int                 int32;
typedef long long           int64;
typedef unsigned char       uint8;
typedef unsigned short      uint16;
typedef unsigned int        uint32;
typedef unsigned long long  uint64;

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