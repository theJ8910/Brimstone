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

template< typename Signature_t >
class Signal;

template< typename Signature_t >
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
    typedef Return Signature_t( Args... );    //Signature_t;
    typedef Signal< Signature_t >               Signal_t;
    typedef ScopedConnection< Signature_t >     My_t;

    typedef std::vector< Signal_t* >            SignalCollection_t;
public:
    ~ScopedConnection();
    void release();
private:
    void connected( Signal_t& signal );
    void disconnected( Signal_t& signal );
private:
    SignalCollection_t  m_signals;
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
void ScopedConnection< Return ( Args... ) >::connected( Signal_t& signal ) {
    m_signals.push_back( &signal );
}

template< typename Return, typename... Args >
void ScopedConnection< Return ( Args... ) >::disconnected( Signal_t& signal ) {
    auto it = std::find( m_signals.begin(), m_signals.end(), &signal );
    if( it == m_signals.end() )
        return;
    m_signals.erase( it );
}

}




#endif //BS_SIGNALS_SCOPEDCONNECTION_HPP