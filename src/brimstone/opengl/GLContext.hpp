/*
opengl/GLContext.hpp
-----------------------
Copyright (c) 2014, theJ89

Description:
    Includes the header for GLContext's implementation,
    which varies according to build settings.

    GLContext is defined as an alias of the chosen implementation.
*/
#ifndef BS_OPENGL_GLCONTEXT_HPP
#define BS_OPENGL_GLCONTEXT_HPP




//Includes
#if defined( BS_BUILD_WINDOWS )
#include "WindowsGLContext.hpp"
#elif defined( BS_BUILD_LINUX )
#include "LinuxGLContext.hpp"
#endif




//Typedefs
namespace Brimstone {
namespace Private {

#if defined( BS_BUILD_WINDOWS )
typedef class WindowsGLContext  GLContext;
#elif defined( BS_BUILD_LINUX )
typedef class LinuxGLContext    GLContext;
#endif

}
}




#endif //BS_OPENGL_GLCONTEXT_HPP