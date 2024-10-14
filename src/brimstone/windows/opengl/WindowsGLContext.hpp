/*
windows/opengl/WindowsGLContext.hpp
-----------------------------------
Copyright (c) 2024, theJ89

Description:
    Windows implementation of the OpenGL Context.
*/
#ifndef BS_WINDOWS_OPENGL_WINDOWSGLCONTEXT_HPP
#define BS_WINDOWS_OPENGL_WINDOWSGLCONTEXT_HPP




//Include
#include "GLHeader.hpp"                     //HWND, HDC, HGLRC


namespace Brimstone {
class Window;
namespace Private {

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
    void init( HWND hwnd );
    void destroyContext();
    void destroyDC();
    void destroyWindow();
private:
    bool       m_hiddenWindow;
    HWND       m_hwnd;
    HDC        m_hdc;
    HGLRC      m_context;
};

}
}




#endif //BS_WINDOWS_OPENGL_WINDOWSGLCONTEXT_HPP
