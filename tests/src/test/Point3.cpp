/*
test/Point3.cpp
-----------------------
Copyright (c) 2014, theJ89

Description:
    Unit tests for PointN<T,3> specialization
*/




//Includes
#include <sstream>              //std::ostringstream

#include "../Test.hpp"          //UT_TEST_BEGIN, UT_TEST_END
#include "../utils.hpp"         //allEqual, allEqualTo, copyAll

#include <brimstone/Point.hpp>
#include <brimstone/Vector.hpp> //Vector




namespace {
    using ::Brimstone::Point3i;
    using ::Brimstone::Point3f;
    using ::Brimstone::Vector3i;
    using ::Brimstone::BoundsException;

    const size_t cv_size         = 3;
    const int    cv_zero[3]      {  0, 0, 0 };
    const int    cv_values[3]    {  1, 2, 3 };
    const int    cv_valuesAlt[3] {  4, 5, 6 };
    const int    cv_distant[3]   { -6, 5, 4 };
    const int    cv_distanceSq   = 59;
    const int    cv_manhattan    = 11;
    const char*  cv_output       = "( 1, 2, 3 )";

    const float cv_valuesF[3]   { 1.0f, 2.0f, 3.0f };
    const char* cv_outputF      = "( 1.00000, 2.00000, 3.00000 )";
}

namespace UnitTest {

UT_TEST_BEGIN( Point3_constructorFill )
    Point3i o( 1 );

    return allEqualTo( o.data, 1 );
UT_TEST_END()

UT_TEST_BEGIN( Point3_constructorCppRange )
    Point3i o( cv_values );

