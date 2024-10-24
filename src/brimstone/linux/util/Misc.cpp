/*
linux/util/Misc.cpp
-------------------
Copyright (c) 2024, theJ89

Description:
    See linux/util/Misc.hpp for more information.
*/




//Includes
#include "Misc.hpp"                            //Header

#include <brimstone/Exception.hpp>             //Brimstone::Exception
#include <brimstone/linux/LinuxException.hpp>  //Brimstone::throwLinuxException

#include <cstddef>                             //std::size_t

#include <limits.h>                            //PATH_MAX
#include <unistd.h>                            //readlink
#include <sys/stat.h>                          //lstat, stat
#include <sys/types.h>                         //ssize_t, off_t




namespace {




using namespace Brimstone;
using namespace Brimstone::Private;




//Forward declarations
//ustring getLinkTarget( const char* path );
ustring getLinkTarget( const char* path, const off_t bufsiz );



/*
getLinkTarget{1}
----------------

Description:
    Returns the target of the symbolic link at the given path.
    This override calls lstat() to automatically determine how large of a buffer should be allocated for reading the link target.

Arguments:
    path:  The path to the symbolic link.

Returns:
    ustring:  The target of the symbolic link at the given path.

Throws:
    LinuxException:  If lstat() or readlink() fails.
    Exception:       If the buffer wasn't large enough to contain the symbolic link's target.
*/
/*
ustring getLinkPath( const char* path ) {
    //Determine how many bytes the buffer we allocate for readlink() needs to be.
    //We'll try to lstat() the link and get the st_size from the resulting stat struct, which should tell us how many bytes long the link's pathname is (no terminating null byte).
    struct stat statbuf;
    if( lstat( path, &statbuf ) != 0 )
        throwLinuxException();

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

/*
getLinkTarget{2}
----------------

Description:
    Returns the target of the symbolic link at the given path.
    This override takes the size of the buffer to allocate as an argument, avoiding the overhead of the call to lstat() performed by getLinkTarget{1}.
    However, by doing so you will almost always allocate more (or worse, less) space in the buffer than you need.

Arguments:
    path:    The path to the symbolic link.
    bufsiz:  The size of the buffer to allocate for reading the symbolic link's target.
             If in doubt, specify PATH_MAX.

Returns:
    ustring:  The target of the symbolic link at the given path.

Throws:
    LinuxException: If readlink() fails.
    Exception:      If the buffer wasn't large enough to contain the symbolic link's target.
*/
ustring getLinkTarget( const char* path, const off_t bufsiz ) {
    //Allocate the buffer:
    ustring target( bufsiz, '\0' );

    //Get the link's path:
    ssize_t rv = readlink( path, target.data(), bufsiz );
    if( rv == -1 ) {
        throwLinuxException();
    } else if( rv == bufsiz ) {
        throw Exception( "readlink() failed: Not enough space in buffer to contain returned path." );
    }

    //Shrink size of string to fit returned contents if necessary:
    target.resize( rv );

    //Return path:
    return target;
}




} //namespace




namespace Brimstone::Private {




/*
getExecutablePath
-----------------

Description:
    Returns the path of this process's executable.

    This is the Linux implementation of getExecutablePath().
    It works by reading the target of "/proc/self/exe", which is a special symbolic link created by the procfs filesystem whose target is the path to the executable whose process is reading
    its target.

Arguments:
    N/A

Returns:
    ustring:  The path to this process's executable.
*/
ustring getExecutablePath() {
    //NOTE:
    //    We're calling getLinkTarget{2} here because lstat() of "/proc/self/exe" always returns 0 for stat.st_size.
    //    lstat() is a wasted call, so circumvent it by directly specifying a buffer size of PATH_MAX:
    return getLinkTarget( "/proc/self/exe", PATH_MAX );
}




} // namespace Brimstone::Private
