/*
signals/ScopedConnection.hpp
-----------------------
Copyright (c) 2014, theJ89

Description:
    See Signal.hpp for more information.
*/
#ifndef BS_SIGNALS_SCOPEDCONNECTION_HPP
#define BS_SIGNALS_SCOPEDCONNECTION_HPP




//Includes
#include <vector>                   //std::vector
#include <utility>                  //std::move, std::forward
#include <algorithm>                //std::find




namespace Brimstone {

template< typename Signature >
class Signal;

template< typename Signature >
class ScopedConnection;

//When connecting a Signal to a Slot, the connection optionally can be managed by a ScopedConnection.
//The Signal will call ScopedConnection's connected() method, which will register it with the ScopedConnection.
//If the Signal is destroyed, or the associated connection is broken,
//it will call disconnected() on the ScopedConnection, unregistering it.
//If the ScopedConnection is destroyed, for all registered Signals in the ScopedConnection,
//connections involving this ScopedConnection are broken.
template< typename Return, typename... Args >
class ScopedConnection< Return ( Args... ) > {
friend class Signal< Return ( Args... ) >;

public:
    typedef Return Signature( Args... );      //Signature;
    typedef Signal< Signature >                 MySignal;
    typedef ScopedConnection< Signature >       My;

    typedef std::vector< MySignal* >            SignalCollection;
public:
    ~ScopedConnection();
    void release();
private:
    void connected( MySignal& signal );
    void disconnected( MySignal& signal );
private:
    SignalCollection    m_signals;
};

template< typename Return, typename... Args >
ScopedConnection< Return ( Args... ) >::~ScopedConnection() {
    //Break connections with signals
    for( auto it = m_signals.begin(); it != m_signals.end(); ++it )
        (**it).disconnect( *this );
}

template< typename Return, typename... Args >
void ScopedConnection< Return ( Args... ) >::release() {
    for( auto it = m_signals.begin(); it != m_signals.end(); ++it )
        (**it).release( *this );

    m_signals.clear();
}

template< typename Return, typename... Args >
void ScopedConnection< Return ( Args... ) >::connected( MySignal& signal ) {
    m_signals.push_back( &signal );
}

template< typename Return, typename... Args >
void ScopedConnection< Return ( Args... ) >::disconnected( MySignal& signal ) {
    auto it = std::find( m_signals.begin(), m_signals.end(), &signal );
    if( it == m_signals.end() )
        return;
    m_signals.erase( it );
}

}




#endif //BS_SIGNALS_SCOPEDCONNECTION_HPP
