﻿/*
console/Menu.cpp
----------------
Copyright (c) 2024, theJ89

Description:
    See console/Menu.hpp for more information.
*/




//Includes
#include "Menu.hpp"         //Header
#include "TextColor.hpp"    //UnitTest::setTextColor
#include "../Exception.hpp" //UnitTest::EOFError

#include <iostream>         //std::cout, std::cin
#include <limits>           //std::numeric_limits




namespace UnitTest {




/*
menu
----

Description:
    Displays a numbered menu of available options (starting at 1) to the user and asks him to make a choice.
    Returns the index (starting at 0) of the chosen option.

    The user's input is validated - if the user makes an invalid choice (i.e. text or an out-of-range number),
    then the user is informed and the function keeps looping until a valid choice is made by the user.

    Empty entries (for spacing and numbering purposes) are allowed, and will be displayed as unnumbered blank lines.
    These are not counted as valid choices. Numbering continues to increase.

Arguments:
    choices:     An array of C-style strings (array of const char*) containing the choices offered by the menu.
    numChoices:  The number of strings in the above array (empty strings count!)

Returns:
    int:  index of the chosen choice (between 0 and iNumChoices)

Throws:
    EOFError:  If EOF is encountered on stdin while waiting for input.
*/
int menu( const char* const* const choices, const int numChoices ) {
    int choice;
    bool invalidOption = true;
    do {
        std::cout << "Please select an action to perform:"
                  << std::endl;
        for( int i = 0; i < numChoices; ++i ) {
            if( *choices[i] != '\0' )
                std::cout << "    " << ( i + 1 ) << ".) " << choices[i];
            std::cout << std::endl;
        }
        std::cout << std::endl
                  << "Choose an option: "; std::cin >> choice;
        std::cout << std::endl;

        //Detect end-of-file (EOF) in the input stream; this can happen if, for example, the user presses Ctrl + D on their keyboard.
        if( std::cin.eof() ) {
            throw EOFError( "EOF encountered while waiting for user input." );
        }

        //If the user supplied invalid input, we can detect it and reset the stream.
        //If we didn't do this, besides staying in the "fail" state, the invalid input would remain on the stream.
        if( std::cin.fail() ) {
            std::cin.clear();
            std::cin.ignore( std::numeric_limits<int>::max(), '\n' );
            choice = -1;
        }

        //The user provided a valid integer, but does that correspond to a valid choice?
        if( choice > 0 && choice <= numChoices && *choices[ choice - 1 ] != '\0' ) {
            invalidOption = false;
        //If not, the user is warned and we repeat the prompt.
        } else {
            setTextColor( TextColors::RED );
            std::cout << "That is not a valid choice. Please try again." << std::endl
                      << std::endl;
            setTextColor();
        }
    } while( invalidOption );
    return choice - 1;
}




} //namespace UnitTest
