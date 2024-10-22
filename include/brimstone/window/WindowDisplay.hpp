/*
window/WindowDisplay.hpp
------------------------
Copyright (c) 2024, theJ89

Description:
    Forward declares the appropriate type as WindowDisplay, depending on build settings.
*/
#ifndef BS_WINDOW_WINDOWDISPLAY_HPP
#define BS_WINDOW_WINDOWDISPLAY_HPP




//Forward declarations
#if defined( BS_BUILD_LINUX )
using Display = struct _XDisplay;
#endif //BS_BUILD_LINUX




namespace Brimstone {




#if defined( BS_BUILD_WINDOWS )
using WindowDisplay = nullptr_t;
#elif defined( BS_BUILD_LINUX )
using WindowDisplay = Display*;
#endif




} //namespace Brimstone




#endif //BS_WINDOW_WINDOWDISPLAY_HPP
