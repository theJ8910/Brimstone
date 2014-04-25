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

enum class Key {
    A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
    Digit0, Digit1, Digit2, Digit3, Digit4, Digit5, Digit6, Digit7, Digit8, Digit9
};

class MouseDownEvent {
public:
    MouseDownEvent( const MouseButton eButton );
    MouseButton getButton() const;
private:
    MouseButton m_eButton;
};

class MouseUpEvent {
public:
    MouseUpEvent( const MouseButton eButton );
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

class MouseScrollEvent {
public:
    MouseScrollEvent( const int32 iScrollAmount );
private:
    int32 iScrollAmount;
};

class KeyDownEvent {
public:
    KeyDownEvent( const Key eKey );
    Key getKey() const;
private:
    Key m_eKey;
};

class KeyUpEvent {
public:
    KeyUpEvent( const Key eKey );
    Key getKey() const;
private:
    Key m_eKey;
};

class CharacterTypedEvent {
public:
    const uchar* getCharacter() const;
private:
    uchar m_pszKey[5];
};

}

#endif //BS_WINDOWEVENTS_HPP