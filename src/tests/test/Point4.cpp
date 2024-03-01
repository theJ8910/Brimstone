/*
test/Point4.cpp
---------------
Copyright (c) 2024, theJ89

Description:
    Unit tests for PointN<T,4> specialization
*/




//Includes
#include "../Test.hpp"          //UT_TEST_BEGIN, UT_TEST_END
#include "../utils.hpp"         //allEqual, allEqualTo, copyAll

#include <cstddef>              //std::size_t
#include <sstream>              //std::ostringstream

#include <brimstone/Point.hpp>
#include <brimstone/Vector.hpp> //Vector




namespace {
    using ::Brimstone::Point4i;
    using ::Brimstone::Point4f;
    using ::Brimstone::Vector4i;
    using ::Brimstone::BoundsException;

    const std::size_t cv_size         = 4;
    const int         cv_zero[4]      {  0,  0,  0,  0 };
    const int         cv_values[4]    {  1,  2,  3,  4 };
    const int         cv_valuesAlt[4] {  5,  6,  7,  8 };
    const int         cv_distant[4]   { -8,  7, -6,  5 };
    const int         cv_distanceSq   = 188;
    const int         cv_manhattan    = 24;
    const char*       cv_output       = "( 1, 2, 3, 4 )";

    const float       cv_valuesF[4]   { 1.0f, 2.0f, 3.0f, 4.0f };
    const char*       cv_outputF      = "( 1.00000, 2.00000, 3.00000, 4.00000 )";
}

namespace UnitTest {

UT_TEST_BEGIN( Point4_constructorFill )
    Point4i o( 1 );

    return allEqualTo( o.data, 1 );
UT_TEST_END()

UT_TEST_BEGIN( Point4_constructorCppRange )
    Point4i o( cv_values );

