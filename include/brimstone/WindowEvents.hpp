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

//Forward declarations
class Window;

enum class MouseButton {
    //Invalid buttons are mapped to this button
    INVALID,

    //Standard left, right, and middle mouse (mousewheel click) buttons
    LEFT,
    RIGHT,
    MIDDLE,

    //Some mice have a couple of extra buttons (usually on the side)
    X1,
    X2,

    //Dummy value to retrieve total number of buttons
    COUNT
};

const uchar* mouseButtonToString( const MouseButton eButton );

enum class Key {
    //Invalid key codes are mapped to this key
    INVALID,

    //Keys on the "function line" on the main part of the keyboard
    ESCAPE,
    F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,

    //Keys on the "number line" on the main part of the keyboard
    //Note: ~ is tilde, ` is backtick; they share the same key
    TILDE,
    // )        !        @        #        $        %        ^        &        *        (
    DIGIT_0, DIGIT_1, DIGIT_2, DIGIT_3, DIGIT_4, DIGIT_5, DIGIT_6, DIGIT_7, DIGIT_8, DIGIT_9,
    // _     +
    DASH, EQUALS,

    //Letters
    A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,

    //Miscellaneous keys on the main part of the keyboard
    SPACE, ENTER, TAB, BACKSPACE, CAPS_LOCK, COMMA, PERIOD, QUOTE,
    SEMICOLON, SLASH, BACKSLASH,
    // [ {      } ]
    LBRACKET, RBRACKET,

    LSHIFT,   RSHIFT,
    LCTRL,    RCTRL,
    LALT,     RALT,
    //Windows key for instance
    LSYSTEM,  RSYSTEM,
    MENU,
    
    //Keys above the arrow keys
    PRINT_SCREEN, SCROLL_LOCK, PAUSE_BREAK,
    INSERT, DEL, HOME, END, PAGE_UP, PAGE_DOWN,

    //Arrow keys
    UP, DOWN, LEFT, RIGHT,

    //Numpad keys
    NUMPAD_0, NUMPAD_1, NUMPAD_2, NUMPAD_3, NUMPAD_4, NUMPAD_5, NUMPAD_6, NUMPAD_7, NUMPAD_8, NUMPAD_9,
    ADD, SUBTRACT, MULTIPLY, DIVIDE, DECIMAL, NUMLOCK,
    NUMPAD_INSERT, NUMPAD_DEL, NUMPAD_HOME, NUMPAD_END, NUMPAD_PAGE_UP, NUMPAD_PAGE_DOWN,
    NUMPAD_UP, NUMPAD_DOWN, NUMPAD_LEFT, NUMPAD_RIGHT,
    NUMPAD_ENTER, CLEAR,

    //Dummy value to retrieve total number of key codes
    COUNT
};

const uchar* keyToString( const Key eKey );

class MouseEvent {
public:
    MouseEvent( const int32 iX, const int32 iY );
    int32 getX() const;
    int32 getY() const;
private:
    int32 m_iX, m_iY;
};

class MouseButtonEvent : public MouseEvent {
public:
    MouseButtonEvent( const MouseButton eButton, const int32 iX, const int32 iY );
    MouseButton getButton() const;
private:
    MouseButton m_eButton;
};

class MouseDownEvent : public MouseButtonEvent {
public:
    MouseDownEvent( const MouseButton eButton, const int32 iX, const int32 iY );
};

class MouseUpEvent : public MouseButtonEvent {
public:
    MouseUpEvent( const MouseButton eButton, const int32 iX, const int32 iY );
};

class MouseMoveEvent : public MouseEvent {
public:
    MouseMoveEvent( const int32 iX, const int32 iY );
};

class MouseScrollEvent : public MouseEvent {
public:
    MouseScrollEvent( const float fScrollAmount, const int32 iX, const int32 iY );
    float getScrollAmount() const;
private:
    float m_fScrollAmount;
};

class MouseVScrollEvent : public MouseScrollEvent {
public:
    MouseVScrollEvent( const float fScrollAmount, const int32 iX, const int32 iY );
};

class MouseHScrollEvent : public MouseScrollEvent {
public:
    MouseHScrollEvent( const float fScrollAmount, const int32 iX, const int32 iY );
};

class KeyEvent {
public:
    KeyEvent( const Key eKey );
    Key getKey() const;
private:
    Key m_eKey;
};

class KeyDownEvent : public KeyEvent {
public:
    KeyDownEvent( const Key eKey );
};

class KeyUpEvent : public KeyEvent {
public:
    KeyUpEvent( const Key eKey );
};

class CharacterTypedEvent {
public:
    const uchar* getCharacter() const;
private:
    uchar m_pszKey[5];
};

class WindowCloseEvent {
public:
    WindowCloseEvent( Window& cWindow );
    Window* getWindow() const;
private:
    Window* m_pcWindow;
};

}

#endif //BS_WINDOWEVENTS_HPP
