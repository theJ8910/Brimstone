/*
input/Key.cpp
-------------
Copyright (c) 2024, theJ89

Description:
    See Key.hpp for more information.
*/




//Includes
#include <brimstone/input/Key.hpp>  //Header

#include <cstdint>                  //std::size_t




namespace {




constexpr const char* keyToStringMap[ static_cast<std::size_t>( Brimstone::Key::COUNT ) ] = {
    "Invalid",
    "Escape",
    "F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "F10", "F11", "F12",

    "Tilde",
    "0", "1", "2", "3", "4", "5", "6", "7", "8", "9",
    "Minus", "Equals",

    "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M",
    "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z",

    "Space", "Return", "Tab", "Backspace", "CapsLock", "Comma", "Period", "Quote",
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
    "Enter", "Clear",

    //Media keys
    "MediaPlayPause", "MediaStop", "MediaPrevious", "MediaNext",

    //Volume controls
    "VolumeUp", "VolumeDown", "Mute"
};




} //namespace




namespace Brimstone {




const char* keyToString( const Key key ) {
    return keyToStringMap[ static_cast<std::size_t>( key ) ];
}




} //namespace Brimstone
