/*
test/SizeN.cpp
--------------
Copyright (c) 2024, theJ89

Description:
    Unit tests for Size
*/




//Includes
#include <sstream>              //std::ostringstream

#include "../Test.hpp"          //UT_TEST_BEGIN, UT_TEST_END
#include "../utils.hpp"         //allEqual, allEqualTo, copyAll, isWithin, allWithin, FAST_SQRT_ERR

#include <brimstone/Size.hpp>




namespace {
    typedef ::Brimstone::Size< int, 5 >   Size5i;
    typedef ::Brimstone::Size< float, 5 > Size5f;
    using   ::Brimstone::BoundsException;

    const size_t cv_size               = 5;
    const int    cv_zero[5]            {  0,  0,   0,  0,   0 };
    const int    cv_values[5]          {  1,  2,   3,  4,   5 };
    const int    cv_valuesAlt[5]       {  6,  7,   8,  9,  10 };
    const int    cv_volume             = 6 * 7 * 8 * 9 * 10;
    const char*  cv_output             = "[ 1, 2, 3, 4, 5 ]";

    const float cv_zeroF[5]            { 0.0f, 0.0f, 0.0f, 0.0f,  0.0f };
    const float cv_valuesF[5]          { 1.0f, 2.0f, 3.0f, 4.0f,  5.0f };

    const char* cv_outputF = "[ 1.00000, 2.00000, 3.00000, 4.00000, 5.00000 ]";
}

namespace UnitTest {

UT_TEST_BEGIN( SizeN_constructorFill )
    Size5i o( 1 );

    return allEqualTo( o.data, 1 );
UT_TEST_END()

UT_TEST_BEGIN( SizeN_constructorCppRange )
    Size5i o( cv_values );

    return allEqual( o.data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( SizeN_constructorInitializerList )
    Size5i o( { cv_values[0], cv_values[1], cv_values[2], cv_values[3], cv_values[4] } );

    return allEqual( o.data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( SizeN_setCppRange )
    Size5i o( cv_values );

    o.set( cv_valuesAlt );

    return allEqual( o.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( SizeN_setInitializerList )
    Size5i o( cv_values );

    o.set( { cv_valuesAlt[0], cv_valuesAlt[1], cv_valuesAlt[2], cv_valuesAlt[3], cv_valuesAlt[4] } );

    return allEqual( o.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( SizeN_getCppRange )
    int values[ cv_size ];
    copyAll( cv_values, values );
    Size5i o( cv_valuesAlt );

    o.get( values );

    return allEqual( o.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( SizeN_fill )
    Size5i o( cv_values );

    o.fill( 1 );

    return allEqualTo( o.data, 1 );
UT_TEST_END()

UT_TEST_BEGIN( SizeN_begin )
    Size5i o( cv_values );

    return o.begin() == std::begin( o.data );
UT_TEST_END()

UT_TEST_BEGIN( SizeN_beginConst )
    const Size5i o( cv_values );

    return o.begin() == std::begin( o.data );
UT_TEST_END()

UT_TEST_BEGIN( SizeN_cbegin )
    Size5i o( cv_values );

    return o.cbegin() == std::begin( o.data );
UT_TEST_END()

UT_TEST_BEGIN( SizeN_end )
    Size5i o( cv_values );

    return o.end() == std::end( o.data );
UT_TEST_END()

UT_TEST_BEGIN( SizeN_endConst )
    const Size5i o( cv_values );

    return o.end() == std::end( o.data );
UT_TEST_END()

UT_TEST_BEGIN( SizeN_cend )
    Size5i o( cv_values );

    return o.cend() == std::end( o.data );
UT_TEST_END()

UT_TEST_BEGIN( SizeN_rangedFor )
    Size5i o( cv_values );

    int idx = 0;
    for( int& i : o )
        i = cv_valuesAlt[idx++];

    return allEqual( o.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( SizeN_rangedForConst )
    int data[cv_size];
    copyAll( cv_values, data );
    const Size5i o( cv_valuesAlt );

    int idx = 0;
    for( int i : o )
        data[idx++] = i;

    return allEqual( data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( SizeN_index )
    Size5i o( cv_values );

    for( size_t i = 0; i < cv_size; ++i )
        o[i] = cv_valuesAlt[i];

    return allEqual( o.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( SizeN_indexConst )
    int data[cv_size];
    const Size5i o( cv_values );

    for( size_t i = 0; i < cv_size; ++i )
        data[i] = o[i];

    return allEqual( data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( SizeN_constructorCopy )
    Size5f o1( cv_valuesF );

    Size5i o2( o1 );

    return allEqual( o2.data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( SizeN_zero )
    Size5i o( cv_values );

    o.zero();

    return allEqual( o.data, cv_zero );
UT_TEST_END()

UT_TEST_BEGIN( SizeN_isZero )
    Size5i o1( cv_zero );
    Size5i o2( cv_values );

    return o1.isZero() == true &&
           o2.isZero() == false;
UT_TEST_END()

UT_TEST_BEGIN( SizeN_assignCopy )
    Size5f o1( cv_valuesF );
    Size5i o2( cv_valuesAlt );

    o2 = o1;

    return allEqual( o2.data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( SizeN_getArea )
    Size5i o( cv_valuesAlt );

    return o.getVolume() == cv_volume;
UT_TEST_END()

UT_TEST_BEGIN( SizeN_equals )
    Size5i o1( cv_values );
    Size5i o2( cv_values );
    Size5i o3( cv_valuesAlt );

    return ( o1 == o2 ) == true   &&
           ( o1 == o3 ) == false;
UT_TEST_END()

UT_TEST_BEGIN( SizeN_notEquals )
    Size5i o1( cv_values );
    Size5i o2( cv_values );
    Size5i o3( cv_valuesAlt );

    return ( o1 != o2 ) == false  &&
           ( o1 != o3 ) == true;
UT_TEST_END()

UT_TEST_BEGIN( SizeN_output_int )
    Size5i o( cv_values );

    std::stringstream sout;
    sout << o;

    return sout.str() == cv_output;
UT_TEST_END()

UT_TEST_BEGIN( SizeN_output_float )
    Size5f o( cv_valuesF );

    std::stringstream sout;
    sout << o;

    return sout.str() == cv_outputF;
UT_TEST_END()




#ifdef BS_ZERO

UT_TEST_BEGIN( SizeN_constructorZero )
    Size5i o;
    return allEqual( o.data, cv_zero );
UT_TEST_END()

#endif //BS_ZERO




#ifdef BS_CHECK_INDEX

UT_TEST_BEGIN( SizeN_index_OOB )
    Size5i o;

    try {
        o[(size_t)-1];
        return false;
    } catch( const BoundsException& ) {}

    try {
        o[5];
        return false;
    } catch( const BoundsException& ) {}

    return true;
UT_TEST_END()

UT_TEST_BEGIN( SizeN_constIndex_OOB )
    const Size5i o;

    try {
        o[(size_t)-1];
        return false;
    } catch( const BoundsException& ) {}

    try {
        o[5];
        return false;
    } catch( const BoundsException& ) {}

    return true;
UT_TEST_END()

#endif //BS_CHECK_INDEX

}
