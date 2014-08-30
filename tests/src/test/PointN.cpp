/*
test/PointN.cpp
-----------------------
Copyright (c) 2014, theJ89

Description:
    Unit tests for PointN
*/




//Includes
#include <sstream>              //std::ostringstream

#include "../Test.hpp"          //UT_TEST_BEGIN, UT_TEST_END
#include "../utils.hpp"         //allEqual, allEqualTo, copyAll, isWithin, FAST_SQRT_ERR

#include <brimstone/Point.hpp>
#include <brimstone/Vector.hpp> //Vector




namespace {
    typedef ::Brimstone::Point< int, 5 >   Point5i;
    typedef ::Brimstone::Point< float, 5 > Point5f;
    typedef ::Brimstone::Vector< int, 5 >  Vector5i;
    using   ::Brimstone::BoundsException;

    const size_t cv_size         = 5;
    const int    cv_zero[5]      {   0, 0, 0,  0,  0 };
    const int    cv_values[5]    {   1, 2, 3,  4,  5 };
    const int    cv_valuesAlt[5] {   6, 7, 8,  9, 10 };
    const int    cv_distant[5]   { -10, 9, 8,  7,  6 };
    const int    cv_distance     = 14;   //14.3178211 -> 14
    const int    cv_distanceSq   = 205;
    const int    cv_manhattan    = 27;
    const char*  cv_output       = "( 1, 2, 3, 4, 5 )";

    const float cv_valuesF[5]   {   1.0f, 2.0f, 3.0f, 4.0f, 5.0f };
    const float cv_distantF[5]  { -10.0f, 9.0f, 8.0f, 7.0f, 6.0f };
    const float cv_distanceF    = 14.3178211f;
    const char* cv_outputF      = "( 1.00000, 2.00000, 3.00000, 4.00000, 5.00000 )";
}

namespace UnitTest {

UT_TEST_BEGIN( PointN_constructorFill )
    Point5i o( 1 );

    return allEqualTo( o.data, 1 );
UT_TEST_END()

UT_TEST_BEGIN( PointN_constructorCppRange )
    Point5i o( cv_values );

