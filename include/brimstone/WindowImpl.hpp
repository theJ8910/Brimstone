/*
WindowImpl.hpp
-----------------------
Copyright (c) 2014, theJ89

Description:
    Forward declares either WindowsWindow or LinuxWindow as WindowImpl, depending on build settings.
*/
#ifndef BS_WINDOWIMPL_HPP
#define BS_WINDOWIMPL_HPP

namespace Brimstone {
namespace Private {
#if defined( BS_BUILD_WINDOWS )
typedef class WindowsWindow WindowImpl;
#elif defined( BS_BUILD_LINUX )
typedef class LinuxWindow   WindowImpl;
#endif
}
}

#endif //BS_WINDOWIMPL_HPP