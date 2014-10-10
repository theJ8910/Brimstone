/*
test/Size4.cpp
-----------------------
Copyright (c) 2014, theJ89

Description:
    Unit tests for Size4
*/




//Include
#include <sstream>              //std::ostringstream

#include "../Test.hpp"          //UT_TEST_BEGIN, UT_TEST_END
#include "../utils.hpp"         //allEqual, allEqualTo, copyAll, isWithin, allWithin, FAST_SQRT_ERR

#include <brimstone/Size.hpp>




namespace {
    using ::Brimstone::Size4i;
    using ::Brimstone::Size4f;
    using ::Brimstone::BoundsException;

    const size_t cv_size         = 4;
    const int    cv_zero[4]      {  0,  0,  0,  0 };
    const int    cv_values[4]    {  1,  2,  3,  4 };
    const int    cv_valuesAlt[4] {  5,  6,  7,  8 };
    const int    cv_volume       = 5 * 6 * 7 * 8;
    const char*  cv_output       = "[ 1, 2, 3, 4 ]";

    const float  cv_valuesF[4]   { 1.0f, 2.0f, 3.0f, 4.0f };
    const char*  cv_outputF      = "[ 1.00000, 2.00000, 3.00000, 4.00000 ]";
}

namespace UnitTest {

UT_TEST_BEGIN( Size4_constructorFill )
    Size4i o( 1 );

    return allEqualTo( o.data, 1 );
UT_TEST_END()

UT_TEST_BEGIN( Size4_constructorCppRange )
    Size4i o( cv_values );

    return allEqual( o.data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( Size4_constructorInitializerList )
    Size4i o( { cv_values[0], cv_values[1], cv_values[2], cv_values[3] } );

    return allEqual( o.data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( Size4_setCppRange )
    Size4i o( cv_values );

    o.set( cv_valuesAlt );

    return allEqual( o.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( Size4_setInitializerList )
    Size4i o( cv_values );

    o.set( { cv_valuesAlt[0], cv_valuesAlt[1], cv_valuesAlt[2], cv_valuesAlt[3] } );

    return allEqual( o.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( Size4_getCppRange )
    int values[ cv_size ];
    copyAll( cv_values, values );
    Size4i o( cv_valuesAlt );

    o.get( values );

    return allEqual( o.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( Size4_fill )
    Size4i o( cv_values );

    o.fill( 1 );

    return allEqualTo( o.data, 1 );
UT_TEST_END()

UT_TEST_BEGIN( Size4_begin )
    Size4i o( cv_values );

    return o.begin() == std::begin( o.data );
UT_TEST_END()

UT_TEST_BEGIN( Size4_beginConst )
    const Size4i o( cv_values );

    return o.begin() == std::begin( o.data );
UT_TEST_END()

UT_TEST_BEGIN( Size4_cbegin )
    Size4i o( cv_values );

    return o.cbegin() == std::begin( o.data );
UT_TEST_END()

UT_TEST_BEGIN( Size4_end )
    Size4i o( cv_values );

    return o.end() == std::end( o.data );
UT_TEST_END()

UT_TEST_BEGIN( Size4_endConst )
    const Size4i o( cv_values );

    return o.end() == std::end( o.data );
UT_TEST_END()

UT_TEST_BEGIN( Size4_cend )
    Size4i o( cv_values );

    return o.cend() == std::end( o.data );
UT_TEST_END()

UT_TEST_BEGIN( Size4_rangedFor )
    Size4i o( cv_values );

    int idx = 0;
    for( int& i : o )
        i = cv_valuesAlt[idx++];

    return allEqual( o.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( Size4_rangedForConst )
    int data[ cv_size ];
    copyAll( cv_values, data );
    const Size4i o( cv_valuesAlt );

    int idx = 0;
    for( int i : o )
        data[idx++] = i;

    return allEqual( data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( Size4_index )
    Size4i o( cv_values );

    for( size_t i = 0; i < cv_size; ++i )
        o[i] = cv_valuesAlt[i];

    return allEqual( o.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( Size4_indexConst )
    int data[ cv_size ];
    const Size4i o( cv_values );

    for( size_t i = 0; i < cv_size; ++i )
        data[i] = o[i];

    return allEqual( data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( Size4_constructorValues )
    Size4i pt( cv_values[0], cv_values[1], cv_values[2], cv_values[3] );

    return allEqual( pt.data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( Size4_constructorCopy )
    Size4f o1( cv_valuesF );

    Size4i o2( o1 );

    return allEqual( o2.data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( Size4_setValues )
    Size4i o( cv_values );

    o.set( cv_valuesAlt[0], cv_valuesAlt[1], cv_valuesAlt[2], cv_valuesAlt[3] );

    return allEqual( o.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( Size4_getValues )
    Size4i o( cv_values );

    int width, length, height, depth;
    o.get( width, length, height, depth );

    return width  == cv_values[0] &&
           length == cv_values[1] &&
           height == cv_values[2] &&
           depth  == cv_values[3];
UT_TEST_END()

UT_TEST_BEGIN( Size4_zero )
    Size4i o( cv_values );

    o.zero();

    return allEqual( o.data, cv_zero );
UT_TEST_END()

UT_TEST_BEGIN( Size4_isZero )
    Size4i o1( cv_zero );
    Size4i o2( cv_values );

    return o1.isZero() == true &&
           o2.isZero() == false;
UT_TEST_END()

UT_TEST_BEGIN( Size4_assignCopy )
    Size4f o1( cv_valuesF );
    Size4i o2( cv_valuesAlt );

    o2 = o1;

    return allEqual( o2.data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( Size4_getVolume )
    Size4i b( cv_valuesAlt );

    return b.getVolume() == cv_volume;
UT_TEST_END()

UT_TEST_BEGIN( Size4_equals )
    Size4i o1( cv_values );
    Size4i o2( cv_values );
    Size4i o3( cv_valuesAlt );

    return ( o1 == o2 ) == true   &&
           ( o1 == o3 ) == false;
UT_TEST_END()

UT_TEST_BEGIN( Size4_notEquals )
    Size4i o1( cv_values );
    Size4i o2( cv_values );
    Size4i o3( cv_valuesAlt );

    return ( o1 != o2 ) == false  &&
           ( o1 != o3 ) == true;
UT_TEST_END()

UT_TEST_BEGIN( Size4_output_int )
    Size4i o( cv_values );

    std::stringstream sout;
    sout << o;

    return sout.str() == cv_output;
UT_TEST_END()

UT_TEST_BEGIN( Size4_output_float )
    Size4f o( cv_valuesF );

    std::stringstream sout;
    sout << o;

    return sout.str() == cv_outputF;
UT_TEST_END()




#ifdef BS_ZERO

UT_TEST_BEGIN( Size4_constructorZero )
    Size4i o;
    return allEqual( o.data, cv_zero );
UT_TEST_END()

#endif //BS_ZERO




#ifdef BS_CHECK_INDEX

UT_TEST_BEGIN( Size4_index_OOB )
    Size4i o;

    try {
        o[(size_t)-1];
        return false;
    } catch( const BoundsException& ) {}

    try {
        o[4];
        return false;
    } catch( const BoundsException& ) {}

    return true;
UT_TEST_END()

UT_TEST_BEGIN( Size4_constIndex_OOB )
    const Size4i o;

    try {
        o[(size_t)-1];
        return false;
    } catch( const BoundsException& ) {}

    try {
        o[4];
        return false;
    } catch( const BoundsException& ) {}

    return true;
UT_TEST_END()

#endif //BS_CHECK_INDEX




}