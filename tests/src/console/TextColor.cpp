/*
console/TextColor.cpp
-----------------------
Copyright (c) 2014, theJ89

Description:
    See console/TextColor.h for more information.
*/




//Includes
#include "TextColor.hpp"

#ifdef UT_BUILD_LINUX
#include <stdio.h>
#endif //UT_BUILD_LINUX




namespace UnitTest {

//Globals
#ifdef UT_BUILD_WINDOWS
HANDLE console = 0;
#endif //UT_BUILD_WINDOWS




//Forward Declarations
#ifdef UT_BUILD_LINUX
uint8 getXTIndex( unsigned char red, unsigned char green, unsigned char blue );
#endif //UT_BUILD_LINUX




/*
InitTextColor
-----------------------

Description:
    This function must be called once at startup to make use of the TextColor function.

Arguments:
    N/A

Returns:
    void:   N/A
*/
void initTextColor() {
#ifdef UT_BUILD_WINDOWS
    console = GetStdHandle( STD_OUTPUT_HANDLE );
#endif //UT_BUILD_WINDOWS
}


/*
setTextColor
-----------------------

Description:
    When called prior to outputting text to the console, this function sets the foreground and background color of the text.
    Call this with no arguments to reset the text color back to normal.

Arguments:
    textColor:          The color of the text itself.
                        Valid values are defined in the TextColors enum in TextColor.h.
                        Defaults to TextColors::DEFAULT_FOREGROUND.
    backgroundColor:    The color of the text's background.
                        The same values that work for textColor will work for the background.
                        Defaults to TextColors::DEFAULT_BACKGROUND.

Returns:
    void:               N/A
*/
void setTextColor( const TextColors textColor, const TextColors backgroundColor ) {

#ifdef UT_BUILD_WINDOWS

    //Background colors are defined the same way as foreground colors, but four bits shifted to the left
    SetConsoleTextAttribute( console, (unsigned short)textColor | ( (unsigned short)backgroundColor << (unsigned short)4u ) );

#endif //UT_BUILD_WINDOWS

#ifdef UT_BUILD_LINUX

    printf( "\x1B[38;5;%um\x1B[48;5;%um", textColor, backgroundColor );

#endif //UT_BUILD_LINUX
}

#ifdef UT_BUILD_LINUX

/*
setTextColorXT{1}
-----------------------

Description:
    When called prior to outputting text to the console, this function sets the foreground color of the text.
    This function supports a greater range of colors than TextColor does.

    Does nothing on Windows.

Arguments:
    xtermIndex:     An xterm256 color index.

Returns:
    void:           N/A
*/
void setTextColorXT( const unsigned char xtermIndex ) {
    //NOTE: \x1B is short for "character with hexidecimal value 1B". 1B in hex is 16+11 = 27.
    //In the ASCII table, character 27 is the "escape" control character.
    //38 indicates we want to use xterm256 colors to set the foreground color
    //5 needs to be given after that as well (I'm guessing this is related to the 6 degrees each color has, 0-5?)
    printf( "\x1B[38;5;%um", xtermIndex );
}

/*
setTextColorXT{2}
-----------------------

Description:
    Same as above, but rather than taking a color index,
    takes RGB values and tries to match them as closely to a color index as possible.

Arguments:
    red:            The red component of the color, between 0 and 255.
    green:          The green component of the color, between 0 and 255.
    blue:           The blue component of the color, between 0 and 255.

Returns:
    void:           N/A
*/
void setTextColorXT( const unsigned char red, const unsigned char green, const unsigned char blue ) {
    setTextColorXT( getXTIndex( red, green, blue ) );
}

/*
setBackgroundColorXT{1}
-----------------------

Description:
    When called prior to outputting text to the console, this function sets the background color of the text.
    This function supports a greater range of colors than TextColor does.

    Does nothing on Windows.

Arguments:
    xtermIndex:     An xterm256 color index.

Returns:
    void:           N/A
*/
void setBackgroundColorXT( const unsigned char xtermIndex ) {
    printf( "\x1B[48;5;%um", xtermIndex );
}

/*
setBackgroundColorXT{2}
-----------------------

Description:
    When called prior to outputting text to the console, this function sets the background color of the text.
    This function supports a greater range of colors than TextColor does.

    Does nothing on Windows.

Arguments:
    red:            The red component of the color, between 0 and 255.
    green:          The green component of the color, between 0 and 255.
    blue:           The blue component of the color, between 0 and 255.

Returns:
    void:           N/A
*/
void setBackgroundColorXT( const unsigned char red, const unsigned char green, const unsigned char blue ) {
    //Same as setting foreground color, except 48 indicates we want to use xterm256 colors to set the background color
    setBackgroundColorXT( getXTIndex( red, green, blue ) );
}

/*
getXTIndex
-----------------------

Description:
    Returns an xterm256 color index close to the given RGB values.

Arguments:
    red:            The red component of the color, between 0 and 255.
    green:          The green component of the color, between 0 and 255.
    blue:           The blue component of the color, between 0 and 255.

Returns:
    uint8:          The xterm256 color index.
*/
unsigned char getXTIndex( const unsigned char red, const unsigned char green, const unsigned char blue ) {
    unsigned char colorIndex;

    //If red, green, and blue are the same the color is monochrome
    if( red == green && red == blue ) {
        //RGB less than 16 uses the system black
        if( red < 16 ) {
            colorIndex = 0;
        //RGB greater than 237 uses the system white
        } else if( red > 237 ) {
            colorIndex = 15;

        //RGB between [16, 237] use the greyscale index range
        } else {
            //Grayscale colors start at index 232 and end at index 255.
            //There are 24 shades of grey: 255-232 + 1 = 23 + 1
            //So we have to transform the RBG 16-237 range into a 0-23 range.
            //We can subtract 16 from the red, putting the RGB into the 0-221 range.
            //Then dividing by 9.21 we get a number between 0 and 23.
            colorIndex = 232 + ( red - 16 ) / 9.21f;
        }
    } else {
        static const float scaling = 1.0f / 43.0f;

        //Non-system colors are from 16 to 231.
        //This means there are 216 colors = 6*6*6 colors.
        //In other words, this allows for 6 degrees of red, 6 degrees of green, and 6 degrees of blue.
        //We transform the RGB values from 0-255 to the rgb values 0-5.
        //The index can be thought of as 16 + a base-6 number in the form rgb, where r is the most significant digit and b is the least significant digit.
        //So, forming the index is basically just computing the base-6 number from it's digits, and then adding 16, which is what we do here:
        colorIndex =       16u                              +
                     36u * unsigned char( scaling * red   ) +
                      6u * unsigned char( scaling * green ) +
                           unsigned char( scaling * blue  );
    }

    return colorIndex;
}

#else //UT_BUILD_LINUX

void setTextColorXT( const unsigned char ) {}
void setTextColorXT( const unsigned char, const unsigned char, const unsigned char ) {}
void setBackgroundColorXT( const unsigned char ) {}
void setBackgroundColorXT( const unsigned char, const unsigned char, const unsigned char ) {}

#endif //UT_BUILD_LINUX

}