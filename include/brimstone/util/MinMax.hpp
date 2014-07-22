/*
util/MinMax.hpp
-----------------------
Copyright (c) 2014, theJ89

Description:
    Defines several functions for taking the minimum, maximum,
    or both minimum and maximum of two or more numbers.
*/

#ifndef BS_UTIL_MINMAX_HPP
#define BS_UTIL_MINMAX_HPP




//Includes
#include <utility>                      //std::swap

#include <brimstone/Exception.hpp>      //NullPointerException, SizeException
#include <brimstone/types.hpp>          //uint32
#include <brimstone/util/Macros.hpp>    //BS_ASSERT_NON_NULLPTR, BS_ASSERT_SIZE, etc




namespace Brimstone {

//Math
//Faster ------------------------------------------------------------------------------------------------------------------------------------------ Slower

template< typename T >  inline void         electMin( T& currentInWinnerOut, const T& candidate );
template< typename T >  inline void         min( const T& a, const T& b, T& minOut );                                                               template< typename T >  inline T        min( const T& a, const T& b );
template< typename T >  inline void         min( const T* values, uint32 numItems, T& minOut );

template< typename T >  inline void         electMax( T& currentInWinnerOut, const T& candidate );
template< typename T >  inline void         max( const T& a, const T& b, T& maxOut );                                                               template< typename T >  inline T        max( const T& a, const T& b );
template< typename T >  inline void         max( const T* values, uint32 numItems, T& maxOut );

template< typename T >  inline void         electMinMax( T& minInOut, T& maxInOut, const T& candidate );
template< typename T >  inline void         minMax( const T& a, const T& b, T& minOut, T& maxOut );                                                 template< typename T >  inline void     minMax( T& aInMinOut, T& bInMaxOut );
template< typename T >  inline void         minMax( const T* values, uint32 numItems, T& minOut, T& maxOut );


/*
electMin
-----------------------

Description:
    In this function, two values (the first of which is currently considered the smallest) are compared,
    and whichever value is smaller replaces the first value.

    The function's naming comes from the idea it's trying to convey. There are two candidates, one currently in
    office and another competiting for office. In an election, the people decide who is better suited to be in office.

    The metaphor:
    The "office" is the variable the first value is stored in.
    The first candidate (the value of the first variable) is currently in that office.
    The second candidate (the value of the second variable) will potentially replace the person currently in office.
    Whichever candidate is selected (the smallest variable) will occupy the office after the election (after the function runs).

    Usage:
        int smallest = 10;
        int a        = 20;
        int b        = 5;
        electMin( smallest, a ); //smallest is still 10
        electMin( smallest, b ); //smallest is now 5

Arguments:
    currentInWinnerOut:     The current value. Can potentially be replaced by xCandidate.
    candidate:              A potentially smaller value.

Returns:
    void:                   N/A
*/
template< typename T >
inline void electMin( T& currentInWinnerOut, const T& candidate ) {
    if( candidate < currentInWinnerOut )
        currentInWinnerOut = candidate;
}

/*
min{1}
-----------------------

Description:
    Takes two values, compares them, and sets minOut to the smaller value.

    Usage:
        int a = 1;
        int b = 2;
        int smallest;
        min( a, b, smallest );

Arguments:
    a:          A value.
    b:          Another value.
    minOut:     A given variable set to the smaller of the two values.

Returns:
    void:       N/A
*/
template< typename T >
inline void min( const T& a, const T& b, T& minOut ) {
    minOut = ( ( a < b ) ? a : b );
}

/*
min{2}
-----------------------

Description:
    Takes two values, compares them, and returns the smaller value.
    NOTE: In some cases this may be less efficient to use than the above function.

    Usage:
        int a = 1;
        int b = 2;
        int smallest = min( a, b );

Arguments:
    a:      A value.
    b:      Another value.

Returns:
    T:      The smaller value.
*/
template< typename T >
inline T min( const T& a, const T& b ) {
    return ( ( a < b ) ? a : b );
}

/*
min{3}
-----------------------

Description:
    Finds the smallest element (minimum) in an array of values.

    Usage:
        int arr[] = { 162, 26, 31, 34, 255, 64, 7 }
        int smallest;
        min( arr, 7, smallest );

Arguments:
    values:                 Pointer to an array of values to find the smallest element of.
                                Cannot be nullptr.
    numItems:               The number of items in the array.
                                Must be at least 1.
    minOut:                 Will contain the smallest value in the array after the function runs.

Returns:
    void:                   N/A

Throws:
    NullPointerException:   If values is nullptr.
    SizeException:          If numItems is 0.
*/
template< typename T >
inline void min( const T* values, uint32 numItems, T& minOut ) {
    BS_ASSERT_NON_NULLPTR( values );
    BS_ASSERT_SIZE( numItems, 1 );

    minOut = *values;
    while( ( --numItems ) > 0u ) {
        if( *(++values) < minOut )
            minOut = *values;
    }
}

/*
electMax
-----------------------

Description:
    In this function, two values (the first of which is currently considered the largest) are compared,
    and whichever value is larger replaces the first value.

    The naming / logic behind it is the same as "electMin", except instead of focusing on the smallest value,
    this function focuses on the largest value instead.

    Usage:
        int largest = 10;
        int a       = 20;
        int b       = 5;
        electMax( largest, a ); //largest is now 20
        electMax( largest, b ); //largest is still 20

Arguments:
    currentInWinnerOut:     The current value. Can potentially be replaced by candidate.
    candidate:              A potentially larger value.

Returns:
    void:                   N/A
*/
template< typename T >
inline void electMax( T& currentInWinnerOut, const T& candidate ) {
    if( candidate > currentInWinnerOut )
        currentInWinnerOut = candidate;
}

/*
max{1}
-----------------------

Description:
    Takes two values, compares them, and sets maxOut to the larger value.
    
    Usage:
        int a = 1;
        int b = 2;
        int largest;
        max( a, b, largest );

Arguments:
    a:          A value.
    b:          Another value.
    maxOut:     A given variable set to the larger of the two values.

Returns:
    void:       N/A
*/
template< typename T >
inline void max( const T& a, const T& b, T& maxOut ) {
    maxOut = ( ( a > b ) ? a : b );
}

/*
max{2}
-----------------------

Description:
    Takes two values, compares them, and returns the larger value.

    Usage:
        int a = 1;
        int b = 2;
        int largest = max( a, b );

Arguments:
    a:      A value.
    b:      Another value.

Returns:
    T:      The larger value.
*/
template< typename T >
inline T max( const T& a, const T& b ) {
    return ( ( a > b ) ? a : b );
}

/*
max{3}
-----------------------

Description:
    Finds the largest element (maximum) in an array of values.

    Usage:
        int arr[] = { 162, 26, 31, 34, 255, 64, 7 }
        int largest;
        max( arr, 7, largest );

Arguments:
    values:                 Pointer to an array of values to find the largest element of.
                                Cannot be nullptr.
    numItems:               The number of items in the array.
                                Must be at least 1.
    maxOut:                 Will contain the largest value in the array after the function runs.

Returns:
    void:                   N/A

Throws:
    NullPointerException:   If values is nullptr.
    SizeException:          If numItems is 0.
*/
template< typename T >
inline void max( const T* values, uint32 numItems, T& maxOut ) {
    BS_ASSERT_NON_NULLPTR( values );
    BS_ASSERT_SIZE( numItems, 1 );

    maxOut = *values;
    while( ( --numItems ) > 0u ) {
        if( *( ++values ) > maxOut )
            maxOut = *values;
    }
}

/*
electMinMax
-----------------------

Description:
    In this function, two values (the first and second are the values currently considered smallest and largest, respectively) are compared
    with a third value, which can possibly replace one value or the other (but not both).
    
    That is, one of thee things can happen:
    If the third value is beneath min, it becomes the new min;
    If the third value is above max, it becomes the new max;
    or if the third value is between min and max (inclusive), nothing happens.

    The naming / logic behind it is the same as "electMin", except instead of just focusing on the smallest value,
    this function focuses on both a smallest and largest value.

    IMPORTANT:
    minInOut is expected to be less than or equal to maxInOut prior to calling this function.

    Usage:
        int smallest    = 10;
        int largest     = 15;

        int a           = 20;
        int b           = 5;
        int c           = 30
        
        electMinMax( smallest, largest, a );
        electMinMax( smallest, largest, b );
        electMinMax( smallest, largest, c );

Arguments:
    minInOut:       Currently the smallest value. Can potentially be replaced by candidate.
    maxInOut:       Currently the largest value. Can potentially be replaced by candidate.
    candidate:      A value potentially smaller than minInOut or larger than maxInOut.

Returns:
    void:           N/A
*/
template< typename T >
inline void electMinMax( T& minInOut, T& maxInOut, const T& candidate ) {
    if( candidate > maxInOut )
        maxInOut = candidate;
    else if( candidate < minInOut )
        minInOut = candidate;
}

/*
minMax{1}
-----------------------

Description:
    Takes two values, compares them, and sets minOut to the smaller value,
    and maxOut to the larger value.

    Note:
        With minMax{1}, minOut and maxOut CANNOT be the same variable as "a" and "b".
        Instead, you should use minMax{2}.

    Usage:
        int a = 1;
        int b = 2;
        int smallest
        int largest;
        minMax( a, b, smallest, largest );

Arguments:
    a:          A value.
    b:          Another value.
    minOut:     A given variable set to the smaller of the two values.
    maxOut:     A given variable set to the larger of the two values.

Returns:
    void:       N/A
*/
template< typename T >
inline void minMax( const T& a, const T& b, T& minOut, T& maxOut ) {
    if( a > b ) {
        minOut = b;
        maxOut = a;
    } else {
        minOut = a;
        maxOut = b;
    }
}

/*
minMax{2}
-----------------------

Description:
    Takes two values, compares them, and if necessary swaps
    them so that aInMinOut will be the minimum value,
    and bInMaxOut will be the maximum value.

    Usage:
        int a = 2;
        int b = 1;
        minMax( a, b );

Arguments:
    aInMinOut:      A reference to an existing variable holding the first value.
                    Will contain the smaller of the two values after the function runs.
    bInMaxOut:      A reference to an existing variable holding the second value.
                    Will contain the larger of the two values after the function runs.

Returns:
    void:           N/A
*/
template< typename T >
inline void minMax( T& aInMinOut, T& bInMaxOut ) {
    if( aInMinOut > bInMaxOut )
        std::swap( aInMinOut, bInMaxOut );
}

/*
minMax{3}
-----------------------

Description:
    Finds the smallest (min) and largest (max) element in an array of values.

    Usage:
        int arr[] = { 162, 26, 31, 34, 255, 64, 7 }
        int smallest;
        int largest;
        minMax( arr, 7, smallest, largest );

Arguments:
    values:         Pointer to an array of values to find the min/max of.
                        Cannot be nullptr.
    numItems:       The number of items in the array.
                        Must be at least 1.
    minOut:         Will contain the smallest value in the array after the function runs.
    maxOut:         Will contain the largest value in the array after the function runs.

Returns:
    void:           N/A

Throws:
    NullPointerException:   If values is nullptr.
    SizeException:          If numItems is 0.
*/
template< typename T >
inline void minMax( const T* values, uint32 numItems, T& minOut, T& maxOut ) {
    BS_ASSERT_NON_NULLPTR( values );
    BS_ASSERT_SIZE( numItems, 1 );

    minOut = maxOut = *values;
    while( (--numItems) > 0u ) {
        ++values;
        if( *values > maxOut )
            maxOut = *values;
        else if( *values < minOut )
            minOut = *values;
    }
}

}




#endif //BS_UTIL_MINMAX_HPP