/*
WindowEvent.hpp
-----------------------
Copyright (c) 2014, theJ89

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
    KeyDown,        //msg.key
    KeyUp,          //msg.key
    Text,           //msg.text
    Focus,          //N/A
    Blur,           //N/A
    Move,           //msg.move
    Resize,         //msg.resize
    Close           //N/A
};

struct WindowEvent {
    WindowEventType type;
    union {
        struct Mouse {
            int x;
            int y;
            MouseButton button;
        } mouse;
        struct MouseMove {
            int x;
            int y;
        } mouseMove;
        struct MouseScroll {
            int x;
            int y;
            float scroll;
        } mouseScroll;
        struct KeyMessage {
            Key key;
        } key;
        struct TextMessage {
            uchar utf8[5];
        } text;
        struct MoveMessage {
            int x;
            int y;
        } move;
        struct ResizeMessage {
            int w;
            int h;
        } resize;
    };
};



}

#endif //BS_WINDOWEVENTS_HPP
