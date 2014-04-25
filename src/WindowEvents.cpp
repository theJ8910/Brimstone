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

const uchar* mouseButtonToString( MouseButton eButton ) {
    static const uchar* apszMap[ (uint)MouseButton::COUNT ] = {
        "Left",     //<= LEFT
        "Right",    //<= RIGHT
        "Middle",   //<= MIDDLE
        "X1",       //<= X1
        "X2"        //<= X2
    };

    return apszMap[ (uint)eButton ];
}

const uchar* keyToString( Key eKey ) {
    static const uchar* apszMap[ (uint)Key::COUNT ] = {
        "Invalid",
        "Escape",
        "F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "F10", "F11", "F12",

        "Tilda",
        "0", "1", "2", "3", "4", "5", "6", "7", "8", "9",
        "Dash", "Equals",

        "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M",
        "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z",

        "Space", "Enter", "Tab", "Backspace", "CapsLock", "Comma", "Period", "Quote",
        "Semicolon", "Slash", "Backslash",
        "LeftBracket", "RightBracket",

        "LeftShift",    "RightShift",
        "LeftControl",  "RightControl",
        "LeftAlt",      "RightAlt",
        
        "LeftSystem",   "RightSystem", 

        "PrintScreen", "ScrollLock", "PauseBreak",
        "Insert", "Delete", "Home", "End", "PageUp", "PageDown",

        "Up", "Down", "Left", "Right",

        //Numpad keys
        "Numpad0", "Numpad1", "Numpad2", "Numpad3", "Numpad4", "Numpad5", "Numpad6", "Numpad7", "Numpad8", "Numpad9",
        "Add", "Subtract", "Multiply", "Divide", "Decimal", "NumpadEnter", "Numlock"
    };

    return apszMap[ (uint)eKey ];
}

MouseEvent::MouseEvent( const int32 iX, const int32 iY ) :
    m_iX( iX ), m_iY( iY ) {
}

int32 MouseEvent::getX() const {
    return m_iX;
}

int32 MouseEvent::getY() const {
    return m_iY;
}

MouseButtonEvent::MouseButtonEvent( const MouseButton eButton, const int32 iX, const int32 iY ) :
    MouseEvent( iX, iY ),
    m_eButton( eButton ) {
}

MouseButton MouseButtonEvent::getButton() const {
    return m_eButton;
}

MouseDownEvent::MouseDownEvent( const MouseButton eButton, const int32 iX, const int32 iY ) :
    MouseButtonEvent( eButton, iX, iY ) {
}

MouseUpEvent::MouseUpEvent( const MouseButton eButton, const int32 iX, const int32 iY ) :
    MouseButtonEvent( eButton, iX, iY ) {
}

MouseMoveEvent::MouseMoveEvent( const int32 iX, const int32 iY ) :
    MouseEvent( iX, iY ) {
}

MouseScrollEvent::MouseScrollEvent( const float fScrollAmount, const int32 iX, const int32 iY ) :
    MouseEvent( iX, iY ),
    m_fScrollAmount( fScrollAmount ) {
}

float MouseScrollEvent::getScrollAmount() const {
    //This value is almost always 1.0f or -1.0f.
    //However, this number can potentially be larger or smaller
    //based on the precision of the mousewheel being used.
    //The magnitude of this value indicates how much the wheel was scrolled.
    //The sign of the value indicates the direction the user scrolled:
    //    If scrolling vertically:
    //        +: forwards (towards the screen)
    //        -: backwards (towards the user)
    //    If scrolling horizontally:
    //        +: to the right
    //        -: to the left 
    return m_fScrollAmount;
}

MouseVScrollEvent::MouseVScrollEvent( const float fScrollAmount, const int32 iX, const int32 iY ) :
    MouseScrollEvent( fScrollAmount, iX, iY ) {
}

MouseHScrollEvent::MouseHScrollEvent( const float fScrollAmount, const int32 iX, const int32 iY ) :
    MouseScrollEvent( fScrollAmount, iX, iY ) {
}

KeyEvent::KeyEvent( const Key eKey ) :
    m_eKey( eKey ) {
}

Key KeyEvent::getKey() const {
    return m_eKey;
}

KeyDownEvent::KeyDownEvent( const Key eKey ) :
    KeyEvent( eKey ) {
}

KeyUpEvent::KeyUpEvent( const Key eKey ) :
    KeyEvent( eKey ) {
}

const uchar* CharacterTypedEvent::getCharacter() const {
    return m_pszKey;
}

}