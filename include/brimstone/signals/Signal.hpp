/*
signals/Signal.hpp
-----------------------
Copyright (c) 2014, theJ89

Description:
    Signals and slots are a modular form of the observer pattern.
    Signals are analagous to subjects, and slots are analagous to observers.
    
    Signals are callable objects (i.e. functors) that maintain a list of
    zero or more callbacks functions (known as slots).
    When a signal is called, it calls each registered slot, passing the values given to the call.

    To ensure that one cannot register a slot that takes a different set of parameters than those given to a call to a Signal object,
    Signal is a template class that takes a function signature as a parameter.
    This signature defines the return value and set of parameters the slots must possess,
    as well as the set of parameters the call operator takes.

    Furthermore, slots are implemented as Delegates, allowing both static functions and bound methods to be utilized as callbacks.

    For situations where the lifetime of a bound object is shorter than the signal it's registered with,
    a slot can optionally be registered with a ScopedConnection object, which will automatically break associated connections when
    the ScopedConnection is destroyed. The reverse is also true; if the Signal is destroyed, the ScopedConnection object stops tracking
    any registered Signals.
*/
#ifndef BS_SIGNALS_SIGNAL_HPP
#define BS_SIGNALS_SIGNAL_HPP




//Includes
#include <vector>                                   //std::vector
#include <utility>                                  //std::move, std::forward
#include <algorithm>                                //std::find

#include <brimstone/signals/Delegate.hpp>           //Delegate
#include <brimstone/signals/ScopedConnection.hpp>   //ScopedConnection




namespace Brimstone {

template< typename Signature >
class ScopedConnection;

template< typename Signature >
class Signal;

template< typename Return, typename... Args >
class Signal< Return ( Args... ) > {
    friend class ScopedConnection< Return ( Args... ) >;
private:
    typedef Return Signature( Args... );              //Signature;
    typedef Delegate< Signature >                       Slot;
    typedef Signal< Signature >                         My;
    typedef ScopedConnection< Signature >               MyScopedConnection;
    typedef std::pair< Slot, MyScopedConnection* >        Connection;

    typedef std::vector< Connection >                   SlotCollection;

public:
    Signal();
    Signal( const Signal& toCopy );
    ~Signal();

    void connect( const Slot& slot );
    void connect( const Slot& slot, MyScopedConnection& scopedConnection );
    void disconnect( const Slot& slot );
    void disconnectAll();

    void emit( Args... args ) const;
    void operator()( Args... args ) const;

private:
    void disconnect( const MyScopedConnection& scopedConnection );
    void release( const MyScopedConnection& scopedConnection );
    void disconnectAllFromSC();
    Signal& operator =( const Signal& toCopy );
    
private:
    SlotCollection      m_slots;
};

//Do-nothing default constructor (needed because we have a copy constructor)
template< typename Return, typename... Args >
Signal< Return ( Args... ) >::Signal() {}

//Do-nothing copy constructor (needed because we want Signal to be copyable, but not its connections)
template< typename Return, typename... Args >
Signal< Return ( Args... ) >::Signal( const Signal& toCopy ) {}

//Unregisters itself with any remaining scoped connections
template< typename Return, typename... Args >
Signal< Return ( Args... ) >::~Signal() {
    disconnectAllFromSC();
}

//Connect signal to a slot (not managed by a scoped connection)
template< typename Return, typename... Args >
void Signal< Return ( Args... ) >::connect( const Slot& slot ) {
    m_slots.emplace_back( slot, nullptr );
}

//Connect signal to a slot (managed by a scoped connection)
template< typename Return, typename... Args >
void Signal< Return ( Args... ) >::connect( const Slot& slot, MyScopedConnection& scopedConnection ) {
    m_slots.emplace_back( slot, &scopedConnection );
    scopedConnection.connected( *this );
}

//Disconnect a slot, inform scoped connection (if any)
template< typename Return, typename... Args >
void Signal< Return ( Args... ) >::disconnect( const Slot& slot ) {
    for( auto it = m_slots.begin(); it != m_slots.end(); ++it ) {
        if( it->first != slot )
            continue;

        //Inform associated scoped connection we're no longer connected
        MyScopedConnection* scope = it->second;
        if( scope != nullptr )
            scope->disconnected( *this );
                
        //Erase the slot
        m_slots.erase( it );
        return;
    }
}

//Disconnect all connected slots, inform their scoped connections
template< typename Return, typename... Args >
void Signal< Return ( Args... ) >::disconnectAll() {
    disconnectAllFromSC();
    m_slots.clear();
}

//Invoke connected slots (method form)
template< typename Return, typename... Args >
void Signal< Return ( Args... ) >::emit( Args... args ) const {
    for( auto it = m_slots.begin(); it != m_slots.end(); ++it )
        (*it).first( std::forward< Args >( args )... );
}

//Invoke connected slots (call operator form)
template< typename Return, typename... Args >
inline void Signal< Return ( Args... ) >::operator ()( Args... args ) const {
    emit( std::forward< Args >( args )... );
}

//Disconnect all slots managed by the given scoped connection
template< typename Return, typename... Args >
void Signal< Return ( Args... ) >::disconnect( const MyScopedConnection& scopedConnection ) {
    //HACK: We don't bother to inform scopedConnection about individual slots being disconnected()
    //because it's assumed that this method will only be called when the scopedConnection is being destroyed.
    for( auto it = m_slots.begin(); it != m_slots.end(); /* N/A */ ) {
        //Skip this slot if it isn't managed by the given scoped connection.
        if( it->second != &scopedConnection )
            ++it;
        //Otherwise, erase the slot
        else
            it = m_slots.erase( it );

    }
}

//All slots managed by the given scoped connection will no longer be managed by it
template< typename Return, typename... Args >
void Signal< Return ( Args... ) >::release( const MyScopedConnection& scopedConnection ) {
    for( auto it = m_slots.begin(); it != m_slots.end(); ++it ) {
        if( it->second != &scopedConnection )
            continue;

        it->second = nullptr;
    }
}

//Do-nothing assignment operator
template< typename Return, typename... Args >
Signal< Return ( Args... ) >& Signal< Return ( Args... ) >::operator =( const Signal& toCopy ) {
    return *this;
}

template< typename Return, typename... Args >
void Signal< Return ( Args... ) >::disconnectAllFromSC() {
    for( auto it = m_slots.begin(); it != m_slots.end(); ++it ) {
        //Inform associated scoped connection we're no longer connected
        MyScopedConnection* scope = it->second;
        if( scope != nullptr )
            scope->disconnected( *this );
    }
}

}




#endif //BS_SIGNALS_SIGNAL_HPP
