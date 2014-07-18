/*
util/Range.hpp
-----------------------
Copyright (c) 2014, theJ89

Description:
    Defines several functions for testing if a given scalar is within a range,
    or restricting the given scalar to a range.
*/

#ifndef BS_UTIL_RANGE_HPP
#define BS_UTIL_RANGE_HPP




namespace Brimstone {

//Faster ------------------------------------------------------------------------------------------------------------------------------------------ Slower
template< typename T >  inline bool         isBetween( const T& val, const T& lowerBound, const T& upperBound );
template< typename T >  inline bool         isOutside( const T& val, const T& lowerBound, const T& upperBound );
template< typename T >  inline bool         approxEquals( const T& actualVal, const T& idealVal, const T& tolerance );
template< typename T >  inline void         clamp( T& valInOut, const T& lowerBound, const T& upperBound );                                         template< typename T >  inline T        ClampedValue( const T& val, const T& lowerBound, const T& upperBound );
template< typename T >  inline void         lowClamp( T& valInOut, const T& lowerBound );                                                           template< typename T >  inline T        LowClampedValue( const T& val, const T& lowerBound );
template< typename T >  inline void         highClamp( T& valInOut, const T& upperBound );                                                          template< typename T >  inline T        HighClampedValue( const T& val, const T& upperBound );




/*
isBetween
-----------------------

Description:
    Takes a value and the lower and upper bounds of a range of numbers.
    Returns true if the value is between the two bounds (inclusive; i.e. the bounds count as "inside" the range).
    Returns false otherwise.

    WARNING: In order for the function to work properly, lowerBound must be less than or equal to upperBound.

Arguments:
    val:                A value.
    lowerBound:         The smallest value in the range.
    upperBound:         The largest value in the range.

Returns:
    bool:               true if the value is between the two bounds, false otherwise.
*/
template< typename T >
inline bool isBetween( const T& val, const T& lowerBound, const T& upperBound )
{
    return ( val >= lowerBound && val <= upperBound );
}

/*
isOutside
-----------------------

Description:
    Takes a value and the lower and upper bounds of a range of numbers.
    Returns true if the value is outside of that range (the range is inclusive; i.e. the bounds count as "inside" the range).
    Returns false otherwise.

    WARNING: In order for the function to work properly, lowerBound must be less than or equal to upperBound.

Arguments:
    val:                A value.
    lowerBound:         The smallest value in the range.
    upperBound:         The largest value in the range.

Returns:
    bool:               true if the value is outside the two bounds, false otherwise.
*/
template< typename T >
inline bool isOutside( const T& val, const T& lowerBound, const T& upperBound )
{
    return ( val < lowerBound || val > upperBound );
}

/*
approxEquals
-----------------------

Description:
    Compares two values - an 'actual' and an 'ideal' - to see if they are "approximately equal" to one another.
    These two values are approximately equal if the difference between them is less than or equal to the given tolerance value.
    This means that the actual can be larger or smaller than the ideal by at most the given tolerance value.

    Returns true if the difference between the two values is less than or equal to the tolerance.
    Returns false otherwise.

    NOTE: The actual and ideal names have no real significance to how the function operates; they were simply chosen arbitarily to tell the two values apart.
          In fact, the actual and ideal values can be swapped with one another without affecting the result of this function.

Arguments:
    actualVal:          One of the two values to compare.
    idealVal:           One of the two values to compare.
    tolerance:          The most these two values can differ without being non-approximately equal.
                        Must be a non-negative number.

Returns:
    bool:               true if the two values are approximately equal, false otherwise.
*/

template< typename T >
inline bool approxEquals( const T& actualVal, const T& idealVal, const T& tolerance )
{
    return abs( actualVal - idealVal ) <= tolerance;
}

/*
clamp
-----------------------

Description:
    Takes a value and two bounds.
    If the value exceeds either of the bounds, replaces it with the bound it exceeded.

    WARNING: In order for the function to work properly, lowerBound must be less than or equal to upperBound.

Arguments:
    valInOut:           The value to potentially clamp.
    lowerBound:         The smallest that val can be.
    upperBound:         The largest that val can be.

Returns:
    void:               N/A
*/
template< typename T >
inline void clamp( T& valInOut, const T& lowerBound, const T& upperBound )
{
    if( valInOut > upperBound )
        valInOut = upperBound;
    else if( valInOut < lowerBound )
        valInOut = lowerBound;
}

/*
clampedValue
-----------------------

Description:
    Takes a value and two bounds.
    The returned value is between these two bounds (inclusive).

    If the value is beneath the lower bound, the lower bound is returned.
    If the value is above the upper bound, the upper bound is returned.
    Otherwise, the value itself is returned.

    WARNING: In order for the function to work properly, lowerBound must be less than or equal to upperBound.

Arguments:
    val:                The value to potentially clamp.
    lowerBound:         The smallest that val can be.
    upperBound:         The largest that val can be.

Returns:
    T:                  A value between lowerBound and upperBound (inclusive).
*/
template< typename T >
inline T clampedValue( const T& val, const T& lowerBound, const T& upperBound )
{
    if( val > upperBound )
        return upperBound;
    else if( val < lowerBound )
        return lowerBound;
    
    return val;
}

/*
lowClamp
-----------------------

Description:
    Takes a value and a lower bound.
    If the value is below the lower bound, it is replaced with the lower bound.

Arguments:
    valInOut:           The value to potentially clamp.
    lowerBound:         The smallest that valInOut can be.

Returns:
    void:               N/A
*/
template< typename T >
inline void lowClamp( T& valInOut, const T& lowerBound )
{
    if( valInOut < lowerBound )
        valInOut = lowerBound;
}

/*
lowClampedValue
-----------------------

Description:
    Takes a value and a lower bound.
    If the value is below the lower bound, the lower bound is returned.
    Otherwise, the value itself is returned.

Arguments:
    val:                The value to potentially clamp.
    lowerBound:         The smallest that val can be.

Returns:
    T:                  A value no larger than lowerBound.
*/
template< typename T >
inline T lowClampedValue( const T& val, const T& lowerBound )
{
    if( val < lowerBound )
        return lowerBound;

    return val;
}

/*
highClamp
-----------------------

Description:
    Takes a value and an upper bound.
    If the value is above the upper bound, it is replaced with the upper bound.

Arguments:
    valInOut:           The value to potentially clamp.
    upperBound:         The largest that valInOut can be.

Returns:
    void:               N/A
*/
template< typename T >
inline void highClamp( T& valInOut, const T& upperBound )
{
    if( valInOut > upperBound )
        valInOut = upperBound;
}

/*
highClampedValue
-----------------------

Description:
    Takes a value and an upper bound.
    If the value is above the upper bound, the upper bound is returned.
    Otherwise, the value itself is returned.

Arguments:
    val:                The value to potentially clamp.
    upperBound:         The largest that val can be.

Returns:
    T:                  A value no larger than upperBound.
*/
template< typename T >
inline T highClampedValue( const T& val, const T& upperBound )
{
    if( val > upperBound )
        return upperBound;

    return val;
}

}




#endif //BS_UTIL_RANGE_HPP