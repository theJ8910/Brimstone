/*
opengl/LinuxGLContext.hpp
-----------------------
Copyright (c) 2014, theJ89

Description:
    Linux implementation of the OpenGL context.
*/
#ifndef BS_OPENGL_LINUXGLCONTEXT_HPP
#define BS_OPENGL_LINUXGLCONTEXT_HPP




//Include
#include <brimstone/util/NonCopyable.hpp>   //Brimstone::NonCopyable
#include "GLHeader.hpp"                     //OpenGL

#include <X11/Xlib.h>                       //X11
#include <X11/Xutil.h>                      //X11
#include <X11/Xos.h>                        //X11




namespace Brimstone {
class Window;
namespace Private {

class LinuxGLContext : public NonCopyable {
public:
    LinuxGLContext();
    LinuxGLContext( const Window& window );
    ~LinuxGLContext();

    void init();
    void init( const Window& window );
    void destroy();

    void setVSync( const bool vsync );
    void swapBuffers();

    void begin();
    void end();
private:
    GLXContext m_context;
    Display    m_display;
    ::Window   m_window;
};

}
}




#endif //BS_OPENGL_LINUXGLCONTEXT_HPP