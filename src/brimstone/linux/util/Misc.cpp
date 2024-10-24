/*
linux/util/Misc.cpp
-------------------
Copyright (c) 2024, theJ89

Description:
    See linux/util/Misc.hpp for more information.
*/




//Includes
#include "Misc.hpp"                 //Header

#include <brimstone/Exception.hpp>  //Brimstone::Exception

#include <cstddef>                  //std::size_t

#include <limits.h>                 //PATH_MAX
#include <unistd.h>                 //readlink
#include <sys/stat.h>               //lstat, stat
#include <sys/types.h>              //ssize_t, off_t




namespace {




using namespace Brimstone;

//Forward declarations
ustring getLinkPath( const char* path, const off_t bufsiz );



/*
ustring getLinkPath( const char* path ) {
    //Determine how many bytes the buffer we allocate for readlink() needs to be.
    //We'll try to lstat() the link and get the st_size from the resulting stat struct, which should tell us how many bytes long the link's pathname is (no terminating null byte).
    struct stat statbuf;
    if( lstat( path, &statbuf ) != 0 ) {
        //TODO: better errno handling
        throw Exception( "lstat() failed." );
    }

    //st_size may be 0 in some cases (e.g. for magic symlinks like those found beneath /proc/), in which case we'll allocate a buffer of size PATH_MAX instead.
    //NOTE:
    //    On my system, PATH_MAX is 4096, which is long enough to store the vast majority of paths, though it's worth noting that paths can be potentially longer than this.
    //NOTE:
    //    readlink() can potentially return a longer path than what we were expecting based on the data we got from lstat(). This can happen, for instance, because of a race condition where
    //    the link target is changed to a longer path between the call to lstat() and readlink(). We can detect this by adding 1 to the buffer size, then checking if the number of bytes that
    //    readlink returned (rv) is equal to the buffer size (bufsiz); if this is the case, then it means than rv > statbuf.st_size and that the returned path was possibly truncated.
    off_t bufsiz = ( statbuf.st_size != 0 ? statbuf.st_size + 1 : PATH_MAX );

    return getLinkPath( path, bufsiz );
}
*/

ustring getLinkPath( const char* path, const off_t bufsiz ) {
    //Allocate the buffer:
    ustring target( bufsiz, '\0' );

    //Get the link's path:
    ssize_t rv = readlink( path, target.data(), bufsiz );
    if( rv == -1 ) {
        //TODO: better errno handling
        throw Exception( "readlink() failed." );
    } else if( rv == bufsiz ) {
        throw Exception( "readlink() failed: returned path was possibly truncated." );
    }

    //Shrink size of string to fit returned contents if necessary:
    target.resize( rv );

    return target;
}




} //namespace




namespace Brimstone::Private {




ustring getExecutablePath() {
    //NOTE:
    //    Calling the override of getLinkPath() that takes maxPathSize because lstat() of "/proc/self/exe" always returns 0 for stat.st_size.
    //    lstat() is a wasted call, so circumvent it:
    return getLinkPath( "/proc/self/exe", PATH_MAX );
}




} // namespace Brimstone::Private
