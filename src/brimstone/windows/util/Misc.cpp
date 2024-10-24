/*
windows/util/Misc.cpp
---------------------
Copyright (c) 2024, theJ89

Description:
    See windows/util/Misc.hpp for more information.
*/




//Includes
#include "Misc.hpp"                 //Header
#include "../WindowsHeader.hpp"     //GetModuleFileNameA, DWORD

#include <brimstone/Exception.hpp>  //Brimstone::Exception




namespace Brimstone::Private {




ustring getExecutablePath() {
    //TODO: Needs testing
    ustring path( nSize, '\0' );
    DWORD nSize = 4096;

    //Get the executable path.
    DWORD rv = GetModuleFileNameA( NULL, path.data(), nSize );
    if( rv == 0 )
        throwWindowsException();

    //GetModuleFileNameA() returns the number of characters that were copied to the buffer, or 0 if the function failed.
    //If the number of characters copied was equal to the size of the buffer, nSize, then it's possible that there wasn't enough space to contain the entire path and it was truncated.
    //We check for this case here and treat it as an error if it occurs.
    //NOTE:
    //    Even if rv == nSize, it's possible that the path wasn't truncated.
    //    For versions of Windows newer than Windows XP, we can confirm this by checking GetLastError(), which will return ERROR_INSUFFICIENT_BUFFER if the path was truncated.
    //    But for Windows XP, GetLastError() will return ERROR_SUCCESS even if the path was truncated, so it isn't safe to assume the path wasn't truncated without taking the version of
    //    Windows into account. Not that anybody is actually using Windows XP anymore, mind you.
    if( rv == nSize )
        throw Exception( "GetModuleFileNameA() failed: Not enough space in buffer to contain returned path." );

    //Shrink size of string to fit returned contents if necessary:
    path.resize( rv );

    //Return path:
    return path;
}




} // namespace Brimstone::Private
