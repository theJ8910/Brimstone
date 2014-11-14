/*
util/Math.cpp
-----------------------
Copyright (c) 2014, theJ89

Description:
    See util/Math.hpp for more information.
*/

//Includes
#include <brimstone/util/Math.hpp>      //Header
#include <brimstone/util/Macros.hpp>    //BS_ASSERT_NONZERO_DIVISOR, BS_ASSERT_DOMAIN_GTE




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

/*
fastSqrt
-----------------------

Description:
    A fast and fairly accurate square root function.

    Calculated by calculating the inverse square root and multiplying it against the given value (Quake 3 Method).

    ACCURACY NOTE:
    Can under or overestimate within 0.2% of the correct answer.

Arguments:
    value:              The function calculates an approximation of the square root of this number.
                        WARNING: Cannot be negative.

Returns:
    float:              The square root of the given value.

Throws:
    DomainException:    If value is negative.
*/
float fastSqrt( float value ) {
    BS_ASSERT_DOMAIN_GTE( value, 0 );

    //TODO: Zeroes, infinities, denormalized numbers, and NaNs may be troublesome. Check.
    if( value == 0.0f )
        return 0.0f;

    //value = sqrt( value ) * sqrt( value ), so
    //value / sqrt( value ) = sqrt( value )
    return value * fastInvSqrt( value );
}

/*
fastInvSqrt
-----------------------

Description:
    A fast and fairly accurate inverse square root function that depends on the 32-bit (float) IEEE floating point implementation.
    Calculates an approximation of the inverse of the square root of a number, that is:
    1.0f / sqrt( x ).

    Taken from:
    http://en.wikipedia.org/wiki/Methods_of_computing_square_roots#Approximations_that_depend_on_IEEE_representation

    Based on Quake 3 implementation.

    ACCURACY NOTE:
    Can under or overestimate within 0.2% of the correct answer.

Arguments:
    value:                  The function calculates an approximation of the inverse square root of this number.
                            WARNING: Cannot be negative. Cannot be 0.0f.

Returns:
    float:                  The square root of the given value.

Throws:
    DivideByZeroException:  If value is 0.
    DomainException:        If value is negative.
*/
float fastInvSqrt( float value ) {
    BS_ASSERT_DOMAIN_GTE( value, 0 );
    BS_ASSERT_NONZERO_DIVISOR( value );

    float half = 0.5f * value;
    (int32&)value = 0x5f3759df - ( (int32&)value >> 1 );

    //One iteration of newton's method, using a very good initial guess given above
    //NOTE: This line can be copied and pasted to produce more accurate estimates at the expense of performance.
    value *= ( 1.5f - half * value * value );

    return value;
}

}