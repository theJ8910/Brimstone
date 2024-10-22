/*
opengl/WindowsGLContext.cpp
---------------------------
Copyright (c) 2024, theJ89

Description:
    See WindowsGLContext.hpp for more information.
*/




//Includes
#include "WindowsGLContext.hpp"                     //Header
#include "GLHeader.hpp"                             //OpenGL
#include "GLLoad.hpp"                               //wglSwapIntervalEXT

#include <brimstone/Window.hpp>                     //Brimstone::Window
#include <brimstone/windows/WindowsException.hpp>   //Brimstone::Private::throwWindowsException




namespace Brimstone::Private {




WindowsGLContext::WindowsGLContext() :
    m_hiddenWindow( false ),
    m_hwnd( nullptr ),
    m_hdc( nullptr ),
    m_context( nullptr ) {
}

WindowsGLContext::WindowsGLContext( const Window& window ) :
    WindowsGLContext() {
    init( window );
}

WindowsGLContext::~WindowsGLContext() {
    try                           { destroyContext();        }
    catch( const IException& ex ) { uncaughtException( ex ); }

    try                           { destroyDC();             }
    catch( const IException& ex ) { uncaughtException( ex ); }

    try                           { destroyWindow();         }
    catch( const IException& ex ) { uncaughtException( ex ); }
}

//Initializes the context with a hidden window
void WindowsGLContext::init() {
    if( m_context != nullptr )
        return;

    //Create a hidden window
    HWND hwnd = CreateWindow(
        TEXT( "STATIC" ),
        TEXT( "" ),
        WS_POPUP | WS_DISABLED,
        0, 0, 1, 1,
        nullptr,
        nullptr,
        GetModuleHandle( nullptr ),
        nullptr
    );

    if( hwnd == nullptr )
        throw GraphicsException( "Couldn't create temporary window." );

    m_hiddenWindow = true;
    init( hwnd );
}

//Initializes the context using an existing window
void WindowsGLContext::init( const Window& window ) {
    if( m_context != nullptr )
        return;

    m_hiddenWindow = false;
    init( window.getHandle() );
}

void WindowsGLContext::init( HWND hwnd ) {
    m_hwnd = hwnd;
    m_hdc  = GetDC( m_hwnd );
    if( m_hdc == nullptr )
        throwWindowsException();

    //Request that the device context use a pixel format meeting our specifications
    PIXELFORMATDESCRIPTOR pfd = {
        sizeof( PIXELFORMATDESCRIPTOR ),
        1,
        PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
        PFD_TYPE_RGBA,
        32,
        0, 0, 0, 0, 0, 0,
        0, 0, 0,
        0, 0, 0, 0,
        24,
        8,
        0,
        PFD_MAIN_PLANE,
        0,
        0, 0, 0
    };

    int pf = ChoosePixelFormat( m_hdc, &pfd );
    if( pf == 0 )
        throw GraphicsException( "Invalid pixel format." );

    if( !SetPixelFormat( m_hdc, pf, &pfd ) )
        throw GraphicsException( "Failed to set pixel format." );

    //Create the context
    m_context = wglCreateContext( m_hdc );
    if( m_context == nullptr )
        throwWindowsException();
}

//Set this context as the current context for this thread
void WindowsGLContext::begin() {
    if( wglMakeCurrent( m_hdc, m_context ) == FALSE )
        throwWindowsException();
}

//Clear the active context for this thread
void WindowsGLContext::end() {
    if( wglMakeCurrent( nullptr, nullptr ) == FALSE )
        throwWindowsException();
}

void WindowsGLContext::destroy() {
    destroyContext();
    destroyDC();
    destroyWindow();
}

void WindowsGLContext::destroyContext() {
    if( m_context != nullptr ) {
        if( wglGetCurrentContext() == m_context )
            wglMakeCurrent( nullptr, nullptr );
        if( wglDeleteContext( m_context ) == FALSE )
            throwWindowsException();
        m_context = nullptr;
    }
}

void WindowsGLContext::destroyDC() {
    if( m_hdc != nullptr ) {
        if( ReleaseDC( m_hwnd, m_hdc ) == FALSE )
            throw GraphicsException();
        m_hdc = nullptr;
    }
}

void WindowsGLContext::destroyWindow() {
    if( m_hiddenWindow && m_hwnd != nullptr ) {
        if( DestroyWindow( m_hwnd ) == FALSE )
            throwWindowsException();
        m_hiddenWindow = false;
        m_hwnd      = nullptr;
    }
}

void WindowsGLContext::setVSync( const bool enabled ) {
    if( wglSwapIntervalEXT( enabled ? 1 : 0 ) == FALSE )
        throwWindowsException();
}

bool WindowsGLContext::getVSync() const {
    return wglGetSwapIntervalEXT() == 1;
}

void WindowsGLContext::swapBuffers() {
    if( SwapBuffers( m_hdc ) == FALSE )
        throwWindowsException();
}




} //namespace Brimstone::Private
