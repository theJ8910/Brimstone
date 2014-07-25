/*
Main.cpp
-----------------------
Copyright (c) 2014, theJ89

Description:
    Root of Brimstone's UnitTest program.
*/




//#define _ITERATOR_DEBUG_LEVEL 0




//Includes
#include <iostream>                 //std::cout
#include "console/TextColor.hpp"    //InitTextColor
#include "console/Menu.hpp"         //menu
#include "MeasureXTime.hpp"         //measure
#include "Test.hpp"                 //getTests




namespace Brimstone {
namespace UnitTest {

void doTests() {
    int pass  = 0;
    int fail  = 0;
    int total = 0;
    bool status;
    for( auto test : getTests() ) {
        std::cout << test->getName() << ": ";

        try {
            status = test->run();
        } catch( ... ) { status = false; }

        if( status ) {
            setTextColor( TextColors::GREEN );
            std::cout << "PASS" << std::endl;
            setTextColor();
            ++pass;
        } else {
            setTextColor( TextColors::RED );
            std::cout << "FAIL" << std::endl;
            setTextColor();
            ++fail;
        }
        ++total;
    }
    std::cout << std::endl
              << "Unit tests complete." << std::endl
              << "Passed: " << pass << std::endl
              << "Failed: " << fail << std::endl
              << "Total:  " << total << std::endl;
}

}
}

const char* const choices[] = { "Do Tests", "", "Quit" };

int main( int argc, char** argv ) {
    using namespace Brimstone::UnitTest;

    initTextColor();

    setTextColor( TextColors::YELLOW );
    std::cout << "Unit test environment initialized." << std::endl;
    setTextColor();

    std::cout << "This program tests the functionality of the various" << std::endl
              << "building blocks the engine uses (such as vectors, matrices, etc)." << std::endl
              << std::endl;

    int choice;
    while( ( choice = menu( choices, 3 ) ) != 2 ) {
        switch( choice ) {
        case 0:
            doTests();
        }
    }

    return 0;
}