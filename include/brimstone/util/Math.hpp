/*
util/Math.hpp
-----------------------
Copyright (c) 2014, theJ89

Description:
    Miscellaneous math functions are declared here.
*/
#ifndef BS_MATH_HPP
#define BS_MATH_HPP




//Includes
#include "../types.hpp" //Brimstone::int32




namespace Brimstone {

/*
leadingZeroCount's implementation is platform dependent.
On some platforms it may not be available.
*/
#if defined( BS_BUILD_WINDOWS ) || defined( BS_BUILD_LINUX )

inline uint32 leadingZeroCount( const uint32 i );

#elif 

inline uint32 leadingZeroCount( const uint32 i ) {
    return __builtin_clz( i );
}

#endif

uint32 closestUpperPowerOfTwo( const uint32 i );

}

#endif //BS_MATH_HPP