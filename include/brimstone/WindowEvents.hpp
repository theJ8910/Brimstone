/*
WindowEvents.hpp
-----------------------
Copyright (c) 2014, theJ89

Description:
    Defines several data structures containing information about events
    generated from messages received by a Window.

    The following classes are defined here:
        * MousePressEvent
        * MouseReleaseEvent
        * MouseMoveEvent
        * KeyPressEvent
        * KeyReleaseEvent
*/

#ifndef BS_WINDOWEVENTS_HPP
#define BS_WINDOWEVENTS_HPP




//Includes
#include "types.hpp"    //int32, uchar




namespace Brimstone {

enum class MouseButton {
    LEFT,
    RIGHT,
    MIDDLE
};

class MousePressEvent {
public:
    MousePressEvent( const MouseButton m_eButton );
    MouseButton getButton() const;
private:
    MouseButton m_eButton;
};

class MouseReleaseEvent {
public:
    MouseReleaseEvent( const MouseButton m_eButton );
    MouseButton getButton() const;
private:
    MouseButton m_eButton;
};

class MouseMoveEvent {
public:
    MouseMoveEvent( const int32 iX, const int32 iY );
    int32 getX() const;
    int32 getY() const;
private:
    int32 m_iX, m_iY;
};

class KeyPressEvent {
public:
    KeyPressEvent( const uchar pszKey[5] );
    const uchar* getKey() const;
private:
    uchar m_pszKey[5];
};




}

#endif //BS_WINDOWEVENTS_HPP