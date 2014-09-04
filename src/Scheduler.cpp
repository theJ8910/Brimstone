/*
Scheduler.cpp
-----------------------
Copyright (c) 2014, theJ89

Description:
    See Scheduler.hpp for more information.
*/




//Includes
#include <brimstone/Scheduler.hpp>  //Header
#include <brimstone/Logger.hpp>     //TEMP

#include <chrono>                   //std::chrono::*




namespace Brimstone {

BS_REGISTER_SYSTEM( Scheduler, SCHEDULER );

Scheduler* Scheduler::m_singleton = nullptr;
Scheduler::Scheduler() :
    m_running( false ),
    m_delay( 16 ) {                 //Note: 1000 ms / 60 FPS ~= 16 ms/frame
    if( m_singleton != nullptr )
        throw SingletonException();
    m_singleton = this;
}

Scheduler::~Scheduler() {
    if( m_singleton == this )
        m_singleton = nullptr;
}

void Scheduler::start() {
    m_running = true;
    m_thread = std::thread( [this]{ (*this).run(); } );
}

void Scheduler::stop() {
    m_running = false;
}

void Scheduler::preframe() {
}

void Scheduler::frame() {
    logInfo( "Frame" );
}

void Scheduler::postframe() {
}

void Scheduler::setFrameDelay( int delayInMilliseconds ) {
    m_delay = delayInMilliseconds;
}

int Scheduler::getFrameDelay() const {
    return m_delay;
}

void Scheduler::run() {
    typedef std::chrono::high_resolution_clock clock;
    using std::chrono::milliseconds;

    auto nextFrame = clock::now();
    while( m_running ) {
        nextFrame += milliseconds( m_delay );
        Systems::frame();
        std::this_thread::sleep_until( nextFrame );
    }
}

Scheduler* Scheduler::get() {
    return m_singleton;
}

}