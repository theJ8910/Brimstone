/*
Window.cpp
-----------------------
Copyright (c) 2014, theJ89

Description:
    See Window.hpp for more information.
*/




//Includes
#include <brimstone/Window.hpp>         //Class header
#include "window/WindowImpl.hpp"        //Brimstone::Private::WindowImpl




namespace Brimstone {

Window::Window() :
    m_impl( nullptr ) {

    m_impl = new Private::WindowImpl();
}

Window::~Window() {
    if( m_impl != nullptr ) {
        delete m_impl;
        m_impl = nullptr;
    }
}

Window::Window( Window&& toMove ) :
    m_impl( toMove.m_impl ) {

    toMove.m_impl = nullptr;
}
Window& Window::operator =( Window&& toMove ) {
    m_impl        = toMove.m_impl;
    toMove.m_impl = nullptr;

    return *this;
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

bool Window::peekEvent( WindowEvent& eventOut ) {
    return m_impl->peekEvent( eventOut );
}

bool Window::getEvent( WindowEvent& eventOut ) {
    return m_impl->getEvent( eventOut );
}

void Window::pushEvent( const WindowEvent& eventIn ) {
    m_impl->pushEvent( eventIn );
}

void Window::setTitle( const ustring& title ) {
    m_impl->setTitle( title );
}

ustring Window::getTitle() const {
    return m_impl->getTitle();
}

void Window::setPopup( const bool popup ) {
    m_impl->setPopup( popup );
}

bool Window::isPopup() const {
    return m_impl->isPopup();
}

void Window::setResizable( const bool resizable ) {
    m_impl->setResizable( resizable );
}

bool Window::isResizable() const {
    return m_impl->isResizable();
}

void Window::setVisible( const bool visible ) {
    m_impl->setVisible( visible );
}

bool Window::isVisible() const {
    return m_impl->isVisible();
}

void Window::setBounds( const Bounds2i bounds ) {
    m_impl->setBounds( bounds );
}

Bounds2i Window::getBounds() const {
    return m_impl->getBounds();
}

void Window::setKeyRepeat( const bool keyRepeat ) {
    m_impl->setKeyRepeat( keyRepeat );
}

bool Window::getKeyRepeat() const {
    return m_impl->getKeyRepeat();
}

void Window::setMouseCapture( const bool capture ) {
    m_impl->setMouseCapture( capture );
}

bool Window::getMouseCapture() const {
    return m_impl->getMouseCapture();
}

Point2i Window::screenToWindow( Point2i screenCoords ) const {
    return m_impl->screenToWindow( screenCoords );
}

Point2i Window::windowToScreen( Point2i windowCoords ) const {
    return m_impl->windowToScreen( windowCoords );
}

WindowHandle Window::getHandle() const {
    return m_impl->getHandle();
}

}
