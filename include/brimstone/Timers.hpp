/*
Timers.hpp
----------
Copyright (c) 2024, theJ89

Description:
    Defines the Timers and Timer classes.
    The Timers class manages the creation and execution of timers.
*/
#ifndef BS_TIMERS_HPP
#define BS_TIMERS_HPP




//Includes
#include <cstddef>                         //std::size_t
#include <cstdint>                         //std::uint64_t
#include <limits>                          //std::numeric_limits
#include <functional>                      //std::function

#include <brimstone/Time.hpp>              //Brimstone::getRealTime
#include <brimstone/util/Heap.hpp>         //Brimstone::MinHeap
#include <brimstone/signals/Delegate.hpp>  //Brimstone::Delegate




namespace Brimstone::Private {




template< typename Callback >
class Timer {
public:
    Timer( const std::size_t id, const std::uint64_t time, const Callback callback );
    std::size_t   m_id;
    std::uint64_t m_time;
    Callback      m_callback;
};

template< typename Callback >
Timer< Callback >::Timer( const std::size_t id, const std::uint64_t time, const Callback callback ) :
    m_id( id ),
    m_time( time ),
    m_callback( callback ) {
}

template< typename Callback >
class TimerHeapNodeKey {
public:
    static inline std::uint64_t getKey( const Timer< Callback >* node ) { return node->m_time; }
};




} //namespace Brimstone::Private




namespace Brimstone {




template< typename Callback >
class Timers {
private:
    using TimerQueue = MinHeap< Private::Timer< Callback >*, Private::TimerHeapNodeKey< Callback > >;
public:
    Timers();
    std::size_t setTimeout( const std::uint64_t delay, const Callback callback );
    void        clearTimeout( const std::size_t id );
    void        frame();
private:
    TimerQueue  m_queue;
    std::size_t m_nextID;
};

template< typename Callback >
Timers< Callback >::Timers() :
    m_queue(),
    m_nextID( std::numeric_limits< std::size_t >::max() )
{
}

template< typename Callback >
std::size_t Timers< Callback >::setTimeout( const std::uint64_t delay, const Callback callback ) {
    std::size_t id = ++m_nextID;
    m_queue.push( new Private::Timer< Callback >( id, Time::getRealTime() + delay, callback ) );
    return id;
}

template< typename Callback >
void Timers< Callback >::clearTimeout( const std::size_t id ) {
    for( std::size_t i = 0; i < m_queue.size(); ++i ) {
        Private::Timer< Callback >* timer = m_queue[i];
        if( timer->m_id == id ) {
            m_queue.removeIndex( i );
            delete timer;
            return;
        }
    }
}

template< typename Callback >
void Timers< Callback >::frame() {
    std::uint64_t time = Time::getRealTime();
    Private::Timer< Callback >* timer;
    while( !m_queue.empty() ) {
        //Check the timer at the front of the queue.
        //If it's not time to trigger it yet, we can exit the loop now since we know all the timers that come after it are scheduled to trigger at or later than it.
        timer = m_queue.peek();
        if( timer->m_time > time )
            break;

        //Remove the timer from the queue
        m_queue.pop();

        //Call the timer's callback
        timer->m_callback();

        //Delete the timer
        delete timer;
    }
}




//Types
using TimersD = Timers< Delegate< void() > >;
using TimersF = Timers< std::function< void() > >;




} //namespace Brimstone




#endif //BS_TIMERS_HPP
