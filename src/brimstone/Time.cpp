/*
Time.cpp
---------
Copyright (c) 2024, theJ89

Description:
    See Time.hpp for more information.
*/




//Includes
#include <brimstone/Time.hpp>       //Class header
#include <brimstone/Exception.hpp>  //Brimstone::Exception




namespace Brimstone {




//Static class variables
Time* Time::m_singleton = nullptr;




Time::Time() :
    m_stopwatch(),
    m_paused( false ),
    m_delta( 0 ),
    m_realTime( 0 ),
    m_gameTime( 0 ) {
    if( m_singleton != nullptr )
        throw Exception( "More than one instance of the Time class may not be instantiated." );
    m_singleton = this;
}

Time::~Time() {
    if( m_singleton != nullptr )
        m_singleton = nullptr;
}

void Time::frame() {
    //Get delta time for this frame
    m_delta  = m_stopwatch.getNanoseconds();

    //Advance real time
    m_realTime += m_delta;

    //Advance game time if the game is not paused
    if( !m_paused )
        m_gameTime += m_delta;
}

void Time::setPaused( const bool paused ) {
    m_singleton->m_paused = paused;
}

bool Time::isPaused() {
    return m_singleton->m_paused;
}

std::uint64_t Time::getDelta() {
    return m_singleton->m_delta;
}

std::uint64_t Time::getRealTime() {
    return m_singleton->m_realTime;
}

std::uint64_t Time::getGameTime() {
    return m_singleton->m_gameTime;
}




}
