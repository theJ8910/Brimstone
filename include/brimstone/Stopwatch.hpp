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
#include <cstdint>  //std::uint64_t
#include <chrono>   //std::chrono




namespace Brimstone {




class Stopwatch {
private:
    using Clock = std::chrono::high_resolution_clock;
    using Time  = std::chrono::high_resolution_clock::time_point;
public:
    Stopwatch();
    void reset();
    template< typename Duration >
    std::uint64_t get();
    std::uint64_t getMilliseconds();
    std::uint64_t getMicroseconds();
    std::uint64_t getNanoseconds();
private:
    Time m_start;
};

template< typename Duration >
std::uint64_t Stopwatch::get() {
    using std::chrono::duration_cast;

    Time now = Clock::now();
    std::uint64_t off = duration_cast< Duration >( now - m_start ).count();
    m_start = now;

    return off;
}




} //namespace Brimstone




#endif //BS_STOPWATCH_HPP
