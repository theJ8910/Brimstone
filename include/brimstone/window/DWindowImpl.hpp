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

namespace Brimstone {
namespace Private {

#if defined( BS_BUILD_WINDOWS )
typedef class WindowsWindow WindowImpl;
#elif defined( BS_BUILD_LINUX )
typedef class XWindow       WindowImpl;
#endif

}
}

#endif //BS_WINDOW_DWINDOWIMPL_HPP