    return allEqual( o.data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( Point3_constructorInitializerList )
    Point3i o( { cv_values[0], cv_values[1], cv_values[2] } );

    return allEqual( o.data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( Point3_setCppRange )
    Point3i o( cv_values );

    o.set( cv_valuesAlt );

    return allEqual( o.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( Point3_setInitializerList )
    Point3i o( cv_values );

    o.set( { cv_valuesAlt[0], cv_valuesAlt[1], cv_valuesAlt[2] } );

    return allEqual( o.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( Point3_getCppRange )
    int values[ cv_size ];
    copyAll( cv_values, values );
    Point3i o( cv_valuesAlt );

    o.get( values );

    return allEqual( o.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( Point3_fill )
    Point3i o( cv_values );

    o.fill( 1 );

    return allEqualTo( o.data, 1 );
UT_TEST_END()

UT_TEST_BEGIN( Point3_begin )
    Point3i o( cv_values );

    return o.begin() == std::begin( o.data );
UT_TEST_END()

UT_TEST_BEGIN( Point3_beginConst )
    const Point3i o( cv_values );

    return o.begin() == std::begin( o.data );
UT_TEST_END()

UT_TEST_BEGIN( Point3_cbegin )
    Point3i o( cv_values );

    return o.cbegin() == std::begin( o.data );
UT_TEST_END()

UT_TEST_BEGIN( Point3_end )
    Point3i o( cv_values );

    return o.end() == std::end( o.data );
UT_TEST_END()

UT_TEST_BEGIN( Point3_endConst )
    const Point3i o( cv_values );

    return o.end() == std::end( o.data );
UT_TEST_END()

UT_TEST_BEGIN( Point3_cend )
    Point3i o( cv_values );

    return o.cend() == std::end( o.data );
UT_TEST_END()

UT_TEST_BEGIN( Point3_rangedFor )
    Point3i o( cv_values );

    int idx = 0;
    for( int& i : o )
        i = cv_valuesAlt[idx++];

    return allEqual( o.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( Point3_rangedForConst )
    int data[ cv_size ];
    copyAll( cv_values, data );
    const Point3i o( cv_valuesAlt );

    int idx = 0;
    for( int i : o )
        data[idx++] = i;

    return allEqual( data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( Point3_index )
    Point3i o( cv_values );

    for( int i = 0; i < cv_size; ++i )
        o[i] = cv_valuesAlt[i];

    return allEqual( o.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( Point3_indexConst )
    int data[ cv_size ];
    const Point3i o( cv_values );

    for( int i = 0; i < cv_size; ++i )
        data[i] = o[i];

    return allEqual( data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( Point3_constructorValues )
    Point3i pt( cv_values[0], cv_values[1], cv_values[2] );

    return allEqual( pt.data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( Point3_constructorCopy )
    Point3f o1( cv_valuesF );

    Point3i o2( o1 );

    return allEqual( o2.data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( Point3_setValues )
    Point3i o( cv_values );

    o.set( cv_valuesAlt[0], cv_valuesAlt[1], cv_valuesAlt[2] );

    return allEqual( o.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( Point3_getValues )
    Point3i o( cv_values );

    int x, y, z;
    o.get( x, y, z );

    return x == cv_values[0] &&
           y == cv_values[1] &&
           z == cv_values[2];
UT_TEST_END()

UT_TEST_BEGIN( Point3_zero )
    Point3i o( cv_values );

    o.zero();

    return allEqual( o.data, cv_zero );
UT_TEST_END()

UT_TEST_BEGIN( Point3_isZero )
    Point3i o1( cv_zero );
    Point3i o2( cv_values );

    return o1.isZero() == true &&
           o2.isZero() == false;
UT_TEST_END()

UT_TEST_BEGIN( Point3_assignCopy )
    Point3f o1( cv_valuesF );
    Point3i o2( cv_valuesAlt );

    o2 = o1;

    return allEqual( o2.data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( Point3_typecastVector )
    Point3i o( cv_values );
    Vector3i v( cv_valuesAlt );

    v = (Vector3i&)o;

    return allEqual( o.data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( Point3_typecastVector_const )
    const Point3i o( cv_values );
    Vector3i v( cv_valuesAlt );

    v = (const Vector3i&)o;

    return allEqual( o.data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( Point3_equals )
    Point3i o1( cv_values );
    Point3i o2( cv_values );
    Point3i o3( cv_valuesAlt );

    return ( o1 == o2 ) == true   &&
           ( o1 == o3 ) == false;
UT_TEST_END()

UT_TEST_BEGIN( Point3_notEquals )
    Point3i o1( cv_values );
    Point3i o2( cv_values );
    Point3i o3( cv_valuesAlt );

    return ( o1 != o2 ) == false  &&
           ( o1 != o3 ) == true;
UT_TEST_END()

UT_TEST_BEGIN( Point3_output_int )
    Point3i o( cv_values );

    std::stringstream sout;
    sout << o;

    return sout.str() == cv_output;
UT_TEST_END()

UT_TEST_BEGIN( Point3_output_float )
    Point3f o( cv_valuesF );

    std::stringstream sout;
    sout << o;

    return sout.str() == cv_outputF;
UT_TEST_END()

UT_TEST_BEGIN( Point3_distanceSq )
    Point3i o1( cv_values );
    Point3i o2( cv_distant );

    return distanceSq( o1, o2 ) == cv_distanceSq;
UT_TEST_END()

UT_TEST_BEGIN( Point3_manhattan )
    Point3i o1( cv_values );
    Point3i o2( cv_distant );

    return manhattan( o1, o2 ) == cv_manhattan;
UT_TEST_END()




#ifdef BS_ZERO

UT_TEST_BEGIN( Point3_constructorZero )
    Point3i o;
    return allEqual( o.data, cv_zero );
UT_TEST_END()

#endif //BS_ZERO




#ifdef BS_CHECK_INDEX

UT_TEST_BEGIN( Point3_index_OOB )
    Point3i o;
    int i;

    try {
        i = o[(size_t)-1];
        return false;
    } catch( const BoundsException& ) {}

    try {
        i = o[3];
        return false;
    } catch( const BoundsException& ) {}

    return true;
UT_TEST_END()

UT_TEST_BEGIN( Point3_constIndex_OOB )
    const Point3i o;
    int i;

    try {
        i = o[(size_t)-1];
        return false;
    } catch( const BoundsException& ) {}

    try {
        i = o[3];
        return false;
    } catch( const BoundsException& ) {}

    return true;
UT_TEST_END()

#endif //BS_CHECK_INDEX




}