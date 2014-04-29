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
template <typename T>   inline bool         IsBetween( const T& xVal, const T& xLowerBound, const T& xUpperBound );
template <typename T>   inline bool         IsOutside( const T& xVal, const T& xLowerBound, const T& xUpperBound );
template <typename T>   inline bool         ApproxEquals( const T& xActualVal, const T& xIdealVal, const T& xTolerance );
template <typename T>   inline void         Clamp( T& xValInOut, const T& xLowerBound, const T& xUpperBound );                                      template <typename T>   inline T        ClampedValue( const T& xVal, const T& xLowerBound, const T& xUpperBound );
template <typename T>   inline void         LowClamp( T& xValInOut, const T& xLowerBound );                                                         template <typename T>   inline T        LowClampedValue( const T& xVal, const T& xLowerBound );
template <typename T>   inline void         HighClamp( T& xValInOut, const T& xUpperBound );                                                        template <typename T>   inline T        HighClampedValue( const T& xVal, const T& xUpperBound );




/*
IsBetween
-----------------------

Description:
    Takes a value and the lower and upper bounds of a range of numbers.
    Returns true if the value is between the two bounds (inclusive; i.e. the bounds count as "inside" the range).
    Returns false otherwise.

    WARNING: In order for the function to work properly, _LowerBound must be less than or equal to _UpperBound.

Arguments:
    xVal:               A value.
    xLowerBound:        The smallest value in the range.
    xUpperBound:        The largest value in the range.

Returns:
    bool:               true if the value is between the two bounds, false otherwise.
*/
template< typename T >
inline bool IsBetween( const T& xVal, const T& xLowerBound, const T& xUpperBound )
{
    return ( xVal >= xLowerBound && xVal <= xUpperBound );
}

/*
IsOutside
-----------------------

Description:
    Takes a value and the lower and upper bounds of a range of numbers.
    Returns true if the value is outside of that range (the range is inclusive; i.e. the bounds count as "inside" the range).
    Returns false otherwise.

    WARNING: In order for the function to work properly, _LowerBound must be less than or equal to _UpperBound.

Arguments:
    xVal:               A value.
    xLowerBound:        The smallest value in the range.
    xUpperBound:        The largest value in the range.

Returns:
    bool:               true if the value is outside the two bounds, false otherwise.
*/
template< typename T >
inline bool IsOutside( const T& xVal, const T& xLowerBound, const T& xUpperBound )
{
    return ( xVal < xLowerBound || xVal > xUpperBound );
}

/*
ApproxEquals
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
    xActualVal:         One of the two values to compare.
    xIdealVal:          One of the two values to compare.
    xLowerBound:        The most these two values can differ without being non-approximately equal.

Returns:
    bool:               true if the two values are approximately equal, false otherwise.
*/

template< typename T >
inline bool ApproxEquals( const T& xActualVal, const T& xIdealVal, const T& xTolerance )
{
    return abs( xActualVal - xIdealVal ) <= xTolerance;
}

/*
Clamp
-----------------------

Description:
    Takes a value and two bounds.
    If the value exceeds either of the bounds, replaces it with the bound it exceeded.

    WARNING: In order for the function to work properly, _LowerBound must be less than or equal to _UpperBound.

Arguments:
    xValInOut:          The value to potentially clamp.
    xLowerBound:        The smallest that _Val can be.
    xUpperBound:        The largest that _Val can be.

Returns:
    void:               N/A
*/
template< typename T >
inline void Clamp( T& xValInOut, const T& xLowerBound, const T& xUpperBound )
{
    if( xValInOut > xUpperBound )
        xValInOut = xUpperBound;
    else if( xValInOut < xLowerBound )
        xValInOut = xLowerBound;
}

/*
ClampedValue
-----------------------

Description:
    Takes a value and two bounds.
    The returned value is between these two bounds (inclusive).

    If the value is beneath the lower bound, the lower bound is returned.
    If the value is above the upper bound, the upper bound is returned.
    Otherwise, the value itself is returned.

    WARNING: In order for the function to work properly, _LowerBound must be less than or equal to _UpperBound.

Arguments:
    xVal:               The value to potentially clamp.
    xLowerBound:        The smallest that _Val can be.
    xUpperBound:        The largest that _Val can be.

Returns:
    T:                  A value between _LowerBound and _UpperBound (inclusive).
*/
template< typename T >
inline T ClampedValue( const T& xVal, const T& xLowerBound, const T& xUpperBound )
{
    if( xVal > xUpperBound )
        return xUpperBound;
    else if( xVal < xLowerBound )
        return xLowerBound;
    
    return xVal;
}

/*
LowClamp
-----------------------

Description:
    Takes a value and a lower bound.
    If the value is below the lower bound, it is replaced with the lower bound.

Arguments:
    xValInOut:          The value to potentially clamp.
    xLowerBound:        The smallest that _ValInOut can be.

Returns:
    void:               N/A
*/
template< typename T >
inline void LowClamp( T& xValInOut, const T& xLowerBound )
{
    if( xValInOut < xLowerBound )
        xValInOut = xLowerBound;
}

/*
LowClampedValue
-----------------------

Description:
    Takes a value and a lower bound.
    If the value is below the lower bound, the lower bound is returned.
    Otherwise, the value itself is returned.

Arguments:
    _Val:               The value to potentially clamp.
    _LowerBound:        The smallest that _Val can be.

Returns:
    T:                  A value no larger than _LowerBound.
*/
template <typename T>
inline T LowClampedValue( const T& xVal, const T& xLowerBound )
{
    if( xVal < xLowerBound )
        return xLowerBound;

    return xVal;
}

/*
HighClamp
-----------------------

Description:
    Takes a value and an upper bound.
    If the value is above the upper bound, it is replaced with the upper bound.

Arguments:
    _ValInOut:          The value to potentially clamp.
    _UpperBound:        The largest that _ValInOut can be.

Returns:
    void:               N/A
*/
template <typename T>
inline void HighClamp( T& xValInOut, const T& xUpperBound )
{
    if( xValInOut > xUpperBound )
        xValInOut = xUpperBound;
}

/*
HighClampedValue
-----------------------

Description:
    Takes a value and a higher bound.
    If the value is above the upper bound, the upper bound is returned.
    Otherwise, the value itself is returned.

Arguments:
    _Val:               The value to potentially clamp.
    _UpperBound:        The largest that _Val can be.

Returns:
    T:                  A value no larger than _UpperBound.
*/
template <typename T>
inline T HighClampedValue( const T& xVal, const T& xUpperBound )
{
    if( xVal > xUpperBound )
        return xUpperBound;

    return xVal;
}

}




#endif //BS_UTIL_RANGE_HPP