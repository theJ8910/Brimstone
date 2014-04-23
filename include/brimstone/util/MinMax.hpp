/*
MinMax.hpp
-----------------------
Copyright (c) 2014, theJ89

Description:
    Defines several functions for taking the minimum, maximum,
    or both minimum and maximum of two or more numbers.
*/

#ifndef BS_MINMAX_HPP
#define BS_MINMAX_HPP




//Includes
#include <utility>          //std::swap
#include "../Exception.hpp" //NullPointerException, SizeException
#include "../types.hpp"     //uint32

//Defines
//#define BS_MINMAX_CHECKNULLPTR    //If this is declared, the minmax functions make sure that pointers given as parameters to functions that are required to be non-NULL actually are non-NULL.
//#define BS_MINMAX_CHECKSIZE       //If this is declared, the minmax functions make sure that functions that require nonzero array sizes are given nonzero sizes.




namespace Brimstone {

//Math
//Faster ------------------------------------------------------------------------------------------------------------------------------------------ Slower

//Statistics
template <typename T>   inline void         electMin( T& xCurrentInWinnerOut, const T& xCandidate );
template <typename T>   inline void         min( const T& xA, const T& xB, T& xMinOut );                                                            template <typename T>   inline T        min( const T& xA, const T& xB );
template <typename T>   inline void         min( const T* paxValues, uint32 uiNumItems, T& xMinOut );

template <typename T>   inline void         electMax( T& xCurrentInWinnerOut, const T& _Candidate );
template <typename T>   inline void         max( const T& xA, const T& xB, T& xMaxOut );                                                            template <typename T>   inline T        max( const T& xA, const T& xB );
template <typename T>   inline void         max( const T* paxValues, uint32 uiNumItems, T& xMaxOut );

template <typename T>   inline void         electMinMax( T& xMinInOut, T& xMaxInOut, const T& xCandidate );
template <typename T>   inline void         minMax( const T& xA, const T& xB, T& xMinOut, T& xMaxOut );                                             template <typename T>   inline void     minMax( T& xAInMinOut, T& xBInMaxOut );
template <typename T>   inline void         minMax( const T* paxValues, uint32 uiNumItems, T& xMinOut, T& xMaxOut );


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
        int iMin = 10;
        int iA  = 20;
        int iB  = 5;
        electMin( iMin, iA );
        electMin( iMin, iB );

Arguments:
    xCurrentInWinnerOut:    The current value. Can potentially be replaced by xCandidate.
    xCandidate:             A potentially smaller value.

Returns:
    void:                   N/A
*/
template<typename T>
inline void electMin( T& xCurrentInWinnerOut, const T& xCandidate )
{
    if( xCandidate < xCurrentInWinnerOut )
        xCurrentInWinnerOut = xCandidate;
}

/*
min
-----------------------

Description:
    Takes two values, compares them, and sets _MinOut to the smaller value.

    Usage:
        int a = 1;
        int b = 2;
        int smallest;
        Min( a, b, smallest );

Arguments:
    xA:         A value.
    xB:         Another value.
    xMaxOut:    A given variable set to the smaller of the two values.

Returns:
    void:       N/A
*/
template<typename T>
inline void min( const T& xA, const T& xB, T& xMinOut )
{
    xMinOut = ( ( xA < xB ) ? xA : xB );
}

/*
min
-----------------------

Description:
    Takes two values, compares them, and returns the smaller value.
    NOTE: This is less efficient to use than the above function.

    Usage:
        int a = 1;
        int b = 2;
        int smallest = Min( a, b );

Arguments:
    xA:     A value.
    xB:     Another value.

Returns:
    T:      The smaller value.
*/
template<typename T>
inline T min( const T& xA, const T& xB )
{
    return ( ( xA < xB ) ? xA : xB );
}

/*
min
-----------------------

Description:
    Finds the smallest element (minimum) in an array of values.

    Usage:
        int arr[] = { 162, 26, 31, 34, 255, 64, 7 }
        int smallest;
        min( arr, 7, smallest );

Arguments:
    paxValues:      Pointer to an array of values to find the smallest element of.
                    Cannot be nullptr.
    uiNumItems:     The number of items in the array.
                    Must be at least 1.
    xMinOut:        Will contain the smallest value in the array after the function runs.

Returns:
    void:           N/A
*/
template<typename T>
inline void min( const T* paxValues, uint32 uiNumItems, T& xMinOut )
{

#ifdef BS_MINMAX_CHECKNULLPTR

    if( _paValues == nullptr ) {
        throw NullPointerException();
    }

#endif //BS_MINMAX_CHECKNULLPTR

#ifdef BS_MINMAX_CHECKSIZE

    if( uiNumItems == 0 ) {
        throw SizeException();
    }

#endif //BS_MINMAX_CHECKSIZE

    xMinOut = *paxValues;
    while( ( --uiNumItems ) > 0u ) {
        if( *(++paxValues) < xMinOut )
            xMinOut = *paxValues;
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
        int iMax = 10;
        int iA   = 20;
        int iB   = 5;
        electMax( iMax, iA );   //iMax is now 20
        electMax( iMax, iB );   //iMax is still 20

Arguments:
    xCurrentInWinnerOut:    The current value. Can potentially be replaced by _Candidate.
    xCandidate:             A potentially larger value.

Returns:
    void:                   N/A
*/
template<typename T>
inline void electMax( T& xCurrentInWinnerOut, const T& xCandidate )
{
    if( xCandidate > xCurrentInWinnerOut )
        xCurrentInWinnerOut = xCandidate;
}

/*
max
-----------------------

Description:
    Takes two values, compares them, and sets _MaxOut to the larger value.
    
    Usage:
        int a = 1;
        int b = 2;
        int largest;
        max( a, b, largest );

Arguments:
    xA:         A value.
    xB:         Another value.
    xMaxOut:    A given variable set to the larger of the two values.

Returns:
    void:       N/A
*/
template<typename T>
inline void max( const T& xA, const T& xB, T& xMaxOut )
{
    xMaxOut = ( ( xA > xB ) ? xA : xB );
}

/*
max
-----------------------

Description:
    Takes two values, compares them, and returns the larger value.

    Usage:
        int a = 1;
        int b = 2;
        int largest = max( a, b );

Arguments:
    xA:     A value.
    xB:     Another value.

Returns:
    T:      The larger value.
*/
template<typename T>
inline T max( const T& xA, const T& xB )
{
    return ( ( xA > xB ) ? xA : xB );
}

/*
max
-----------------------

Description:
    Finds the largest element (maximum) in an array of values.

    Usage:
        int arr[] = { 162, 26, 31, 34, 255, 64, 7 }
        int maximum;
        max( arr, 7, maximum );

Arguments:
    paxValues:      Pointer to an array of values to find the largest element of.
                    Cannot be nullptr.
    uiNumItems:     The number of items in the array.
                    Must be at least 1.
    xMaxOut:        Will contain the largest value in the array after the function runs.

Returns:
    void:           N/A
*/
template<typename T>
inline void max( const T* paxValues, uint32 uiNumItems, T& xMaxOut )
{

#ifdef BS_MINMAX_CHECKNULLPTR

    if( paxValues == nullptr ) {
        throw NullPointerException();
    }

#endif //BS_MINMAX_CHECKNULLPTR

#ifdef BS_MINMAX_CHECKSIZE

    if( uiNumItems == 0 ) {
        throw SizeException();
    }

#endif //BS_MINMAX_CHECKSIZE

    xMaxOut = *paxValues;
    while( ( --uiNumItems ) > 0u ) {
        if( *( ++paxValues ) > xMaxOut )
            xMaxOut = *paxValues;
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
    _MinInOut is expected to be less than or equal to _MaxInOut prior to calling this function.

    Usage:
        int iMin = 10
        int iMax = 15;

        int iA   = 20;
        int iB   = 5;
        int iC   = 30
        
        electMinMax( iMin, iMax, iA );
        electMinMax( iMin, iMax, iB );
        electMinMax( iMin, iMax, iC );

Arguments:
    xMinInOut:      Currently the smallest value. Can potentially be replaced by _Candidate.
    xMaxInOut:      Currently the largest value. Can potentially be replaced by _Candidate.
    xCandidate:     A value potentially larger or smaller than _Candidate.

Returns:
    void:           N/A
*/
template<typename T>
inline void electMinMax( T& xMinInOut, T& xMaxInOut, const T& xCandidate )
{
    if( xCandidate > xMaxInOut ) {
        xMaxInOut = xCandidate;
    } else if( xCandidate < xMinInOut ) {
        xMinInOut = xCandidate;
    }
}

/*
minMax
-----------------------

Description:
    Takes two values, compares them, and sets _MinOut to the smaller value,
    and _MaxOut to the larger value.

    Usage:
        int a = 1;
        int b = 2;
        int smallest
        int largest;
        minMax( a, b, smallest, largest );

Arguments:
    xA:         A value.
    xB:         Another value.
    xMinOut:    A given variable set to the smaller of the two values.
    xMaxOut:    A given variable set to the larger of the two values.

Returns:
    void:       N/A
*/
template <typename T>
inline void minMax( const T& xA, const T& xB, T& xMinOut, T& xMaxOut )
{
    if( xA > xB ) {
        xMinOut = xB;
        xMaxOut = xA;
    } else {
        xMinOut = xA;
        xMaxOut = xB;
    }
}

/*
minMax
-----------------------

Description:
    Takes two values, compares them, and if necessary swaps
    them so that xAInMinOut will be the minimum value,
    and xBInMaxOut will be the maximum value.

    If the items are already in order, this function is more efficient than
    running MinMax( xAInMinOut, xBInMaxOut, xAInMinOut, xBInMaxOut ).

    However, if the items are out of order, this function is less efficient than the above function.
    That being said, this function is best used for things that are likely to be in order already (if this is possible to know, of course).
    
    Overall, the difference in the efficiency of the two functions isn't huge, so don't worry about it too much.

    Usage:
        int a = 2;
        int b = 1;
        minMax( a, b );

Arguments:
    xAInMinOut:     A reference to an existing variable holding the first value.
                    Will contain the smaller of the two values after the function runs.
    xBInMaxOut:     A reference to an existing variable holding the second value.
                    Will contain the larger of the two values after the function runs.

Returns:
    void:           N/A
*/
template <typename T>
inline void minMax( T& xAInMinOut, T& xBInMaxOut )
{
    if( xAInMinOut > xBInMaxOut )
        std::swap( xAInMinOut, xBInMaxOut );
}

/*
minMax
-----------------------

Description:
    Finds the smallest (min) and largest (max) element in an array of values.

    Usage:
        int arr[] = { 162, 26, 31, 34, 255, 64, 7 }
        int smallest;
        int largest;
        minMax( arr, 7, smallest, largest );

Arguments:
    _paValues:      Pointer to an array of values to find the min/max of.
                    Cannot be nullptr.
    uiNumItems:     The number of items in the array.
                    Must be at least 1.
    _MinOut:        Will contain the smallest value in the array after the function runs.
    _MaxOut:        Will contain the largest value in the array after the function runs.

Returns:
    void:           N/A
*/
template<typename T>
inline void minMax( const T* paxValues, uint32 uiNumItems, T& xMinOut, T& xMaxOut )
{

#ifdef BS_MINMAX_CHECKNULLPTR

    if( paxValues == nullptr ) {
        throw NullPointerException();
    }

#endif //BS_MINMAX_CHECKNULLPTR

#ifdef BS_MINMAX_CHECKSIZE

    if( uiNumItems == 0 ) {
        throw SizeException();
    }

#endif //BS_MINMAX_CHECKSIZE

    xMinOut = xMaxOut = *paxValues;
    while( (--uiNumItems) > 0u ) {
        ++paxValues;
        if( *paxValues > xMaxOut )
            xMaxOut = *paxValues;
        else if( *paxValues < xMinOut )
            xMinOut = *paxValues;
    }
}

}




#endif //BS_MINMAX_HPP