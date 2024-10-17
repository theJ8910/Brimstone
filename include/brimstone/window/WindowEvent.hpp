/*
window/WindowEvent.hpp
----------------------
Copyright (c) 2024, theJ89

Description:
    WindowEvent and WindowEventType are defined here.
    WindowEvent records events sent to a Window by the operating system.
    This includes the type of event that occured (a WindowEventType),
    and data related to the event (e.g. the button / key pressed, mouse move location, etc)..
*/

#ifndef BS_WINDOW_WINDOWEVENT_HPP
#define BS_WINDOW_WINDOWEVENT_HPP




//Includes
#include <brimstone/types.hpp>              //Brimstone::int32, Brimstone::uchar
#include <brimstone/Point.hpp>              //Brimstone::Point2i
#include <brimstone/input/Key.hpp>          //Brimstone::Key
#include <brimstone/input/MouseButton.hpp>  //Brimstone::MouseButton




namespace Brimstone {

//Forward declarations
class Window;



enum class WindowEventType {
    MouseUp,        //msg.mouse
    MouseDown,      //msg.mouse
    MouseMove,      //msg.mouseMove
    MouseVScroll,   //msg.mouseScroll
    MouseHScroll,   //msg.mouseScroll
    MouseEnter,     //msg.mouseMove
    MouseLeave,     //msg.mouseMove
    KeyDown,        //msg.key
    KeyUp,          //msg.key
    Text,           //msg.text
    Focus,          //N/A
    Blur,           //N/A
    Move,           //msg.move
    Resize,         //msg.resize
    EnterFullscreen,//N/A
    ExitFullscreen, //N/A
    Maximize,       //N/A
    Unmaximize,     //N/A
    Minimize,       //N/A
    Unminimize,     //N/A
    Shade,          //N/A
    Unshade,        //N/A
    Restore,        //N/A
    Close           //N/A
};

struct WindowEventMouse {
    int x;
    int y;
    MouseButton button;
    bool ctrl;
    bool alt;
    bool shift;
    bool system;
};

struct WindowEventMouseMove {
    int x;
    int y;
    bool ctrl;
    bool alt;
    bool shift;
    bool system;
};

struct WindowEventMouseScroll {
    int x;
    int y;
    float scroll;
    bool ctrl;
    bool alt;
    bool shift;
    bool system;
};

struct WindowEventKey {
    Key key;
    bool ctrl;
    bool alt;
    bool shift;
    bool system;
    bool repeat;
};

struct WindowEventText {
    uchar utf8[5];
    bool  repeat;
};

struct WindowEventMove {
    int x;
    int y;
};

struct WindowEventResize {
    int w;
    int h;
};

struct WindowEvent {
    WindowEventType type;
    union {
        WindowEventMouse        mouse;
        WindowEventMouseMove    mouseMove;
        WindowEventMouseScroll  mouseScroll;
        WindowEventKey          key;
        WindowEventText         text;
        WindowEventMove         move;
        WindowEventResize       resize;
    };
};



}

#endif //BS_WINDOWEVENTS_HPP
