/*
linux/opengl/LinuxGLContext.hpp
-------------------------------
Copyright (c) 2024, theJ89

Description:
    Linux implementation of the OpenGL context.
*/
#ifndef BS_LINUX_OPENGL_LINUXGLCONTEXT_HPP
#define BS_LINUX_OPENGL_LINUXGLCONTEXT_HPP




//Includes
#include "../x11/XVisualInfo.hpp"  //Brimstone::Private::XVisualInfo

#include <X11/Xlib.h>              //X11
#include <X11/Xutil.h>             //X11
#include <X11/Xos.h>               //X11




//TEMP: gll doesn't have a glx_types header yet
using GLXContext  = struct __GLXcontextRec*;
using GLXFBConfig = struct __GLXFBConfigRec*;




namespace Brimstone {




//Forward declarations
class Window;




} //namespace Brimstone




namespace Brimstone::Private {




class LinuxGLContext {
public:
    LinuxGLContext();
    LinuxGLContext( const LinuxGLContext& toCopy ) = delete;
    LinuxGLContext& operator =( const LinuxGLContext& toCopy ) = delete;
    LinuxGLContext( const Window& window );
    ~LinuxGLContext();

    void init();
    void init( const Window& window );
    void destroy();

    void begin();
    void end();

    void setVSync( const bool enabled );
    bool getVSync() const;

    void swapBuffers();    
private:
    void destroyContext();
    void destroyFinish();
private:
    GLXContext m_context;
    ::Window   m_window;
public:
    static XVisualInfo getIdealVisualInfo( Display* display );
private:
    static void initGLX( Display* display );
    static void destroyGLX();
private:
    static bool        m_glxInitialized;
    static Display*    m_display;
    static GLXFBConfig m_bestFBC;

    static int         m_contextCount;
};




} //namespace Brimstone::Private




#endif //BS_LINUX_OPENGL_LINUXGLCONTEXT_HPP
