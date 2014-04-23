#ifndef BS_SIGNALS_HPP
#define BS_SIGNALS_HPP




//Includes
#include "Signal.hpp"
//#include "Slot.hpp"




namespace Brimstone {
namespace Signals {

/*
template< typename Fn_t >
Slot< Fn_t > bind( Fn_t&& fn ) {
    return Slot< Fn_t >( std::forward< Fn_t >( fn ) );
}

//Overload allowing you to do bind( functionName ) instead of bind( &functionName )
template< typename R, typename... P >
Slot< R(*)( P... ) > bind( R(&fn)( P... ) ) {
    return Slot< R(*)( P... ) >( &fn );
}

template< typename Class_t, typename Method_t >
Slot< Fn_t > bind( Class_t& pThis, Method_t&& fn ) {
    return Slot< Method_t >( pThis, std::forward< Fn_t >( fn ) );
}
*/

}
}




#endif //BS_SIGNALS_HPP