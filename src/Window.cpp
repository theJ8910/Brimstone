/*
Window.cpp
-----------------------
Copyright (c) 2014, theJ89

Description:
    See Window.hpp for more information.
*/




//Includes
#include <brimstone/Window.hpp>       //Class header




namespace Brimstone {

Window::Window() :
    m_title( "Default Window Title" ),
    m_popup( false ),
    m_keyRepeat( false ),
    m_bounds( 0, 0, 1024, 768 ),
    m_impl( nullptr ) {
}

Window::~Window() {
    if( m_impl != nullptr )
        delete m_impl;
}

void Window::processEvents() {
    Private::WindowImpl::processEvents();
}

void Window::setTitle( const ustring& title ) {
    m_title = title;

    if( m_impl != nullptr )
        m_impl->setTitle( title );
}

void Window::getTitle( ustring& titleOut ) const {
    titleOut = m_title;
}

void Window::setPopup( const bool popup ) {
    m_popup = popup;

    if( m_impl != nullptr )
        m_impl->setPopup( popup );
}

bool Window::getPopup() const {
    return m_popup;
}

void Window::setKeyRepeat( const bool keyRepeat ) {
    m_keyRepeat = keyRepeat;
}

bool Window::getKeyRepeat() const {
    return m_keyRepeat;
}

void Window::setVisible( const bool visible ) {
    if( visible == getVisible() )
        return;

    if( visible ) {
        if( m_impl == nullptr )
            m_impl = new Private::WindowImpl( *this );
    } else if( m_impl != nullptr ) {
        delete m_impl;
        m_impl = nullptr;
    }
}

bool Window::getVisible() const {
    //Visible is defined as "the window exists"
    return m_impl != nullptr;
}

void Window::setBounds( const LongRectangle& bounds ) {
    m_bounds = bounds;

    if( m_impl != nullptr )
        m_impl->setBounds( bounds );
}

void Window::getBounds( LongRectangle& boundsOut ) const {
    boundsOut = m_bounds;
}

}
