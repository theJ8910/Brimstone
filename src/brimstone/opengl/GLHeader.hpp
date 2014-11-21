/*
opengl/GLHeader.hpp
-----------------------
Copyright (c) 2014, theJ89

Description:
    Include this instead of gl/GL.h.

    This file includes Windows.h if we are on Windows
    as well as gl/GL.h.
*/
#ifndef BS_OPENGL_GLHEADER_HPP
#define BS_OPENGL_GLHEADER_HPP




//Includes
//On Windows, gl/GL.h uses Windows defines, but doesn't include Windows.h itself.
//This means we need to do it ourselves before including gl/GL.h.
#ifdef BS_BUILD_WINDOWS
#include <brimstone/windows/WindowsHeader.hpp>
#endif //BS_BUILD_WINDOWS

#include <gl/GL.h>




#endif //BS_OPENGL_GLHEADER_HPP