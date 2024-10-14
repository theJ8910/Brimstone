/*
input/Key.hpp
-------------
Copyright (c) 2024, theJ89

Description:
    The Key enum and its associated functions are defined in this header.

    All keyboard keys recognized by Brimstone are defined here.
*/
#ifndef BS_INPUT_KEY_HPP
#define BS_INPUT_KEY_HPP




namespace Brimstone {

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
    MINUS, EQUALS,

    //Letters
    A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,

    //Miscellaneous keys on the main part of the keyboard
    SPACE, RETURN, TAB, BACKSPACE, CAPS_LOCK, COMMA, PERIOD, QUOTE,
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
    ENTER, CLEAR,

    //Media keys
    MEDIA_PLAY_PAUSE, MEDIA_STOP, MEDIA_PREVIOUS, MEDIA_NEXT,

    //Volume controls
    VOLUME_UP, VOLUME_DOWN, MUTE,

    //Dummy value to retrieve total number of key codes
    COUNT
};

const char* keyToString( const Key key );

}




#endif //BS_INPUT_KEY_HPP