    return allEqual( o.data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( PointN_constructorInitializerList )
    Point5i o( { cv_values[0], cv_values[1], cv_values[2], cv_values[3], cv_values[4] } );

    return allEqual( o.data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( PointN_setCppRange )
    Point5i o( cv_values );

    o.set( cv_valuesAlt );

    return allEqual( o.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( PointN_setInitializerList )
    Point5i o( cv_values );

    o.set( { cv_valuesAlt[0], cv_valuesAlt[1], cv_valuesAlt[2], cv_valuesAlt[3], cv_valuesAlt[4] } );

    return allEqual( o.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( PointN_getCppRange )
    int values[ cv_size ];
    copyAll( cv_values, values );
    Point5i o( cv_valuesAlt );

    o.get( values );

    return allEqual( o.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( PointN_fill )
    Point5i o( cv_values );

    o.fill( 1 );

    return allEqualTo( o.data, 1 );
UT_TEST_END()

UT_TEST_BEGIN( PointN_begin )
    Point5i o( cv_values );

    return o.begin() == std::begin( o.data );
UT_TEST_END()

UT_TEST_BEGIN( PointN_beginConst )
    const Point5i o( cv_values );

    return o.begin() == std::begin( o.data );
UT_TEST_END()

UT_TEST_BEGIN( PointN_cbegin )
    Point5i o( cv_values );

    return o.cbegin() == std::begin( o.data );
UT_TEST_END()

UT_TEST_BEGIN( PointN_end )
    Point5i o( cv_values );

    return o.end() == std::end( o.data );
UT_TEST_END()

UT_TEST_BEGIN( PointN_endConst )
    const Point5i o( cv_values );

    return o.end() == std::end( o.data );
UT_TEST_END()

UT_TEST_BEGIN( PointN_cend )
    Point5i o( cv_values );

    return o.cend() == std::end( o.data );
UT_TEST_END()

UT_TEST_BEGIN( PointN_rangedFor )
    Point5i o( cv_values );

    int idx = 0;
    for( int& i : o )
        i = cv_valuesAlt[idx++];

    return allEqual( o.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( PointN_rangedForConst )
    int data[cv_size];
    copyAll( cv_values, data );
    const Point5i o( cv_valuesAlt );

    int idx = 0;
    for( int i : o )
        data[idx++] = i;

    return allEqual( data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( PointN_index )
    Point5i o( cv_values );

    for( int i = 0; i < cv_size; ++i )
        o[i] = cv_valuesAlt[i];

    return allEqual( o.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( PointN_indexConst )
    int data[cv_size];
    const Point5i o( cv_values );

    for( int i = 0; i < cv_size; ++i )
        data[i] = o[i];

    return allEqual( data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( PointN_constructorCopy )
    Point5f o1( cv_valuesF );

    Point5i o2( o1 );

    return allEqual( o2.data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( PointN_zero )
    Point5i o( cv_values );

    o.zero();

    return allEqual( o.data, cv_zero );
UT_TEST_END()

UT_TEST_BEGIN( PointN_isZero )
    Point5i o1( cv_zero );
    Point5i o2( cv_values );

    return o1.isZero() == true &&
           o2.isZero() == false;
UT_TEST_END()

UT_TEST_BEGIN( PointN_assignCopy )
    Point5f o1( cv_valuesF );
    Point5i o2( cv_valuesAlt );

    o2 = o1;

    return allEqual( o2.data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( PointN_typecastVector )
    Point5i o( cv_values );
    Vector5i v( cv_valuesAlt );

    v = (Vector5i&)o;

    return allEqual( o.data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( PointN_typecastVector_const )
    const Point5i o( cv_values );
    Vector5i v( cv_valuesAlt );

    v = (const Vector5i&)o;

    return allEqual( o.data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( PointN_equals )
    Point5i o1( cv_values );
    Point5i o2( cv_values );
    Point5i o3( cv_valuesAlt );

    return ( o1 == o2 ) == true   &&
           ( o1 == o3 ) == false;
UT_TEST_END()

UT_TEST_BEGIN( PointN_notEquals )
    Point5i o1( cv_values );
    Point5i o2( cv_values );
    Point5i o3( cv_valuesAlt );

    return ( o1 != o2 ) == false  &&
           ( o1 != o3 ) == true;
UT_TEST_END()

UT_TEST_BEGIN( PointN_output_int )
    Point5i o( cv_values );

    std::stringstream sout;
    sout << o;

    return sout.str() == cv_output;
UT_TEST_END()

UT_TEST_BEGIN( PointN_output_float )
    Point5f o( cv_valuesF );

    std::stringstream sout;
    sout << o;

    return sout.str() == cv_outputF;
UT_TEST_END()

UT_TEST_BEGIN( PointN_distance_int )
    Point5i o1( cv_values );
    Point5i o2( cv_distant );

    return distance( o1, o2 ) == cv_distance;
UT_TEST_END()

UT_TEST_BEGIN( PointN_distance_float )
    Point5f o1( cv_valuesF );
    Point5f o2( cv_distantF );

    return isWithin( distance( o1, o2 ), cv_distanceF, FAST_SQRT_ERROR );
UT_TEST_END()

UT_TEST_BEGIN( PointN_distanceSq )
    Point5i o1( cv_values );
    Point5i o2( cv_distant );

    return distanceSq( o1, o2 ) == cv_distanceSq;
UT_TEST_END()

UT_TEST_BEGIN( PointN_manhattan )
    Point5i o1( cv_values );
    Point5i o2( cv_distant );

    return manhattan( o1, o2 ) == cv_manhattan;
UT_TEST_END()




#ifdef BS_ZERO

UT_TEST_BEGIN( PointN_constructorZero )
    Point5i o;
    return allEqual( o.data, cv_zero );
UT_TEST_END()

#endif //BS_ZERO




#ifdef BS_CHECK_INDEX

UT_TEST_BEGIN( PointN_index_OOB )
    Point5i o;
    int i;

    try {
        i = o[(size_t)-1];
        return false;
    } catch( const BoundsException& ) {}

    try {
        i = o[5];
        return false;
    } catch( const BoundsException& ) {}

    return true;
UT_TEST_END()

UT_TEST_BEGIN( PointN_constIndex_OOB )
    const Point5i o;
    int i;

    try {
        i = o[(size_t)-1];
        return false;
    } catch( const BoundsException& ) {}

    try {
        i = o[5];
        return false;
    } catch( const BoundsException& ) {}

    return true;
UT_TEST_END()

#endif //BS_CHECK_INDEX

}