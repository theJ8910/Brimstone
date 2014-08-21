/*
util/Math.hpp
-----------------------
Copyright (c) 2014, theJ89

Description:
    Miscellaneous math functions are declared here.
*/
#ifndef BS_UTIL_MATH_HPP
#define BS_UTIL_MATH_HPP




//Includes
#include <brimstone/types.hpp>  //Brimstone::int32




namespace Brimstone {

/*
leadingZeroCount's implementation is platform dependent.
On some platforms it may not be available.
*/
#if defined( BS_BUILD_WINDOWS ) || defined( BS_BUILD_LINUX )

inline uint32 leadingZeroCount( const uint32 i );

#endif

uint32 closestUpperPowerOfTwo( const uint32 i );

float fastSqrt( const float value );
float fastInvSqrt( float value );

}

#endif //BS_UTIL_MATH_HPP