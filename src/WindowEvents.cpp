/*
WindowEvents.cpp
-----------------------
Copyright (c) 2014, theJ89

Description:
    See WindowEvents.hpp for more information.
*/

//Includes
#include <brimstone/WindowEvents.hpp>

namespace Brimstone {

MousePressEvent::MousePressEvent( const MouseButton eButton ) : m_eButton( eButton ) {
}

MouseButton MousePressEvent::getButton() const {
    return m_eButton;
}

MouseReleaseEvent::MouseReleaseEvent( const MouseButton eButton ) : m_eButton( eButton ) {
}

MouseButton MouseReleaseEvent::getButton() const {
    return m_eButton;
}

MouseMoveEvent::MouseMoveEvent( const int32 iX, const int32 iY ) : m_iX( iX ), m_iY( iY ) {
}

int32 MouseMoveEvent::getX() const {
    return m_iX;
}

int32 MouseMoveEvent::getY() const {
    return m_iY;
}

KeyPressEvent::KeyPressEvent( const uchar pszKey[5] ) {
    for( int i = 0; i < 5; ++i )
        m_pszKey[i] = pszKey[i];
}

const uchar* KeyPressEvent::getKey() const {
    return m_pszKey;
}

}