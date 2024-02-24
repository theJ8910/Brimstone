/*
Stopwatch.hpp
-------------
Copyright (c) 2024, theJ89

Description:
    Stopwatch is defined here.
    You can use a Stopwatch to measure how much time has passed between
    calls to .start() and .stop().
*/
#ifndef BS_STOPWATCH_HPP
#define BS_STOPWATCH_HPP




//Includes
#include <brimstone/types.hpp>  //Brimstone::uint64
#include <chrono>               //std::chrono




namespace Brimstone {

class Stopwatch {
private:
    typedef std::chrono::high_resolution_clock::time_point Time;
public:
    Stopwatch();
    void reset();
    uint64 get();
private:
    Time    m_start;
};

}

#endif //BS_STOPWATCH_HPP
