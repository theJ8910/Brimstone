/*
test/Size2.cpp
--------------
Copyright (c) 2024, theJ89

Description:
    Unit tests for Size2
*/




//Include
#include <sstream>              //std::ostringstream

#include "../Test.hpp"          //UT_TEST_BEGIN, UT_TEST_END
#include "../utils.hpp"         //allEqual, allEqualTo, copyAll, isWithin, allWithin, FAST_SQRT_ERR

#include <brimstone/Size.hpp>




namespace {
    using ::Brimstone::Size2i;
    using ::Brimstone::Size2f;
    using ::Brimstone::BoundsException;

    const size_t cv_size         = 2;
    const int    cv_zero[2]      { 0, 0 };
    const int    cv_values[2]    { 1, 2 };
    const int    cv_valuesAlt[2] { 3, 4 };
    const int    cv_area         = 3 * 4;
    const char*  cv_output       = "[ 1, 2 ]";

    const float  cv_valuesF[2]   { 1.0f, 2.0f };
    const char*  cv_outputF      = "[ 1.00000, 2.00000 ]";
}

namespace UnitTest {

UT_TEST_BEGIN( Size2_constructorFill )
    Size2i o( 1 );

    return allEqualTo( o.data, 1 );
UT_TEST_END()

UT_TEST_BEGIN( Size2_constructorCppRange )
    Size2i o( cv_values );

    return allEqual( o.data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( Size2_constructorInitializerList )
    Size2i o( { cv_values[0], cv_values[1], cv_values[2], cv_values[3] } );

    return allEqual( o.data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( Size2_setCppRange )
    Size2i o( cv_values );

    o.set( cv_valuesAlt );

    return allEqual( o.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( Size2_setInitializerList )
    Size2i o( cv_values );

    o.set( { cv_valuesAlt[0], cv_valuesAlt[1], cv_valuesAlt[2], cv_valuesAlt[3] } );

    return allEqual( o.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( Size2_getCppRange )
    int values[ cv_size ];
    copyAll( cv_values, values );
    Size2i o( cv_valuesAlt );

    o.get( values );

    return allEqual( o.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( Size2_fill )
    Size2i o( cv_values );

    o.fill( 1 );

    return allEqualTo( o.data, 1 );
UT_TEST_END()

UT_TEST_BEGIN( Size2_begin )
    Size2i o( cv_values );

    return o.begin() == std::begin( o.data );
UT_TEST_END()

UT_TEST_BEGIN( Size2_beginConst )
    const Size2i o( cv_values );

    return o.begin() == std::begin( o.data );
UT_TEST_END()

UT_TEST_BEGIN( Size2_cbegin )
    Size2i o( cv_values );

    return o.cbegin() == std::begin( o.data );
UT_TEST_END()

UT_TEST_BEGIN( Size2_end )
    Size2i o( cv_values );

    return o.end() == std::end( o.data );
UT_TEST_END()

UT_TEST_BEGIN( Size2_endConst )
    const Size2i o( cv_values );

    return o.end() == std::end( o.data );
UT_TEST_END()

UT_TEST_BEGIN( Size2_cend )
    Size2i o( cv_values );

    return o.cend() == std::end( o.data );
UT_TEST_END()

UT_TEST_BEGIN( Size2_rangedFor )
    Size2i o( cv_values );

    int idx = 0;
    for( int& i : o )
        i = cv_valuesAlt[idx++];

    return allEqual( o.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( Size2_rangedForConst )
    int data[ cv_size ];
    copyAll( cv_values, data );
    const Size2i o( cv_valuesAlt );

    int idx = 0;
    for( int i : o )
        data[idx++] = i;

    return allEqual( data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( Size2_index )
    Size2i o( cv_values );

    for( size_t i = 0; i < cv_size; ++i )
        o[i] = cv_valuesAlt[i];

    return allEqual( o.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( Size2_indexConst )
    int data[ cv_size ];
    const Size2i o( cv_values );

    for( size_t i = 0; i < cv_size; ++i )
        data[i] = o[i];

    return allEqual( data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( Size2_constructorValues )
    Size2i pt( cv_values[0], cv_values[1] );

    return allEqual( pt.data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( Size2_constructorCopy )
    Size2f o1( cv_valuesF );

    Size2i o2( o1 );

    return allEqual( o2.data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( Size2_setValues )
    Size2i o( cv_values );

    o.set( cv_valuesAlt[0], cv_valuesAlt[1] );

    return allEqual( o.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( Size2_getValues )
    Size2i o( cv_values );

    int width, height;
    o.get( width, height );

    return width  == cv_values[0] &&
           height == cv_values[1];
UT_TEST_END()

UT_TEST_BEGIN( Size2_zero )
    Size2i o( cv_values );

    o.zero();

    return allEqual( o.data, cv_zero );
UT_TEST_END()

UT_TEST_BEGIN( Size2_isZero )
    Size2i o1( cv_zero );
    Size2i o2( cv_values );

    return o1.isZero() == true &&
           o2.isZero() == false;
UT_TEST_END()

UT_TEST_BEGIN( Size2_assignCopy )
    Size2f o1( cv_valuesF );
    Size2i o2( cv_valuesAlt );

    o2 = o1;

    return allEqual( o2.data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( Size2_getArea )
    Size2i o( cv_valuesAlt );

    return o.getArea() == cv_area;
UT_TEST_END()

UT_TEST_BEGIN( Size2_equals )
    Size2i o1( cv_values );
    Size2i o2( cv_values );
    Size2i o3( cv_valuesAlt );

    return ( o1 == o2 ) == true   &&
           ( o1 == o3 ) == false;
UT_TEST_END()

UT_TEST_BEGIN( Size2_notEquals )
    Size2i o1( cv_values );
    Size2i o2( cv_values );
    Size2i o3( cv_valuesAlt );

    return ( o1 != o2 ) == false  &&
           ( o1 != o3 ) == true;
UT_TEST_END()

UT_TEST_BEGIN( Size2_output_int )
    Size2i o( cv_values );

    std::stringstream sout;
    sout << o;

    return sout.str() == cv_output;
UT_TEST_END()

UT_TEST_BEGIN( Size2_output_float )
    Size2f o( cv_valuesF );

    std::stringstream sout;
    sout << o;

    return sout.str() == cv_outputF;
UT_TEST_END()




#ifdef BS_ZERO

UT_TEST_BEGIN( Size2_constructorZero )
    Size2i o;
    return allEqual( o.data, cv_zero );
UT_TEST_END()

#endif //BS_ZERO




#ifdef BS_CHECK_INDEX

UT_TEST_BEGIN( Size2_index_OOB )
    Size2i o;

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

UT_TEST_BEGIN( Size2_constIndex_OOB )
    const Size2i o;

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
