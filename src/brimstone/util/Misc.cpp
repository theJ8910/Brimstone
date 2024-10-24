/*
util/Misc.cpp
-------------
Copyright (c) 2024, theJ89

Description:
    See util/Misc.hpp for more information.
*/




//Includes
#include <brimstone/util/Misc.hpp>   //Header

#if defined( BS_BUILD_WINDOWS )
#include "../windows/util/Misc.hpp"  //Brimstone::Private::getExecutablePath
#elif defined( BS_BUILD_LINUX )
#include "../linux/util/Misc.hpp"    //Brimstone::Private::getExecutablePath
#endif




namespace Brimstone {




/*
getExecutablePath
-----------------

Description:
    Returns the path of this process's executable.

Arguments:
    N/A

Returns:
    ustring:  The path to this process's executable.
*/
ustring getExecutablePath() {
    return Private::getExecutablePath();
}




} // namespace Brimstone
