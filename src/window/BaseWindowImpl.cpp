/*
window/BaseWindowImpl.cpp
-----------------------
Copyright (c) 2014, theJ89

Description:
    See BaseWindowImpl.hpp for more information.
*/

//Includes
#include "BaseWindowImpl.hpp"   //Header file




namespace Brimstone {
namespace Private {

BaseWindowImpl::BaseWindowImpl() :
    m_title( "Default Window Title" ),
    m_visible( true ),
    m_resizable( false ),
    m_mouseCapture( false ),
    m_popup( false ),
    m_keyRepeat( false ),
    m_bounds( Point2i( 32, 32 ), Size2i( 1024, 768 ) )
{
}

void BaseWindowImpl::pushEvent( const WindowEvent& eventIn ) {
    m_eventQueue.push( eventIn );
}

void BaseWindowImpl::setTitle( const ustring& title ) {
    m_title = title;
}

ustring BaseWindowImpl::getTitle() const {
    return m_title;
}

void BaseWindowImpl::setPopup( const bool popup ) {
    m_popup = popup;
}

bool BaseWindowImpl::isPopup() const {
    return m_popup;
}

void BaseWindowImpl::setResizable( const bool resizable ) {
    m_resizable = resizable;
}

bool BaseWindowImpl::isResizable() const {
    return m_resizable;
}

void BaseWindowImpl::setVisible( const bool visible ) {
    m_visible = visible;
}

bool BaseWindowImpl::isVisible() const {
    return m_visible;
}

void BaseWindowImpl::setBounds( const Bounds2i bounds ) {
    m_bounds = bounds;
}

Bounds2i BaseWindowImpl::getBounds() const {
    return m_bounds;
}

void BaseWindowImpl::setKeyRepeat( const bool keyRepeat ) {
    m_keyRepeat = keyRepeat;
}

bool BaseWindowImpl::getKeyRepeat() const {
    return m_keyRepeat;
}

void BaseWindowImpl::setMouseCapture( const bool capture ) {
    m_mouseCapture = capture;
}

bool BaseWindowImpl::getMouseCapture() const {
    return m_mouseCapture;
}

}
}