    return allEqual( o.data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( Point4_constructorInitializerList )
    Point4i o( { cv_values[0], cv_values[1], cv_values[2], cv_values[3] } );

    return allEqual( o.data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( Point4_setCppRange )
    Point4i o( cv_values );

    o.set( cv_valuesAlt );

    return allEqual( o.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( Point4_setInitializerList )
    Point4i o( cv_values );

    o.set( { cv_valuesAlt[0], cv_valuesAlt[1], cv_valuesAlt[2], cv_valuesAlt[3] } );

    return allEqual( o.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( Point4_getCppRange )
    int values[ cv_size ];
    copyAll( cv_values, values );
    Point4i o( cv_valuesAlt );

    o.get( values );

    return allEqual( o.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( Point4_fill )
    Point4i o( cv_values );

    o.fill( 1 );

    return allEqualTo( o.data, 1 );
UT_TEST_END()

UT_TEST_BEGIN( Point4_begin )
    Point4i o( cv_values );

    return o.begin() == std::begin( o.data );
UT_TEST_END()

UT_TEST_BEGIN( Point4_beginConst )
    const Point4i o( cv_values );

    return o.begin() == std::begin( o.data );
UT_TEST_END()

UT_TEST_BEGIN( Point4_cbegin )
    Point4i o( cv_values );

    return o.cbegin() == std::begin( o.data );
UT_TEST_END()

UT_TEST_BEGIN( Point4_end )
    Point4i o( cv_values );

    return o.end() == std::end( o.data );
UT_TEST_END()

UT_TEST_BEGIN( Point4_endConst )
    const Point4i o( cv_values );

    return o.end() == std::end( o.data );
UT_TEST_END()

UT_TEST_BEGIN( Point4_cend )
    Point4i o( cv_values );

    return o.cend() == std::end( o.data );
UT_TEST_END()

UT_TEST_BEGIN( Point4_rangedFor )
    Point4i o( cv_values );

    int idx = 0;
    for( int& i : o )
        i = cv_valuesAlt[idx++];

    return allEqual( o.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( Point4_rangedForConst )
    int data[ cv_size ];
    copyAll( cv_values, data );
    const Point4i o( cv_valuesAlt );

    int idx = 0;
    for( int i : o )
        data[idx++] = i;

    return allEqual( data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( Point4_index )
    Point4i o( cv_values );

    for( std::size_t i = 0; i < cv_size; ++i )
        o[i] = cv_valuesAlt[i];

    return allEqual( o.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( Point4_indexConst )
    int data[ cv_size ];
    const Point4i o( cv_values );

    for( std::size_t i = 0; i < cv_size; ++i )
        data[i] = o[i];

    return allEqual( data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( Point4_constructorValues )
    Point4i pt( cv_values[0], cv_values[1], cv_values[2], cv_values[3] );

    return allEqual( pt.data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( Point4_constructorCopy )
    Point4f o1( cv_valuesF );

    Point4i o2( o1 );

    return allEqual( o2.data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( Point4_setValues )
    Point4i o( cv_values );

    o.set( cv_valuesAlt[0], cv_valuesAlt[1], cv_valuesAlt[2], cv_valuesAlt[3] );

    return allEqual( o.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( Point4_getValues )
    Point4i o( cv_values );

    int x, y, z, w;
    o.get( x, y, z, w );

    return x == cv_values[0] &&
           y == cv_values[1] &&
           z == cv_values[2] &&
           w == cv_values[3];
UT_TEST_END()

UT_TEST_BEGIN( Point4_zero )
    Point4i o( cv_values );

    o.zero();

    return allEqual( o.data, cv_zero );
UT_TEST_END()

UT_TEST_BEGIN( Point4_isZero )
    Point4i o1( cv_zero );
    Point4i o2( cv_values );

    return o1.isZero() == true &&
           o2.isZero() == false;
UT_TEST_END()

UT_TEST_BEGIN( Point4_assignCopy )
    Point4f o1( cv_valuesF );
    Point4i o2( cv_valuesAlt );

    o2 = o1;

    return allEqual( o2.data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( Point4_typecastVector )
    Point4i o( cv_values );
    Vector4i v( cv_valuesAlt );

    v = (Vector4i&)o;

    return allEqual( o.data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( Point4_typecastVector_const )
    const Point4i o( cv_values );
    Vector4i v( cv_valuesAlt );

    v = (const Vector4i&)o;

    return allEqual( o.data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( Point4_equals )
    Point4i o1( cv_values );
    Point4i o2( cv_values );
    Point4i o3( cv_valuesAlt );

    return ( o1 == o2 ) == true   &&
           ( o1 == o3 ) == false;
UT_TEST_END()

UT_TEST_BEGIN( Point4_notEquals )
    Point4i o1( cv_values );
    Point4i o2( cv_values );
    Point4i o3( cv_valuesAlt );

    return ( o1 != o2 ) == false  &&
           ( o1 != o3 ) == true;
UT_TEST_END()

UT_TEST_BEGIN( Point4_output_int )
    Point4i o( cv_values );

    std::stringstream sout;
    sout << o;

    return sout.str() == cv_output;
UT_TEST_END()

UT_TEST_BEGIN( Point4_output_float )
    Point4f o( cv_valuesF );

    std::stringstream sout;
    sout << o;

    return sout.str() == cv_outputF;
UT_TEST_END()

UT_TEST_BEGIN( Point4_distanceSq )
    Point4i o1( cv_values );
    Point4i o2( cv_distant );

    return distanceSq( o1, o2 ) == cv_distanceSq;
UT_TEST_END()

UT_TEST_BEGIN( Point4_manhattan )
    Point4i o1( cv_values );
    Point4i o2( cv_distant );

    return manhattan( o1, o2 ) == cv_manhattan;
UT_TEST_END()




#ifdef BS_ZERO

UT_TEST_BEGIN( Point4_constructorZero )
    Point4i o;
    return allEqual( o.data, cv_zero );
UT_TEST_END()

#endif //BS_ZERO




#ifdef BS_CHECK_INDEX

UT_TEST_BEGIN( Point4_index_OOB )
    Point4i o;

    try {
        o[(std::size_t)-1];
        return false;
    } catch( const BoundsException& ) {}

    try {
        o[4];
        return false;
    } catch( const BoundsException& ) {}

    return true;
UT_TEST_END()

UT_TEST_BEGIN( Point4_constIndex_OOB )
    const Point4i o;

    try {
        o[(std::size_t)-1];
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
