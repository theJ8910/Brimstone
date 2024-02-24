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
typedef struct _XDisplay Display;
#endif //BS_BUILD_LINUX

namespace Brimstone {
#if defined( BS_BUILD_WINDOWS )
typedef nullptr_t     WindowDisplay;
#elif defined( BS_BUILD_LINUX )
typedef Display* WindowDisplay;
#endif
}

#endif //BS_WINDOW_WINDOWDISPLAY_HPP
