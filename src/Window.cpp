/*
Window.cpp
-----------------------
Copyright (c) 2014, theJ89

Description:
    See Window.hpp for more information.
*/




//Includes
#include <brimstone/Window.hpp>         //Class header

#if defined( BS_BUILD_WINDOWS )
#include "windows/WindowsWindow.hpp"    //WindowImpl (WindowsWindow)
#elif defined( BS_BUILD_LINUX )
#include "linux/LinuxWindow.hpp"        //WindowImpl (LinuxWindow)
#endif



namespace Brimstone {

Window::Window() :
    m_title( "Default Window Title" ),
    m_visible( true ),
    m_resizable( false ),
    m_autoClose( true ),
    m_mouseCapture( false ),
    m_popup( false ),
    m_keyRepeat( false ),
    m_bounds( Point2i( 32, 32 ), 1024, 768 ),
    m_impl( nullptr ) {
    m_impl = new Private::WindowImpl( *this );
}

Window::~Window() {
    if( m_impl != nullptr ) {
        delete m_impl;
        m_impl = nullptr;
    }
}

void Window::processEvents() {
    Private::WindowImpl::processEvents();
}

void Window::open() {
    m_impl->open();
}

void Window::close() {
    m_impl->close();
}

bool Window::isOpen() const {
    return m_impl->isOpen();
}

void Window::setTitle( const ustring& title ) {
    m_title = title;
    m_impl->setTitle( title );
}

ustring Window::getTitle() const {
    return m_title;
}

void Window::setPopup( const bool popup ) {
    m_popup = popup;
    m_impl->setPopup( popup );
}

bool Window::isPopup() const {
    return m_popup;
}

void Window::setResizable( const bool resizable ) {
    m_resizable = resizable;
    m_impl->setResizable( resizable );
}

bool Window::isResizable() const {
    return m_resizable;
}

void Window::setKeyRepeat( const bool keyRepeat ) {
    m_keyRepeat = keyRepeat;
}

bool Window::getKeyRepeat() const {
    return m_keyRepeat;
}

void Window::setVisible( const bool visible ) {
    m_visible = visible;
    m_impl->setVisible( visible );
}

bool Window::isVisible() const {
    return m_visible;
}

void Window::setBounds( const Bounds2i& bounds ) {
    m_bounds = bounds;
    m_impl->setBounds( bounds );
}

Bounds2i Window::getBounds() const {
    return m_bounds;
}

void Window::setAutoClose( const bool autoClose ) {
    m_autoClose = autoClose;
}

bool Window::getAutoClose() const {
    return m_autoClose;
}

void Window::setMouseCapture( const bool capture ) {
    m_mouseCapture = capture;
    m_impl->setMouseCapture( capture );
}

bool Window::getMouseCapture() const {
    return m_mouseCapture;
}

Point2i Window::screenToWindow( const Point2i& screenCoords ) const {
    return m_impl->screenToWindow( screenCoords );
}

Point2i Window::windowToScreen( const Point2i& windowCoords ) const {
    return m_impl->windowToScreen( windowCoords );
}

}
