/*
Window.cpp
----------
Copyright (c) 2024, theJ89

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

void Window::frame() {
    return m_impl->frame();
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

void Window::setBorderless( const bool borderless ) {
    m_impl->setBorderless( borderless );
}

bool Window::isBorderless() const {
    return m_impl->isBorderless();
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

void Window::setFullscreen( const bool fullscreen ) {
    m_impl->setFullscreen( fullscreen );
}

bool Window::isFullscreen() const {
    return m_impl->isFullscreen();
}

void Window::setMaximized( const bool maximized ) {
    m_impl->setMaximized( maximized );
}

bool Window::isMaximized() const {
    return m_impl->isMaximized();
}

void Window::setMinimized( const bool minimized ) {
    m_impl->setMinimized( minimized );
}

bool Window::isMinimized() const {
    return m_impl->isMinimized();
}

void Window::setShaded( const bool shaded ) {
    m_impl->setShaded( shaded );
}

bool Window::isShaded() const {
    return m_impl->isShaded();
}

void Window::restore() {
    m_impl->restore();
}

bool Window::isRestored() const {
    return m_impl->isRestored();
}

void Window::focus() {
    return m_impl->focus();
}

bool Window::isFocused() const {
    return m_impl->isFocused();
}

void Window::setKeyRepeat( const bool keyRepeat ) {
    m_impl->setKeyRepeat( keyRepeat );
}

bool Window::getKeyRepeat() const {
    return m_impl->getKeyRepeat();
}

void Window::setMouseCapture( const bool mouseCapture ) {
    m_impl->setMouseCapture( mouseCapture );
}

bool Window::getMouseCapture() const {
    return m_impl->getMouseCapture();
}

void Window::setCursorTrapped( const bool cursorTrapped ) {
    m_impl->setCursorTrapped( cursorTrapped );
}

bool Window::isCursorTrapped() const {
    return m_impl->isCursorTrapped();
}

void Window::setCursorVisible( const bool cursorVisible ) {
    m_impl->setCursorVisible( cursorVisible );
}

bool Window::isCursorVisible() const {
    return m_impl->isCursorVisible();
}

void Window::setKeepCursorCentered( const bool keepCursorCentered ) {
    m_impl->setKeepCursorCentered( keepCursorCentered );
}

bool Window::getKeepCursorCentered() const {
    return m_impl->getKeepCursorCentered();
}

void Window::sendToTop() {
    return m_impl->sendToTop();
}

void Window::sendToBottom() {
    return m_impl->sendToBottom();
}

Point2i Window::screenToWindow( Point2i screenCoords ) const {
    return m_impl->screenToWindow( screenCoords );
}

Point2i Window::windowToScreen( Point2i windowCoords ) const {
    return m_impl->windowToScreen( windowCoords );
}

WindowDisplay Window::getDisplay() const {
    return m_impl->getDisplay();
}

WindowHandle Window::getHandle() const {
    return m_impl->getHandle();
}

}
