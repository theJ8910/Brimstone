/*
main.cpp
--------
Copyright (c) 2024, theJ89

Description:
    Root of Brimstone's UnitTest program.
*/




//Includes
#include <iostream>               //std::cout
#include "console/TextColor.hpp"  //UnitTest::InitTextColor
#include "console/Menu.hpp"       //UnitTest::menu
#include "MeasureXTime.hpp"       //UnitTest::measure
#include "Test.hpp"               //UnitTest::getTests
#include "Exception.hpp"          //UnitTest::EOFError




namespace {




//Constants
constexpr const char* choices[] = { "Do Tests", "", "Quit" };




} //namespace




namespace UnitTest {




void doTests() {
    int pass  = 0;
    int fail  = 0;
    int total = 0;
    bool status;
    bool exception;
    for( auto test : getTests() ) {
        try {
            status = test->run();
            exception = false;
        } catch( ... ) { status = false; exception = true; }

        if( status ) {
            setTextColor( TextColors::GREEN );
            std::cout << "PASS";
            setTextColor();
            std::cout << ": ";
            ++pass;
        } else {
            if( exception ) {
                setTextColor( TextColors::PURPLE );
                std::cout << "XCPT";
            } else {
                setTextColor( TextColors::RED );
                std::cout << "FAIL";
            }
            setTextColor();
            std::cout << ": ";
            ++fail;
        }
        std::cout << test->getName() << std::endl;
        ++total;
    }
    std::cout << std::endl;
    setTextColor( TextColors::YELLOW );
    std::cout << "Unit tests complete." << std::endl;
    setTextColor();
    std::cout << "Passed: " << pass  << std::endl
              << "Failed: " << fail  << std::endl
              << "Total:  " << total << std::endl
              << std::endl;
}




} //namespace UnitTest




int main( int /* argc */, char** /* argv */ ) {
    using namespace UnitTest;

    initTextColor();

    setTextColor( TextColors::YELLOW );
    std::cout << "Unit test environment initialized." << std::endl;
    setTextColor();

    std::cout << "This program tests the functionality of the various" << std::endl
              << "building blocks the engine uses (such as vectors, matrices, etc)." << std::endl
              << std::endl;

    int choice;
    do {
        try {
            choice = menu( choices, 3 );
        } catch( const EOFError& e ) {
            std::cout << e.getDescription() << std::endl;
            choice = 2;
        }

        switch( choice ) {
            case 0: {
                doTests();
            } break;
        }
    } while( choice != 2 );

    return 0;
}
