/*
input/MouseButton.cpp
---------------------
Copyright (c) 2024, theJ89

Description:
    See MouseButton.hpp for more information.
*/




//Includes
#include <brimstone/input/MouseButton.hpp>  //Header

#include <cstdint>                          //std::size_t




namespace {

constexpr const char* mbToStringMap[ static_cast<std::size_t>( Brimstone::MouseButton::COUNT ) ] = {
    "Invalid",  //<= INVALID

    "Left",     //<= LEFT
    "Right",    //<= RIGHT
    "Middle",   //<= MIDDLE

    "X1",       //<= X1
    "X2"        //<= X2
};

}

namespace Brimstone {

const char* mouseButtonToString( const MouseButton button ) {
    return mbToStringMap[ static_cast<std::size_t>( button ) ];
}

}
