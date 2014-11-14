/*
input/MouseButton.cpp
-----------------------
Copyright (c) 2014, theJ89

Description:
    See MouseButton.hpp for more information.
*/




//Includes
#include <brimstone/input/MouseButton.hpp>  //Header




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

}