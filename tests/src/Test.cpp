/*
Test.cpp
-----------------------
Copyright (c) 2014, theJ89

Description:
    See "Test.hpp" for more information.
*/




//Includes
#include "Test.hpp"     //Class header




namespace Brimstone {
namespace UnitTest {

UnitTest::UnitTest( const std::string& name, RunTestPtr fn ) :
    m_name( name ),
    m_function( fn ) {
    getTests().insert( this );
}

bool UnitTest::run() {
    return m_function();
}

std::string UnitTest::getName() const {
    return m_name;
}

const std::set< IUnitTest* >& UnitTest::getDependencies() const {
    return m_dependencies;
}

void UnitTest::addDependency( IUnitTest* const test ) {
    m_dependencies.insert( test );
}

std::set< IUnitTest* >& getTests() {
    static std::set< IUnitTest* > tests;
    return tests;
}

}
}