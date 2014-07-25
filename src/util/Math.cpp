/*
util/Math.cpp
-----------------------
Copyright (c) 2014, theJ89

Description:
    See util/Math.hpp for more information.
*/

//Includes
#include <brimstone/util/Math.hpp>

namespace Brimstone {

/*
leadingZeroCount's implementation is platform dependent.
On some platforms it may not be available.
*/
#if defined( BS_BUILD_WINDOWS )

inline uint32 leadingZeroCount( const uint32 i ) {
    return __lzcnt( i );
}

#elif defined( BS_BUILD_LINUX )

inline uint32 leadingZeroCount( const uint32 i ) {
    return __builtin_clz( i );
}

#endif

uint32 closestUpperPowerOfTwo( const uint32 i ) {
    /*
    This approach works by finding the position of the leading zero closest to the
    most significant bit in the number and shifting "1" into that location.

    Example: Lets say 3 is given to closestUpperPowerOfTwo.
    The closest power of 2 greater than or equal to 3 is 4.
    
    We start by subtracting 1 from the given number.
    This ensures powers of 2 aren't rounded up to the next power (e.g. 4 rounded up to 8).

    3-1 = 2. The binary representation of 2 is:
        00000000 00000000 00000000 00000010

    leadingZeroCount(x) returns the number of leading zeroes in the binary representation of x.
    It won't work as intended on 0 or negative numbers, hence the i < 2 check.
    In this example, there are a total of 30 leading zeroes.
    32 - 30 = 2. If we shift "1" two places to the left, we'll get the following number in binary:
        00000000 00000000 00000000 00000100
    In decimal, this 4 as we expected.
    */
    if( i < 2 )
        return i;

    return 1 << ( 32 - leadingZeroCount( i - 1 ) );
}

}