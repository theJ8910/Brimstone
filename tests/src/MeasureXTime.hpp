/*
MeasureXTime.hpp
-----------------------
Copyright (c) 2014, theJ89

Description:
    Functions are provided here to measure the execution times of particular functions, and compare them to other functions.

    Typically, to test a function you inherit from Brimstone::UnitTest::BaseTest,
    and override its methods as needed.

    Then you call:
    Brimstone::UnitTest::measure< TestName >()
    where TestName is the name of your class.

    You can also supply multiple tests if you want to compare runtimes:
    Brimstone::UnitTest::measure< Test1Name, Test2Name >

    measure() will instantiate a TestName,
    call the test's .getCount() method to get "count", the number of times it should loop (defaults to 10000).
    Then, it calls its .begin() method to initialize anything test-specific,
    starts the stopwatch,
    then call its .run() method "count" times.
    It then stops the stopwatch, and records how much time that has passed.
    Finally it calls its .end() to clean up anything test-specific,
    and reports the results to the user.
*/
#ifndef UT_MEASUREXTIME_HPP
#define UT_MEASUREXTIME_HPP




//Includes
#include <iostream>     //std::cout
#include <string>       //std::string
#include <chrono>       //std::chrono::high_resolution_clock




namespace UnitTest {

//Run a single test
template< typename SingleTest >
void measure() {
    using namespace std::chrono;
    typedef high_resolution_clock clock;

    SingleTest test;
    int count = test.getCount();

    test.begin();
    auto begin = clock::now();

    for( int i = 0; i < count; ++i )
        test.run();

    auto elapsed = duration_cast< milliseconds >( clock::now() - begin );
    test.end();

    std::cout << "\"" << test.getName() << "\" took " << elapsed.count() << " ms to loop " << count << " times." << std::endl;
}

//Run 2 or more tests
template< typename FirstTest, typename SecondTest, typename... RemainingTests >
void measure() {
    //Run the first test
    timings< FirstTest >();

    //Run the second test and any other remaining tests
    timings< SecondTest, RemainingTests... >();
}

class BaseRuntimeTest {
public:
    std::string getName() const { return "(NAME NOT SET)"; };
    int getCount() { return 10000; };
    void begin() {}
    void run() {}
    void end() {}
};

}




#endif //UT_MEASUREXTIME_HPP