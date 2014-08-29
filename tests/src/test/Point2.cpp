/*
test/Point2.cpp
-----------------------
Copyright (c) 2014, theJ89

Description:
    Unit tests for PointN<T,2> specialization
*/




//Includes
#include <strstream>            //std::ostringstream

#include "../Test.hpp"          //UT_TEST_BEGIN, UT_TEST_END
#include "../utils.hpp"         //allEqual, allEqualTo, copyAll

#include <brimstone/Point.hpp>




namespace {
    using ::Brimstone::Point2i;
    using ::Brimstone::Point2f;
    using ::Brimstone::BoundsException;

    const size_t cv_size         = 2;
    const int    cv_zero[2]      {  0, 0 };
    const int    cv_values[2]    {  1, 2 };
    const int    cv_valuesAlt[2] {  3, 4 };
    const int    cv_distant[2]   { -4, 3 };
    const int    cv_distanceSq   = 26;
    const int    cv_manhattan    = 6;
    const char*  cv_output       = "( 1, 2 )";

    const float cv_valuesF[2]   { 1.0f, 2.0f };
    const char* cv_outputF      = "( 1.00000, 2.00000 )";
}

namespace UnitTest {

UT_TEST_BEGIN( Point2_constructorFill )
    Point2i o( 1 );

    return allEqualTo( o.data, 1 );
UT_TEST_END()

UT_TEST_BEGIN( Point2_constructorCppRange )
    Point2i o( cv_values );

    return allEqual( o.data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( Point2_constructorInitializerList )
    Point2i o( { cv_values[0], cv_values[1] } );

    return allEqual( o.data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( Point2_setCppRange )
    Point2i o( cv_values );

    o.set( cv_valuesAlt );

    return allEqual( o.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( Point2_setInitializerList )
    Point2i o( cv_values );

    o.set( { cv_valuesAlt[0], cv_valuesAlt[1] } );

    return allEqual( o.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( Point2_getCppRange )
    int values[ cv_size ];
    copyAll( cv_values, values );
    Point2i o( cv_valuesAlt );

    o.get( values );

    return allEqual( o.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( Point2_fill )
    Point2i o( cv_values );

    o.fill( 1 );

    return allEqualTo( o.data, 1 );
UT_TEST_END()

UT_TEST_BEGIN( Point2_begin )
    Point2i o( cv_values );

    return o.begin() == std::begin( o.data );
UT_TEST_END()

UT_TEST_BEGIN( Point2_beginConst )
    const Point2i o( cv_values );

    return o.begin() == std::begin( o.data );
UT_TEST_END()

UT_TEST_BEGIN( Point2_cbegin )
    Point2i o( cv_values );

    return o.cbegin() == std::cbegin( o.data );
UT_TEST_END()

UT_TEST_BEGIN( Point2_end )
    Point2i o( cv_values );

    return o.end() == std::end( o.data );
UT_TEST_END()

UT_TEST_BEGIN( Point2_endConst )
    const Point2i o( cv_values );

    return o.end() == std::end( o.data );
UT_TEST_END()

UT_TEST_BEGIN( Point2_cend )
    Point2i o( cv_values );

    return o.cend() == std::cend( o.data );
UT_TEST_END()

UT_TEST_BEGIN( Point2_rangedFor )
    Point2i o( cv_values );

    int idx = 0;
    for( int& i : o )
        i = cv_valuesAlt[idx++];

    return allEqual( o.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( Point2_rangedForConst )
    int data[ cv_size ];
    copyAll( cv_values, data );
    const Point2i o( cv_valuesAlt );

    int idx = 0;
    for( int i : o )
        data[idx++] = i;

    return allEqual( data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( Point2_index )
    Point2i o( cv_values );

    for( int i = 0; i < cv_size; ++i )
        o[i] = cv_valuesAlt[i];

    return allEqual( o.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( Point2_indexConst )
    int data[ cv_size ];
    const Point2i o( cv_values );

    for( int i = 0; i < cv_size; ++i )
        data[i] = o[i];

    return allEqual( data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( Point2_constructorValues )
    Point2i o( cv_values[0], cv_values[1] );

    return allEqual( o.data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( Point2_constructorCopy )
    Point2f o1( cv_valuesF );

    Point2i o2( o1 );

    return allEqual( o2.data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( Point2_setValues )
    Point2i o( cv_values );

    o.set( cv_valuesAlt[0], cv_valuesAlt[1] );

    return allEqual( o.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( Point2_getValues )
    Point2i o( cv_values );

    int x, y;
    o.get( x, y );

    return x == cv_values[0] &&
           y == cv_values[1];
UT_TEST_END()

UT_TEST_BEGIN( Point2_zero )
    Point2i o( cv_values );

    o.zero();

    return allEqual( o.data, cv_zero );
UT_TEST_END()

UT_TEST_BEGIN( Point2_isZero )
    Point2i o1( cv_zero );
    Point2i o2( cv_values );

    return o1.isZero() == true &&
           o2.isZero() == false;
UT_TEST_END()

UT_TEST_BEGIN( Point2_assignCopy )
    Point2f o1( cv_valuesF );
    Point2i o2( cv_valuesAlt );

    o2 = o1;

    return allEqual( o2.data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( Point2_equals )
    Point2i o1( cv_values );
    Point2i o2( cv_values );
    Point2i o3( cv_valuesAlt );

    return ( o1 == o2 ) == true   &&
           ( o1 == o3 ) == false;
UT_TEST_END()

UT_TEST_BEGIN( Point2_notEquals )
    Point2i o1( cv_values );
    Point2i o2( cv_values );
    Point2i o3( cv_valuesAlt );

    return ( o1 != o2 ) == false  &&
           ( o1 != o3 ) == true;
UT_TEST_END()

UT_TEST_BEGIN( Point2_output_int )
    Point2i o( cv_values );

    std::stringstream sout;
    sout << o;

    return sout.str() == cv_output;
UT_TEST_END()

UT_TEST_BEGIN( Point2_output_float )
    Point2f o( cv_valuesF );

    std::stringstream sout;
    sout << o;

    return sout.str() == cv_outputF;
UT_TEST_END()

UT_TEST_BEGIN( Point2_distanceSq )
    Point2i o1( cv_values );
    Point2i o2( cv_distant );

    return distanceSq( o1, o2 ) == cv_distanceSq;
UT_TEST_END()

UT_TEST_BEGIN( Point2_manhattan )
    Point2i o1( cv_values );
    Point2i o2( cv_distant );

    return manhattan( o1, o2 ) == cv_manhattan;
UT_TEST_END()




#ifdef BS_ZERO

UT_TEST_BEGIN( Point2_constructorZero )
    Point2i o;
    return allEqual( o.data, cv_zero );
UT_TEST_END()

#endif //BS_ZERO




#ifdef BS_CHECK_INDEX

UT_TEST_BEGIN( Point2_index_OOB )
    Point2i o;
    int i;

    try {
        i = o[(size_t)-1];
        return false;
    } catch( const BoundsException& ) {}

    try {
        i = o[2];
        return false;
    } catch( const BoundsException& ) {}

    return true;
UT_TEST_END()

UT_TEST_BEGIN( Point2_constIndex_OOB )
    const Point2i o;
    int i;

    try {
        i = o[(size_t)-1];
        return false;
    } catch( const BoundsException& ) {}

    try {
        i = o[2];
        return false;
    } catch( const BoundsException& ) {}

    return true;
UT_TEST_END()

#endif //BS_CHECK_INDEX




}