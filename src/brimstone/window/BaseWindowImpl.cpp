/*
window/BaseWindowImpl.cpp
-------------------------
Copyright (c) 2024, theJ89

Description:
    See BaseWindowImpl.hpp for more information.
*/

//Includes
#include "BaseWindowImpl.hpp"  //Header




namespace Brimstone::Private {




BaseWindowImpl::BaseWindowImpl() :
    m_title( "Default Window Title" ),
    m_cursorPos( -1, -1 ),
    m_bounds( Point2i( 32, 32 ), Size2i( 1024, 768 ) ),
    m_visible( true ),
    m_borderless( false ),
    m_resizable( true ),
    m_fullscreen( false ),
    m_maximized( false ),
    m_minimized( false ),
    m_focused( false ),
    m_keyRepeat( true ),
    m_mouseCapture( false ),
    m_cursorTrapped( false ),
    m_cursorVisible( true ),
    m_keepCursorCentered( false )
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

void BaseWindowImpl::setBorderless( const bool borderless ) {
    m_borderless = borderless;
}

bool BaseWindowImpl::isBorderless() const {
    return m_borderless;
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

Point2i BaseWindowImpl::getCursorPos() const {
    return m_cursorPos;
}

void BaseWindowImpl::setBounds( const Bounds2i bounds ) {
    m_bounds = bounds;
}

Bounds2i BaseWindowImpl::getBounds() const {
    return m_bounds;
}

void BaseWindowImpl::setFullscreen( const bool fullscreen ) {
    m_fullscreen = fullscreen;
}

bool BaseWindowImpl::isFullscreen() const {
    return m_fullscreen;
}

void BaseWindowImpl::setMaximized( const bool maximized ) {
    m_maximized = maximized;
}

bool BaseWindowImpl::isMaximized() const {
    return m_maximized;
}

void BaseWindowImpl::setMinimized( const bool minimized ) {
    m_minimized = minimized;
}

bool BaseWindowImpl::isMinimized() const {
    return m_minimized;
}

void BaseWindowImpl::setShaded( const bool /*shaded*/ ) {
    throw NotImplementedException();
}

bool BaseWindowImpl::isShaded() const {
    throw NotImplementedException();
}

void BaseWindowImpl::restore() {
    throw NotImplementedException();
}

bool BaseWindowImpl::isRestored() const {
    throw NotImplementedException();
}

void BaseWindowImpl::focus() {
    throw NotImplementedException();
}

bool BaseWindowImpl::isFocused() const {
    return m_focused;
}

void BaseWindowImpl::setKeyRepeat( const bool keyRepeat ) {
    m_keyRepeat = keyRepeat;
}

bool BaseWindowImpl::getKeyRepeat() const {
    return m_keyRepeat;
}

void BaseWindowImpl::setMouseCapture( const bool mouseCapture ) {
    m_mouseCapture = mouseCapture;
}

bool BaseWindowImpl::getMouseCapture() const {
    return m_mouseCapture;
}

void BaseWindowImpl::setCursorTrapped( const bool cursorTrapped ) {
    m_cursorTrapped = cursorTrapped;
}

bool BaseWindowImpl::isCursorTrapped() const {
    return m_cursorTrapped;
}

void BaseWindowImpl::setCursorVisible( const bool cursorVisible ) {
    m_cursorVisible = cursorVisible;
}

bool BaseWindowImpl::isCursorVisible() const {
    return m_cursorVisible;
}

void BaseWindowImpl::setKeepCursorCentered( const bool keepCursorCentered ) {
    m_keepCursorCentered = keepCursorCentered;
}

bool BaseWindowImpl::getKeepCursorCentered() const {
    return m_keepCursorCentered;
}




} //namespace Brimstone::Private
