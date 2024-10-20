/*
Time.hpp
--------
Copyright (c) 2024, theJ89

Description:
    Defines a class that keeps track of time.
*/

#ifndef BS_TIME_HPP
#define BS_TIME_HPP




//Includes
#include <brimstone/Stopwatch.hpp>  //Brimstone::Stopwatch




namespace Brimstone {




//Classes
class Time {
public:
    Time();
    ~Time();

    void frame();
private:
    Stopwatch     m_stopwatch;
    bool          m_paused;
    std::uint64_t m_delta;
    std::uint64_t m_realTime;
    std::uint64_t m_gameTime;
public:
    static void          setPaused( const bool paused );
    static bool          isPaused();
    static std::uint64_t getDelta();
    static std::uint64_t getRealTime();
    static std::uint64_t getGameTime();
private:
    static Time*         m_singleton;
};




}

#endif //BS_TIME_HPP
