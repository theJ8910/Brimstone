/*
window/DWindowImpl.hpp
----------------------
Copyright (c) 2024, theJ89

Description:
    Forward declares WindowImpl's implementation,
    which varies according to build settings.

    WindowImpl is defined as an alias of the chosen implementation.
*/
#ifndef BS_WINDOW_DWINDOWIMPL_HPP
#define BS_WINDOW_DWINDOWIMPL_HPP




namespace Brimstone::Private {




//Types
#if defined( BS_BUILD_WINDOWS )
using WindowImpl = class WindowsWindow;
#elif defined( BS_BUILD_LINUX )
using WindowImpl = class XWindow;
#endif




} //namespace Brimstone::Private




#endif //BS_WINDOW_DWINDOWIMPL_HPP
