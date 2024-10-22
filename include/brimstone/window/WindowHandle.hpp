/*
window/WindowHandle.hpp
-----------------------
Copyright (c) 2024, theJ89

Description:
    Forward declares the appropriate type as WindowHandle, depending on build settings.
*/
#ifndef BS_WINDOW_WINDOWHANDLE_HPP
#define BS_WINDOW_WINDOWHANDLE_HPP




//Forward declarations
#if defined( BS_BUILD_WINDOWS )
struct HWND__;
#endif //BS_BUILD_WINDOWS




namespace Brimstone {




#if defined( BS_BUILD_WINDOWS )
using WindowHandle = HWND__*;
#elif defined( BS_BUILD_LINUX )
using WindowHandle = unsigned long;
#endif




} //namespace Brimstone




#endif //BS_WINDOW_WINDOWHANDLE_HPP
