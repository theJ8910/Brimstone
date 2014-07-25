/*
Test.hpp
-----------------------
Copyright (c) 2014, theJ89

Description:
    Defines a UnitTest interface, IUnitTest.
*/
#ifndef BS_UT_TEST_HPP
#define BS_UT_TEST_HPP




//Includes
#include <string>       //std::string
#include <set>          //std::set
#include <functional>   //std::functional




#define BS_UT_TEST_BEGIN( name )                \
    UnitTest test_##name( #name, []() -> bool { \

#define BS_UT_TEST_END()                        \
    } );                                        \

namespace Brimstone {
namespace UnitTest {

class IUnitTest {
public:
    virtual bool run() = 0;
    virtual std::string getName() const = 0;
    virtual const std::set< IUnitTest* >& getDependencies() const = 0;
};

class UnitTest : public IUnitTest {
private:
    typedef bool(*RunTestPtr)();
public:
    UnitTest( const std::string& name, RunTestPtr fn );
    virtual bool run();
    virtual std::string getName() const;
    virtual const std::set< IUnitTest* >& getDependencies() const;
    void addDependency( IUnitTest* const test );
private:
    RunTestPtr              m_function;
    std::string             m_name;
    std::set< IUnitTest* >  m_dependencies;
};

std::set< IUnitTest* >& getTests();

}
}




#endif //BS_UT_TEST_HPP