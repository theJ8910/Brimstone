/*
opengl/GLContext.hpp
--------------------
Copyright (c) 2024, theJ89

Description:
    Includes the header for GLContext's implementation,
    which varies according to build settings.

    GLContext is defined as an alias of the chosen implementation.
*/
#ifndef BS_OPENGL_GLCONTEXT_HPP
#define BS_OPENGL_GLCONTEXT_HPP




//Includes
#if defined( BS_BUILD_WINDOWS )
#include "../windows/opengl/WindowsGLContext.hpp"
#elif defined( BS_BUILD_LINUX )
#include "../linux/opengl/LinuxGLContext.hpp"
#endif




namespace Brimstone::Private {




//Types
#if defined( BS_BUILD_WINDOWS )
using GLContext = class WindowsGLContext;
#elif defined( BS_BUILD_LINUX )
using GLContext = class LinuxGLContext;
#endif




} //namespace Brimstone::Private




#endif //BS_OPENGL_GLCONTEXT_HPP
