/*
utils.hpp
-----------------------
Copyright (c) 2014, theJ89

Description:
    Shared utilities that unit tests use.
*/
#ifndef UT_UTILS_HPP
#define UT_UTILS_HPP




//Includes
#include <algorithm>            //std::equal
#include <iterator>             //std::begin, std::end
#include <type_traits>          //std::is_same
#include <cassert>              //assert




//Macros
#define UT_TMPL_1( param1                                 )  template< param1                                 >
#define UT_TMPL_2( param1, param2                         )  template< param1, param2                         >
#define UT_TMPL_3( param1, param2, param3                 )  template< param1, param2, param3                 >
#define UT_TMPL_4( param1, param2, param3, param4         )  template< param1, param2, param3, param4         >
#define UT_TMPL_5( param1, param2, param3, param4, param5 )  template< param1, param2, param3, param4, param5 >

#define UT_SPEC_1( arg1                                   )  < arg1                                           >
#define UT_SPEC_2( arg1, arg2                             )  < arg1, arg2                                     >
#define UT_SPEC_3( arg1, arg2, arg3                       )  < arg1, arg2, arg3                               >
#define UT_SPEC_4( arg1, arg2, arg3, arg4                 )  < arg1, arg2, arg3, arg4                         >
#define UT_SPEC_5( arg1, arg2, arg3, arg4, arg5           )  < arg1, arg2, arg3, arg4, arg5                   >




namespace UnitTest {

//Constants
extern const float FAST_SQRT_ERROR;

//Forward declarations
bool isWithin( const float value, const float ideal, const float err );
bool allWithin( const float* values, const float* ideals, const float err, const int size );

//Functions
//rangeSize{1}
//Returns the number of elements in a container
template< typename T >
size_t rangeSize( const T& range ) {
    return (size_t)( std::end( range ) - std::begin( range ) );
}

//assertSizeDiff{1}
//Checks the sizes of two given containers.
//If left > right, the assertion fails.
template< typename TL, typename TR >
void assertSizeDiff( const TL& left, const TR& right ) {
    //NOTE: This code makes an assumption that the number of elements in the range between between
    //      two iterators can be calculated by subtracting the larger pointer from the smaller.
    //This is a run-time assertion because we can't determine the sizes at compile time
    assert( rangeSize( left ) <= rangeSize( right ) );
}

//assertSizeDiff{2}
//Same as above but specializes for arrays.
//Since the size of an array is known at compile time, it can use static_assert instead of assert.
template< typename T, size_t NL, size_t NR >
void assertSizeDiff( const T (&/* left */)[NL], const T(&/* right */)[NR] ) {
    static_assert( NL <= NR, "Right array is smaller than left array!" );
}

//allEqual
//Takes two iterables of the same type, "left" (of size NL) and "right" (of size NR).
//Returns true if left[i] == right[i] for each i, where 0 <= i < NL. Returns false otherwise.
//NL must be greater than or equal to NR, or compilation will fail.
template< typename TL, typename TR >
bool allEqual( const TL& left, const TR& right ) {
    assertSizeDiff( left, right );

    return std::equal(
        std::begin( left  ),
        std::end(   left  ),
        std::begin( right )
    );
}

//allEqualTo
//Same as allEqual{1}, but instead takes a single value on the right instead of an array.
//Returns true if left[i] == right for each i, where 0 <= i < NL. Returns false otherwise.
template< typename TL, typename TR >
bool allEqualTo( const TL& left, const TR& right ) {
    return std::all_of(
        std::begin( left ),
        std::end(   left ),
        [right]( const TR& in ){ return in == right; }
    );
}

//copyAll{1}
//Takes two arrays of the same type, "from" (of size NF) and "to" (of size NT).
//Sets to[i] = from[i] for each i, where 0 <= i < NF.
//NT must be greater than or equal to NF, or compilation will fail.
template< typename T, size_t NF, size_t NT >
void copyAll( const T (&from)[NF], T(&to)[NT] ) {
    static_assert( NF <= NT, "Destination array is smaller than source array!" );
    std::copy(
        std::begin( from ),
        std::end(   from ),
                    to
    );
}

//copyAll{2}
//Input pointer, output array
template< typename InPtr, typename T, size_t NT >
void copyAll( InPtr from, T (&to)[NT] ) {
    static_assert(
        std::is_pointer< InPtr >::value &&
        std::is_same<
            const T,
            std::remove_pointer< InPtr >::type
        >::value,
        "First parameter to copyAll is an invalid type"
    );

    std::copy(
        from,
        from + NT,
        to
    );
}

//copyAll{3}
//Input array, output pointer
template< typename T, size_t NF, typename OutPtr >
void copyAll( const T (&from)[NF], OutPtr to ) {
    static_assert(
        std::is_pointer< OutPtr >::value &&
        std::is_same<
            T,
            std::remove_pointer< OutPtr >::type
        >::value,
        "Second parameter to copyAll is an invalid type"
    );

    std::copy(
        std::begin( from ),
        std::end(   from ),
                    to
    );
}



}




#endif //UT_UTILS_HPP