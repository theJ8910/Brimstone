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
    m_strTitle( "Default Window Title" ),
    m_bPopup( false ),
    m_bKeyRepeat( false ),
    m_cBounds( 0, 0, 1024, 768 ),
    m_pcImpl( nullptr ) {
}

Window::~Window() {
    if( m_pcImpl != nullptr )
        delete m_pcImpl;
}

void Window::processEvents() {
    Private::WindowImpl::processEvents();
}

void Window::setTitle( const ustring& strTitle ) {
    m_strTitle = strTitle;

    if( m_pcImpl != nullptr )
        m_pcImpl->setTitle( strTitle );
}

void Window::getTitle( ustring& strTitleOut ) const {
    strTitleOut = m_strTitle;
}

void Window::setPopup( const bool bPopup ) {
    m_bPopup = bPopup;

    if( m_pcImpl != nullptr )
        m_pcImpl->setPopup( bPopup );
}

bool Window::getPopup() const {
    return m_bPopup;
}

void Window::setKeyRepeat( const bool bKeyRepeat ) {
    m_bKeyRepeat = bKeyRepeat;
}

bool Window::getKeyRepeat() const {
    return m_bKeyRepeat;
}

void Window::setVisible( const bool bVisible ) {
    if( bVisible == getVisible() )
        return;

    if( bVisible ) {
        if( m_pcImpl == nullptr )
            m_pcImpl = new Private::WindowImpl( *this );
    } else if( m_pcImpl != nullptr ) {
        delete m_pcImpl;
        m_pcImpl = nullptr;
    }
}

bool Window::getVisible() const {
    //Visible is defined as "the window exists"
    return m_pcImpl != nullptr;
}

void Window::setBounds( const LongRectangle& cBounds ) {
    m_cBounds = cBounds;

    if( m_pcImpl != nullptr )
        m_pcImpl->setBounds( cBounds );
}

void Window::getBounds( LongRectangle& cBoundsOut ) const {
    cBoundsOut = m_cBounds;
}

}
