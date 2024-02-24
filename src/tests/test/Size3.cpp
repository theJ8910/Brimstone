/*
test/Size3.cpp
--------------
Copyright (c) 2024, theJ89

Description:
    Unit tests for Size3
*/




//Include
#include <sstream>              //std::ostringstream

#include "../Test.hpp"          //UT_TEST_BEGIN, UT_TEST_END
#include "../utils.hpp"         //allEqual, allEqualTo, copyAll, isWithin, allWithin, FAST_SQRT_ERR

#include <brimstone/Size.hpp>




namespace {
    using ::Brimstone::Size3i;
    using ::Brimstone::Size3f;
    using ::Brimstone::BoundsException;

    const size_t cv_size         = 3;
    const int    cv_zero[3]      { 0, 0, 0 };
    const int    cv_values[3]    { 1, 2, 3 };
    const int    cv_valuesAlt[3] { 4, 5, 6 };
    const int    cv_volume       = 4 * 5 * 6;
    const char*  cv_output       = "[ 1, 2, 3 ]";

    const float  cv_valuesF[3]   { 1.0f, 2.0f, 3.0f };
    const char*  cv_outputF      = "[ 1.00000, 2.00000, 3.00000 ]";
}

namespace UnitTest {

UT_TEST_BEGIN( Size3_constructorFill )
    Size3i o( 1 );

    return allEqualTo( o.data, 1 );
UT_TEST_END()

UT_TEST_BEGIN( Size3_constructorCppRange )
    Size3i o( cv_values );

    return allEqual( o.data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( Size3_constructorInitializerList )
    Size3i o( { cv_values[0], cv_values[1], cv_values[2], cv_values[3] } );

    return allEqual( o.data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( Size3_setCppRange )
    Size3i o( cv_values );

    o.set( cv_valuesAlt );

    return allEqual( o.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( Size3_setInitializerList )
    Size3i o( cv_values );

    o.set( { cv_valuesAlt[0], cv_valuesAlt[1], cv_valuesAlt[2], cv_valuesAlt[3] } );

    return allEqual( o.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( Size3_getCppRange )
    int values[ cv_size ];
    copyAll( cv_values, values );
    Size3i o( cv_valuesAlt );

    o.get( values );

    return allEqual( o.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( Size3_fill )
    Size3i o( cv_values );

    o.fill( 1 );

    return allEqualTo( o.data, 1 );
UT_TEST_END()

UT_TEST_BEGIN( Size3_begin )
    Size3i o( cv_values );

    return o.begin() == std::begin( o.data );
UT_TEST_END()

UT_TEST_BEGIN( Size3_beginConst )
    const Size3i o( cv_values );

    return o.begin() == std::begin( o.data );
UT_TEST_END()

UT_TEST_BEGIN( Size3_cbegin )
    Size3i o( cv_values );

    return o.cbegin() == std::begin( o.data );
UT_TEST_END()

UT_TEST_BEGIN( Size3_end )
    Size3i o( cv_values );

    return o.end() == std::end( o.data );
UT_TEST_END()

UT_TEST_BEGIN( Size3_endConst )
    const Size3i o( cv_values );

    return o.end() == std::end( o.data );
UT_TEST_END()

UT_TEST_BEGIN( Size3_cend )
    Size3i o( cv_values );

    return o.cend() == std::end( o.data );
UT_TEST_END()

UT_TEST_BEGIN( Size3_rangedFor )
    Size3i o( cv_values );

    int idx = 0;
    for( int& i : o )
        i = cv_valuesAlt[idx++];

    return allEqual( o.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( Size3_rangedForConst )
    int data[ cv_size ];
    copyAll( cv_values, data );
    const Size3i o( cv_valuesAlt );

    int idx = 0;
    for( int i : o )
        data[idx++] = i;

    return allEqual( data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( Size3_index )
    Size3i o( cv_values );

    for( size_t i = 0; i < cv_size; ++i )
        o[i] = cv_valuesAlt[i];

    return allEqual( o.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( Size3_indexConst )
    int data[ cv_size ];
    const Size3i o( cv_values );

    for( size_t i = 0; i < cv_size; ++i )
        data[i] = o[i];

    return allEqual( data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( Size3_constructorValues )
    Size3i pt( cv_values[0], cv_values[1], cv_values[2] );

    return allEqual( pt.data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( Size3_constructorCopy )
    Size3f o1( cv_valuesF );

    Size3i o2( o1 );

    return allEqual( o2.data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( Size3_setValues )
    Size3i o( cv_values );

    o.set( cv_valuesAlt[0], cv_valuesAlt[1], cv_valuesAlt[2] );

    return allEqual( o.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( Size3_getValues )
    Size3i o( cv_values );

    int width, length, height;
    o.get( width, length, height );

    return width  == cv_values[0] &&
           length == cv_values[1] &&
           height == cv_values[2];
UT_TEST_END()

UT_TEST_BEGIN( Size3_zero )
    Size3i o( cv_values );

    o.zero();

    return allEqual( o.data, cv_zero );
UT_TEST_END()

UT_TEST_BEGIN( Size3_isZero )
    Size3i o1( cv_zero );
    Size3i o2( cv_values );

    return o1.isZero() == true &&
           o2.isZero() == false;
UT_TEST_END()

UT_TEST_BEGIN( Size3_assignCopy )
    Size3f o1( cv_valuesF );
    Size3i o2( cv_valuesAlt );

    o2 = o1;

    return allEqual( o2.data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( Size3_getVolume )
    Size3i b( cv_valuesAlt );

    return b.getVolume() == cv_volume;
UT_TEST_END()

UT_TEST_BEGIN( Size3_equals )
    Size3i o1( cv_values );
    Size3i o2( cv_values );
    Size3i o3( cv_valuesAlt );

    return ( o1 == o2 ) == true   &&
           ( o1 == o3 ) == false;
UT_TEST_END()

UT_TEST_BEGIN( Size3_notEquals )
    Size3i o1( cv_values );
    Size3i o2( cv_values );
    Size3i o3( cv_valuesAlt );

    return ( o1 != o2 ) == false  &&
           ( o1 != o3 ) == true;
UT_TEST_END()

UT_TEST_BEGIN( Size3_output_int )
    Size3i o( cv_values );

    std::stringstream sout;
    sout << o;

    return sout.str() == cv_output;
UT_TEST_END()

UT_TEST_BEGIN( Size3_output_float )
    Size3f o( cv_valuesF );

    std::stringstream sout;
    sout << o;

    return sout.str() == cv_outputF;
UT_TEST_END()




#ifdef BS_ZERO

UT_TEST_BEGIN( Size3_constructorZero )
    Size3i o;
    return allEqual( o.data, cv_zero );
UT_TEST_END()

#endif //BS_ZERO




#ifdef BS_CHECK_INDEX

UT_TEST_BEGIN( Size3_index_OOB )
    Size3i o;

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

UT_TEST_BEGIN( Size3_constIndex_OOB )
    const Size3i o;

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
