/*
Stopwatch.cpp
-------------
Copyright (c) 2024, theJ89

Description:
    See Stopwatch.hpp for more information.
*/




//Includes
#include <brimstone/Stopwatch.hpp>  //Header




namespace Brimstone {




Stopwatch::Stopwatch() {
    reset();
}

void Stopwatch::reset() {
    m_start = Clock::now();
}

std::uint64_t Stopwatch::getMilliseconds() {
    return get<std::chrono::milliseconds>();
}

std::uint64_t Stopwatch::getMicroseconds() {
    return get<std::chrono::microseconds>();
}

std::uint64_t Stopwatch::getNanoseconds() {
    return get<std::chrono::nanoseconds>();
}

//Returns the number of milliseconds that have passed between calls to .start() and .stop()




} //namespace Brimstone
