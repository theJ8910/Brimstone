/*
WindowEvents.cpp
-----------------------
Copyright (c) 2014, theJ89

Description:
    See WindowEvents.hpp for more information.
*/

//Includes
#include <brimstone/WindowEvents.hpp>   //Class header

namespace Brimstone {

MouseDownEvent::MouseDownEvent( const MouseButton eButton ) : m_eButton( eButton ) {
}

MouseButton MouseDownEvent::getButton() const {
    return m_eButton;
}

MouseUpEvent::MouseUpEvent( const MouseButton eButton ) : m_eButton( eButton ) {
}

MouseButton MouseUpEvent::getButton() const {
    return m_eButton;
}

MouseMoveEvent::MouseMoveEvent( const int32 iX, const int32 iY ) : m_iX( iX ), m_iY( iY ) {
}

KeyDownEvent::KeyDownEvent( const Key eKey ) : m_eKey( eKey ) {
}

Key KeyDownEvent::getKey() const {
    return m_eKey;
}

KeyUpEvent::KeyUpEvent( const Key eKey ) : m_eKey( eKey ) {
}

Key KeyUpEvent::getKey() const {
    return m_eKey;
}

int32 MouseMoveEvent::getX() const {
    return m_iX;
}

int32 MouseMoveEvent::getY() const {
    return m_iY;
}

const uchar* CharacterTypedEvent::getCharacter() const {
    return m_pszKey;
}

}