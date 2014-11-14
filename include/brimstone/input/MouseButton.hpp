/*
input/MouseButton.hpp
-----------------------
Copyright (c) 2014, theJ89

Description:
    The MouseButton enum and its associated functions are defined in this header.

    All mouse buttons recognized by Brimstone are defined here.
*/
#ifndef BS_INPUT_MOUSEBUTTON_HPP
#define BS_INPUT_MOUSEBUTTON_HPP




//Includes
#include <brimstone/types.hpp>  //uchar




namespace Brimstone {

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

const uchar* mouseButtonToString( const MouseButton button );

}




#endif //BS_INPUT_MOUSEBUTTON_HPP