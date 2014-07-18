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

const uchar* mouseButtonToString( const MouseButton button ) {
    static const uchar* mbToStringMap[ (uint)MouseButton::COUNT ] = {
        "Invalid",  //<= INVALID

        "Left",     //<= LEFT
        "Right",    //<= RIGHT
        "Middle",   //<= MIDDLE

        "X1",       //<= X1
        "X2"        //<= X2
    };

    return mbToStringMap[ (uint)button ];
}

const uchar* keyToString( const Key key ) {
    static const uchar* keyToStringMap[ (uint)Key::COUNT ] = {
        "Invalid",
        "Escape",
        "F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "F10", "F11", "F12",

        "Tilde",
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
        "Menu",

        "PrintScreen", "ScrollLock", "PauseBreak",
        "Insert", "Delete", "Home", "End", "PageUp", "PageDown",

        "Up", "Down", "Left", "Right",

        //Numpad keys
        "Numpad0", "Numpad1", "Numpad2", "Numpad3", "Numpad4", "Numpad5", "Numpad6", "Numpad7", "Numpad8", "Numpad9",
        "Add", "Subtract", "Multiply", "Divide", "Decimal",
        "Numlock",
        "NumpadInsert", "NumpadDelete", "NumpadHome", "NumpadEnd", "NumpadPageUp", "NumpadPageDown",
        "NumpadUp", "NumpadDown", "NumpadLeft", "NumpadRight",
        "NumpadEnter", "Clear"
    };

    return keyToStringMap[ (uint)key ];
}

MouseEvent::MouseEvent( const int32 x, const int32 y ) :
    m_x( x ), m_y( y ) {
}

int32 MouseEvent::getX() const {
    return m_x;
}

int32 MouseEvent::getY() const {
    return m_y;
}

MouseButtonEvent::MouseButtonEvent( const MouseButton button, const int32 x, const int32 y ) :
    MouseEvent( x, y ),
    m_button( button ) {
}

MouseButton MouseButtonEvent::getButton() const {
    return m_button;
}

MouseDownEvent::MouseDownEvent( const MouseButton button, const int32 x, const int32 y ) :
    MouseButtonEvent( button, x, y ) {
}

MouseUpEvent::MouseUpEvent( const MouseButton button, const int32 x, const int32 y ) :
    MouseButtonEvent( button, x, y ) {
}

MouseMoveEvent::MouseMoveEvent( const int32 x, const int32 y ) :
    MouseEvent( x, y ) {
}

MouseScrollEvent::MouseScrollEvent( const float scrollAmount, const int32 x, const int32 y ) :
    MouseEvent( x, y ),
    m_scrollAmount( scrollAmount ) {
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
    return m_scrollAmount;
}

MouseVScrollEvent::MouseVScrollEvent( const float scrollAmount, const int32 x, const int32 y ) :
    MouseScrollEvent( scrollAmount, x, y ) {
}

MouseHScrollEvent::MouseHScrollEvent( const float scrollAmount, const int32 x, const int32 y ) :
    MouseScrollEvent( scrollAmount, x, y ) {
}

KeyEvent::KeyEvent( const Key key ) :
    m_key( key ) {
}

Key KeyEvent::getKey() const {
    return m_key;
}

KeyDownEvent::KeyDownEvent( const Key key ) :
    KeyEvent( key ) {
}

KeyUpEvent::KeyUpEvent( const Key key ) :
    KeyEvent( key ) {
}

const uchar* CharacterTypedEvent::getCharacter() const {
    return m_key;
}

WindowCloseEvent::WindowCloseEvent( Window& window ) : m_window( &window ) {
}

Window* WindowCloseEvent::getWindow() const {
    return m_window;
}

}
