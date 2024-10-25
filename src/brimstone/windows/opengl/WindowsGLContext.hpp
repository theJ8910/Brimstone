/*
windows/opengl/WindowsGLContext.hpp
-----------------------------------
Copyright (c) 2024, theJ89

Description:
    Windows implementation of the OpenGL Context.
*/
#ifndef BS_WINDOWS_OPENGL_WINDOWSGLCONTEXT_HPP
#define BS_WINDOWS_OPENGL_WINDOWSGLCONTEXT_HPP




//Includes
#include "../WindowsHeader.hpp"                    //HWND, HDC

#include <gl/GL.h>                                 //HGLRC

//TODO:
//    This header no longer exists.
//    As soon as I have access to a Windows build environment I'll need to test to make sure the new OpenGL imports are working as intended.
//#include "GLHeader.hpp"  //HWND, HDC, HGLRC




namespace Brimstone {




class Window;




} //namespace Brimstone




namespace Brimstone::Private {




class WindowsGLContext {
public:
    WindowsGLContext();
    WindowsGLContext( WindowsGLContext& toCopy ) = delete;
    WindowsGLContext& operator =( WindowsGLContext& toCopy ) = delete;
    WindowsGLContext( const Window& window );
    ~WindowsGLContext();

    void init();
    void init( const Window& window );
    void destroy();

    void begin();
    void end();

    void setVSync( const bool enabled );
    bool getVSync() const;
    
    void swapBuffers();
private:
    void init( const HWND hwnd );
    void destroyContext();
    void destroyDC();
    void destroyWindow();
private:
    bool       m_hiddenWindow;
    HWND       m_hwnd;
    HDC        m_hdc;
    HGLRC      m_context;
};




} //namespace Brimstone::Private




#endif //BS_WINDOWS_OPENGL_WINDOWSGLCONTEXT_HPP
