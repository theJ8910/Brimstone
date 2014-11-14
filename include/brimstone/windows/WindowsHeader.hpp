/*
windows/WindowsHeader.hpp
-----------------------
Copyright (c) 2014, theJ89

Description:
    Include this instead of <Windows.h>.

    This file includes <Windows.h> after defining several switches that disable unnecessary code and
    instrusive macros such as min() and max().
*/
#ifndef BS_WINDOWS_WINDOWSHEADER_HPP
#define BS_WINDOWS_WINDOWSHEADER_HPP




//Includes
//Turn off a few annoying Windows anti-features before including Windows.h to make our life easier
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#define STRICT
#include <Windows.h>




#endif //BS_WINDOWS_WINDOWSHEADER_HPP