/*
opengl/LinuxGLContext.cpp
-----------------------
Copyright (c) 2014, theJ89

Description:
    See LinuxGLContext.hpp for more information.
*/




//Includes
#include "LinuxGLContext.hpp"
#include "../linux/LinuxWindow.hpp"     //LinuxWindow




namespace Brimstone {
namespace Private {

LinuxGLContext::LinuxGLContext() :
    m_context( 0 ),
    m_display( 0 ),
    m_window( 0 ) {
}

LinuxGLContext::LinuxGLContext( const Window& window ) {
}

LinuxGLContext::~LinuxGLContext() {
}

void LinuxGLContext::init() {
}

void LinuxGLContext::init( const Window& window ) {
}

void LinuxGLContext::destroy() {
}

void LinuxGLContext::setVSync( const bool vsync ) {
}

void LinuxGLContext::swapBuffers() {
    glXSwapBuffers( m_display, m_window );
}

void LinuxGLContext::begin() {
    glXMakeCurrent( m_display, m_window, m_context );
}

void LinuxGLContext::end() {
    glXMakeCurrent( m_display, 0, 0 );
}

}
}