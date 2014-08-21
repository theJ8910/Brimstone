/*
Test.cpp
-----------------------
Copyright (c) 2014, theJ89

Description:
    See "Test.hpp" for more information.
*/




//Includes
#include "Test.hpp"     //Class header





namespace UnitTest {

std::set< IUnitTest* >& getTests() {
    static std::set< IUnitTest* > tests;
    return tests;
}

AbstractUnitTest::AbstractUnitTest( const std::string& name ) :
    m_name( name ) {
    getTests().insert( this );
}

std::string AbstractUnitTest::getName() const {
    return m_name;
}

const std::set< IUnitTest* >& AbstractUnitTest::getDependencies() const {
    return m_dependencies;
}

void AbstractUnitTest::addDependency( IUnitTest* const test ) {
    m_dependencies.insert( test );
}

UnitTest::UnitTest( const std::string& name, RunTestPtr fn ) :
    AbstractUnitTest( name ),
    m_function( fn ) {
}

bool UnitTest::run() {
    return m_function();
}

}