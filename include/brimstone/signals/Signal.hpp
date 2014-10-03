/*
signals/Signal.hpp
-----------------------
Copyright (c) 2014, theJ89

Description:
    Signals and slots are a modular form of the observer pattern.
    Signals are analagous to subjects, and slots are analagous to observers.

    Signals are callable objects (i.e. functors) that maintain a list of
    zero or more callbacks functions (known as slots).
    When a signal is called, it calls each registered slot, passing the arguments given to the call
    exactly as they were passed to the signal.

    Signal is a template class that takes the slot type as a parameter.
    All registered slots on a signal must be of this type.
    Attempting to call the signal with a different set of arguments than those supported by Slot
    will cause a compile-time error.
    Note that if the underlying Slot can be called in several different fashions (e.g. slot(int),
    slot(bool), slot(int, int), etc.), then the Signal may be called in each of these fashions as well.
*/
#ifndef BS_SIGNALS_SIGNAL_HPP
#define BS_SIGNALS_SIGNAL_HPP




//Includes
#include <vector>                                   //std::vector
#include <utility>                                  //std::move, std::forward
#include <algorithm>                                //std::find

#include <brimstone/signals/Delegate.hpp>           //Delegate




namespace Brimstone {
namespace Private {
    //If a function signature is provided as the slot type,
    //we need to rewrite it as a function pointer, otherwise
    //C++ will give you tons of warnings and fail to compile
    //(can't allocate a function type, but can allocate a function pointer).
    template< typename T >
    struct ToFunctionPointer {
        typedef T type;
    };
    template< typename R, typename... Args >
    struct ToFunctionPointer< R( Args... ) > {
        typedef R(*type)(Args...);
    };
}

template< typename Slot >
class Signal {
public:
    typedef typename Private::ToFunctionPointer< Slot >::type   MySlot;
    typedef Signal< Slot >                                      MyType;
private:
    typedef std::vector< MySlot >                               SlotCollection;
public:
    Signal() = default;
    Signal( const Signal& toCopy );
    Signal( Signal&& toMove );
    Signal& operator =( Signal&& toMove );

    void connect( const MySlot& slot );
    void connect( MySlot&& slot );
    void operator +=( const MySlot& slot );
    void operator +=( MySlot&& slot );

    void disconnect( const MySlot& slot );
    void operator -=( const MySlot& slot );
    void disconnectAll();

    template< typename... Args >
    void emit( Args&&... args ) const;

    template< typename... Args >
    void operator ()( Args&&... args ) const;

    bool isEmpty() const;
    size_t size() const;
private:
    Signal& operator =( const Signal& toCopy );
private:
    SlotCollection      m_slots;
};

//Do-nothing copy constructor (needed because we want Signal to be copyable, but not its connections)
template< typename Slot >
Signal< Slot >::Signal( const Signal& toCopy ) {}

//Move constructor; transfers slots
template< typename Slot >
Signal< Slot >::Signal( Signal&& toMove ) :
    m_slots( std::move( toMove.m_slots ) ) {
}
//Move assignment operator; transfers slots
template< typename Slot >
Signal< Slot >& Signal< Slot >::operator =( Signal&& toMove ) {
    m_slots = std::move( toMove.m_slots );
}

//Connect a slot (method form)
//...by copying slot
template< typename Slot >
void Signal< Slot >::connect( const MySlot& slot ) {
    m_slots.emplace_back( slot );
}
//...by moving slot
template< typename Slot >
void Signal< Slot >::connect( MySlot&& slot ) {
    m_slots.emplace_back( std::move( slot ) );
}

//Connect a slot (operator form)
//...by copying slot
template< typename Slot >
inline void Signal< Slot >::operator +=( const MySlot& slot ) {
    connect( slot );
}
//...by moving slot
template< typename Slot >
inline void Signal< Slot >::operator +=( MySlot&& slot ) {
    connect( slot );
}

//Disconnect a slot (method form)
template< typename Slot >
void Signal< Slot >::disconnect( const MySlot& slot ) {
    for( auto it = m_slots.begin(); it != m_slots.end(); ++it ) {
        //Note: to be removed, a != operator between two Slot should be defined.
        if( *it != slot )
            continue;

        //Erase the slot
        m_slots.erase( it );
        return;
    }
}

//Disconnect a slot (operator form)
template< typename Slot >
inline void Signal< Slot >::operator -=( const MySlot& slot ) {
    disconnect( slot );
}

//Disconnect all slots
template< typename Slot >
void Signal< Slot >::disconnectAll() {
    m_slots.clear();
}

//Invoke connected slots (method form)
template< typename Slot >
template< typename... Args >
void Signal< Slot >::emit( Args&&... args ) const {
    for( auto& slot : m_slots )
        slot( std::forward< Args >( args )... );
}

//Invoke connected slots (call operator form)
template< typename Slot >
template< typename... Args >
inline void Signal< Slot >::operator ()( Args&&... args ) const {
    emit( std::forward< Args >( args )... );
}

//Do-nothing assignment operator
template< typename Slot >
Signal< Slot >& Signal< Slot >::operator =( const Signal& toCopy ) {
    return *this;
}

//Returns true if no slots are connected, false otherwise
template< typename Slot >
bool Signal< Slot >::isEmpty() const {
    return m_slots.empty();
}

//Returns the number of slots connected
template< typename Slot >
size_t Signal< Slot >::size() const {
    return m_slots.size();
}

//Typedefs
template< typename Signature >
using SignalD = Signal< typename Delegate< Signature > >;

}




#endif //BS_SIGNALS_SIGNAL_HPP
