/*
Test.hpp
--------
Copyright (c) 2024, theJ89

Description:
    Defines a UnitTest interface, IUnitTest.
*/
#ifndef UT_TEST_HPP
#define UT_TEST_HPP




//Includes
#include <string>       //std::string
#include <set>          //std::set
#include <functional>   //std::functional




#define UT_TEST_BEGIN( name )                                        \
    ::UnitTest::UnitTest test_##name( #name, []() -> bool {

#define UT_TEST_END()                                                \
    } );

#define UT_TEST( name, fn, spec )                                    \
    ::UnitTest::UnitTest test_##name( #name, fn spec );


namespace UnitTest {




//Interface for all unit tests
class IUnitTest {
public:
    virtual bool run() = 0;
    virtual std::string getName() const = 0;
    virtual const std::set< IUnitTest* >& getDependencies() const = 0;
};

//Returns a set of pointers to unit tests
std::set< IUnitTest* >& getTests();




//Abstract Base Class implementing common functionality for IUnitTest
class AbstractUnitTest : public IUnitTest {
public:
    AbstractUnitTest( const std::string& name );
    virtual std::string getName() const;
    virtual const std::set< IUnitTest* >& getDependencies() const;
    void addDependency( IUnitTest* const test );
private:
    std::string             m_name;
    std::set< IUnitTest* >  m_dependencies;
};




//Unit test implementation with a run() that delegates to a function
//provided through the constructor.
//Meant for use with UT_TEST_BEGIN( name ) and UT_TEST_END()
class UnitTest : public AbstractUnitTest {
private:
    typedef bool(*RunTestPtr)();
public:
    UnitTest( const std::string& name, RunTestPtr fn );
    virtual bool run();
private:
    RunTestPtr m_function;
};




}




#endif //UT_TEST_HPP
