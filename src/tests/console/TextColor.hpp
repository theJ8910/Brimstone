﻿/*
console/TextColor.hpp
---------------------
Copyright (c) 2024, theJ89

Description:
    Declares convenience functions to color the text / background in the Windows and Linux terminals,
    and defines enums that can be given as arguments to TextColor().
*/

#ifndef UT_TEXTCOLOR_HPP
#define UT_TEXTCOLOR_HPP




//Includes
#ifdef UT_BUILD_WINDOWS

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#define STRICT
#include <windows.h>

#endif




namespace UnitTest {




//On windows we need to track a handle to the console window (windows.h is needed for this),
//and use combinations of Windows color enums.
#ifdef UT_BUILD_WINDOWS



//Worthless Globals
extern HANDLE console;

//Constants
enum class TextColors {
    //Greyscale
    WHITE       = FOREGROUND_RED    | FOREGROUND_GREEN  |   FOREGROUND_BLUE |   FOREGROUND_INTENSITY,
    GREY        = FOREGROUND_RED    | FOREGROUND_GREEN  |   FOREGROUND_BLUE,
    BLACK       = 0,

    //Primary Colors
    RED         = FOREGROUND_RED    |   FOREGROUND_INTENSITY,
    DARKRED     = FOREGROUND_RED,
    GREEN       = FOREGROUND_GREEN  |   FOREGROUND_INTENSITY,
    DARKGREEN   = FOREGROUND_GREEN,
    BLUE        = FOREGROUND_BLUE   |   FOREGROUND_INTENSITY,
    DARKBLUE    = FOREGROUND_BLUE,

    //Secondary Colors
    YELLOW      = FOREGROUND_RED    |   FOREGROUND_GREEN    |   FOREGROUND_INTENSITY,
    DARKYELLOW  = FOREGROUND_RED    |   FOREGROUND_GREEN,
    PURPLE      = FOREGROUND_RED    |   FOREGROUND_BLUE     |   FOREGROUND_INTENSITY,
    DARKPURPLE  = FOREGROUND_RED    |   FOREGROUND_BLUE,
    CYAN        = FOREGROUND_BLUE   |   FOREGROUND_GREEN    |   FOREGROUND_INTENSITY,
    DARKCYAN    = FOREGROUND_BLUE   |   FOREGROUND_GREEN,

    //Defaults
    DEFAULT_FOREGROUND = GREY,
    DEFAULT_BACKGROUND = BLACK,
};

#endif //UT_BUILD_WINDOWS

//On linux we use xterm256 color indices
#ifdef UT_BUILD_LINUX

enum class TextColors {

    //Greyscale
    WHITE       = 15,
    GREY        = 7,
    BLACK       = 0,

    //Primary Colors
    RED         = 196,
    DARKRED     = 88,
    GREEN       = 46,
    DARKGREEN   = 28,
    BLUE        = 21,
    DARKBLUE    = 18,

    //Secondary Colors
    YELLOW      = 226,
    DARKYELLOW  = 100,
    PURPLE      = 201,
    DARKPURPLE  = 90,
    CYAN        = 51,
    DARKCYAN    = 30,

    //Defaults
    DEFAULT_FOREGROUND = GREY,
    DEFAULT_BACKGROUND = BLACK,
};

#endif //UT_BUILD_LINUX




//Forward declarations
void initTextColor();
void setTextColor( const TextColors textColor = TextColors::DEFAULT_FOREGROUND, const TextColors backgroundColor = TextColors::DEFAULT_BACKGROUND );

void setTextColorXT( const unsigned char xtermIndex );
void setTextColorXT( const unsigned char red, const unsigned char green, const unsigned char blue );

void setBackgroundColorXT( const unsigned char xtermIndex );
void setBackgroundColorXT( const unsigned char red, const unsigned char green, const unsigned char blue );




} //namespace UnitTest




#endif //UT_TEXTCOLOR_HPP
