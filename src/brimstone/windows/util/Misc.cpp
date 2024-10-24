/*
windows/util/Misc.cpp
---------------------
Copyright (c) 2024, theJ89

Description:
    See windows/util/Misc.hpp for more information.
*/




//Includes
#include "Misc.hpp"                     //Header

#include <brimstone/Exception.hpp>      //Brimstone::Exception

#include <brimstone/WindowsHeader.hpp>  //GetModuleFileName, DWORD




namespace Brimstone::Private {




ustring getExecutablePath() {
    //TODO: Needs testing
    ustring path( nSize, '\0' );
    DWORD nSize = 4096;

    //Get the executable path.
    //NOTE:
    //    For versions of Windows newer than Windows XP, GetLastError() will return ERROR_INSUFFICIENT_BUFFER if the path was truncated.
    //    But for Windows XP, GetLastError() remains 
    DWORD rv = GetModuleFileNameA( NULL, path.data(), nSize );
    if( rv == 0 )
        throwWindowsException();

    //If GetModuleFileNameA() returns nSize, it's possible the path was truncated:
    if( rv == nSize )
        throw Exception( "GetModuleFileNameA() failed: returned path was possibly truncated." );

    //Shrink size of string to fit returned contents if necessary:
    path.resize( rv );
    return path;
}




} // namespace Brimstone::Private
