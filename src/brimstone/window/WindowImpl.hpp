/*
window/WindowImpl.hpp
---------------------
Copyright (c) 2024, theJ89

Description:
    Includes the header for WindowImpl's implementation,
    which varies according to build settings.
*/
#ifndef BS_WINDOW_WINDOWIMPL_HPP
#define BS_WINDOW_WINDOWIMPL_HPP

//Includes
#include <brimstone/window/DWindowImpl.hpp> //Brimstone::Private::WindowImpl

#if defined( BS_BUILD_WINDOWS )
#include "../windows/WindowsWindow.hpp"    //WindowImpl (WindowsWindow)
#elif defined( BS_BUILD_LINUX )
#include "../linux/LinuxWindow.hpp"        //WindowImpl (LinuxWindow)
#endif

#endif //BS_WINDOW_WINDOWIMPL_HPP
