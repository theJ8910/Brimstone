/*
Stopwatch.cpp
-----------------------
Copyright (c) 2014, theJ89

Description:
    See Stopwatch.hpp for more information.
*/




//Includes
#include <brimstone/Stopwatch.hpp>  //Header file




namespace Brimstone {

Stopwatch::Stopwatch() {
    reset();
}

void Stopwatch::reset() {
    m_start = std::chrono::high_resolution_clock::now();
}

//Returns the number of milliseconds that have passed between calls to .start() and .stop()
uint64 Stopwatch::get() {
    using std::chrono::duration_cast;
    using std::chrono::milliseconds;
    typedef std::chrono::high_resolution_clock clock;

    Time now = clock::now();
    uint64 off = duration_cast< milliseconds >( now - m_start ).count();
    m_start = now;

    return off;
}

}