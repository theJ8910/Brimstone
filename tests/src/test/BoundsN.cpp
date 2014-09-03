/*
test/BoundsN.cpp
-----------------------
Copyright (c) 2014, theJ89

Description:
    Unit tests for BoundsN
*/




//Includes
#include "../Test.hpp"
#include "../utils.hpp"         //allEqual, allEqualTo

#include <brimstone/Bounds.hpp>




namespace {
    typedef ::Brimstone::Bounds< int, 5 >   Bounds5i;
    typedef ::Brimstone::Point< int, 5 >    Point5i;
    typedef ::Brimstone::Bounds< float, 5 > Bounds5f;
    using   ::Brimstone::BoundsException;

    const size_t cv_size             = 5;
    const int    cv_zero[10]         {  0,  0,  0,  0,  0,    0,  0,  0,  0,  0 };
    const int    cv_values[10]       {  1,  2,  3,  4,  5,    6,  7,  8,  9, 10 };
    const int    cv_valuesMins[5]    {  1,  2,  3,  4,  5 };
    const int    cv_valuesMaxs[5]    {  6,  7,  8,  9, 10 };
    const int    cv_valuesAlt[10]    { 11, 12, 13, 14, 15,   16, 17, 18, 19, 20 };
    const int    cv_valuesAltMins[5] { 11, 12, 13, 14, 15 };
    const int    cv_valuesAltMaxs[5] { 16, 17, 18, 19, 20 };
    const int    cv_dimTest[10]      {  1,  2,  3,  4,  5,   11, 13, 15, 17, 19 };
    const int    cv_outsideMins[5]   {  0,  1,  2,  3,  4 };
    const int    cv_outsideMaxs[5]   {  7,  8,  9, 10, 11 };
    const char*  cv_output           = "[ ( 1, 2, 3, 4, 5 ), ( 6, 7, 8, 9, 10 ) ]";

    const float  cv_valuesAltF[10]   { 11.0f, 12.0f, 13.0f, 14.0f, 15.0f,   16.0f, 17.0f, 18.0f, 19.0f, 20.0f };
}

namespace UnitTest {

UT_TEST_BEGIN( BoundsN_constructorFill )
    Bounds5i o( 1 );

    return allEqualTo( o.data, 1 );
UT_TEST_END()

UT_TEST_BEGIN( BoundsN_constructorCppRange )
    Bounds5i o( cv_values );

    return allEqual( o.data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( BoundsN_constructorInitializerList )
    Bounds5i o( {
        cv_values[0],
        cv_values[1],
        cv_values[2],
        cv_values[3],
        cv_values[4],
        cv_values[5],
        cv_values[6],
        cv_values[7],
        cv_values[8],
        cv_values[9]
    } );

    return allEqual( o.data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( BoundsN_setCppRange )
    Bounds5i o( cv_values );

    o.set( cv_valuesAlt );

    return allEqual( o.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( BoundsN_setInitializerList )
    Bounds5i o( cv_values );

    o.set( {
        cv_valuesAlt[0],
        cv_valuesAlt[1],
        cv_valuesAlt[2],
        cv_valuesAlt[3],
        cv_valuesAlt[4],
        cv_valuesAlt[5],
        cv_valuesAlt[6],
        cv_valuesAlt[7],
        cv_valuesAlt[8],
        cv_valuesAlt[9]
    } );

    return allEqual( o.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( BoundsN_getCppRange )
    int values[ 2*cv_size ];
    copyAll( cv_values, values );
    Bounds5i o( cv_valuesAlt );

    o.get( values );

    return allEqual( o.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( BoundsN_fill )
    Bounds5i o( cv_values );

    o.fill( 1 );

    return allEqualTo( o.data, 1 );
UT_TEST_END()

UT_TEST_BEGIN( BoundsN_begin )
    Bounds5i o( cv_values );

    return o.begin() == std::begin( o.data );
UT_TEST_END()

UT_TEST_BEGIN( BoundsN_beginConst )
    const Bounds5i o( cv_values );

    return o.begin() == std::begin( o.data );
UT_TEST_END()

UT_TEST_BEGIN( BoundsN_cbegin )
    Bounds5i o( cv_values );

    return o.cbegin() == std::begin( o.data );
UT_TEST_END()

UT_TEST_BEGIN( BoundsN_end )
    Bounds5i o( cv_values );

    return o.end() == std::end( o.data );
UT_TEST_END()

UT_TEST_BEGIN( BoundsN_endConst )
    const Bounds5i o( cv_values );

    return o.end() == std::end( o.data );
UT_TEST_END()

UT_TEST_BEGIN( BoundsN_cend )
    Bounds5i o( cv_values );

    return o.cend() == std::end( o.data );
UT_TEST_END()

UT_TEST_BEGIN( BoundsN_rangedFor )
    Bounds5i o( cv_values );

    int idx = 0;
    for( int& i : o )
        i = cv_valuesAlt[idx++];

    return allEqual( o.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( BoundsN_rangedForConst )
    int data[2*cv_size];
    copyAll( cv_values, data );
    const Bounds5i o( cv_valuesAlt );

    int idx = 0;
    for( int i : o )
        data[idx++] = i;

    return allEqual( data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( BoundsN_index )
    Bounds5i o( cv_values );

    for( size_t i = 0; i < 2*cv_size; ++i )
        o[i] = cv_valuesAlt[i];

    return allEqual( o.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( BoundsN_indexConst )
    int data[2*cv_size];
    const Bounds5i o( cv_values );

    for( size_t i = 0; i < 2*cv_size; ++i )
        data[i] = o[i];

    return allEqual( data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( BoundsN_constructorCopy )
    Bounds5f o1( cv_valuesAltF );

    Bounds5i o2( o1 );

    return allEqual( o2.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( BoundsN_assignCopy )
    Bounds5f o1( cv_valuesAltF );
    Bounds5i o2( cv_values );

    o2 = o1;

    return allEqual( o2.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( BoundsN_constructorMinMax )
    Point5i mins( cv_valuesMins );
    Point5i maxs( cv_valuesMaxs );
    Bounds5i o( mins, maxs );

    return allEqual( o.data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( BoundsN_setMinMax )
    Bounds5i o( cv_values );
    Point5i mins( cv_valuesMins );
    Point5i maxs( cv_valuesMaxs );

    o.set( mins, maxs );

    return allEqual( o.data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( BoundsN_getMinMax )
    Point5i mins( cv_valuesMins );
    Point5i maxs( cv_valuesMaxs );

    Bounds5i o( cv_valuesAlt );
    o.get( mins, maxs );

    return allEqual( mins.data, cv_valuesAltMins ) &&
           allEqual( maxs.data, cv_valuesAltMaxs );
UT_TEST_END()

UT_TEST_BEGIN( BoundsN_setDimension )
    Bounds5i o( cv_values );

    for( size_t i = 0; i < cv_size; ++i )
        o.setDimension( i, (int)(10 + i) );

    return allEqual( o.data, cv_dimTest );
UT_TEST_END()

UT_TEST_BEGIN( BoundsN_getDimension )
    Bounds5i o( cv_dimTest );

    for( size_t i = 0; i < cv_size; ++i )
        if( o.getDimension( i ) != (int)( 10 + i ) )
            return false;

    return true;
UT_TEST_END()

UT_TEST_BEGIN( BoundsN_output )
    Bounds5i o( cv_values );

    std::stringstream sout;
    sout << o;

    return sout.str() == cv_output;
UT_TEST_END()

UT_TEST_BEGIN( BoundsN_zero )
    Bounds5i o( cv_values );

    o.zero();

    return allEqual( o.data, cv_zero );
UT_TEST_END()

UT_TEST_BEGIN( BoundsN_isZero )
    Bounds5i o1( cv_zero );
    Bounds5i o2( cv_values );

    return o1.isZero() == true &&
           o2.isZero() == false;
UT_TEST_END()

UT_TEST_BEGIN( BoundsN_equals )
    Bounds5i o1( cv_values );
    Bounds5i o2( cv_values );
    Bounds5i o3( cv_valuesAlt );

    return ( o1 == o2 ) == true   &&
           ( o1 == o3 ) == false;
UT_TEST_END()

UT_TEST_BEGIN( BoundsN_notEquals )
    Bounds5i o1( cv_values );
    Bounds5i o2( cv_values );
    Bounds5i o3( cv_valuesAlt );

    return ( o1 != o2 ) == false  &&
           ( o1 != o3 ) == true;
UT_TEST_END()

UT_TEST_BEGIN( BoundsN_clamp )
    Bounds5i o( cv_values );
    Point5i p1( cv_outsideMins );
    Point5i p2( cv_outsideMaxs );

    clamp( p1, o );
    clamp( p2, o );

    return allEqual( p1.data, cv_valuesMins ) &&
           allEqual( p2.data, cv_valuesMaxs );
UT_TEST_END()

UT_TEST_BEGIN( BoundsN_clampedPoint )
    Bounds5i o( cv_values );
    Point5i p1( cv_outsideMins );
    Point5i p2( cv_outsideMaxs );

    p1 = clampedPoint( p1, o );
    p2 = clampedPoint( p2, o );

    return allEqual( p1.data, cv_valuesMins ) &&
           allEqual( p2.data, cv_valuesMaxs );
UT_TEST_END()




#ifdef BS_ZERO

UT_TEST_BEGIN( BoundsN_constructorZero )
    Bounds5i o;
    return allEqual( o.data, cv_zero );
UT_TEST_END()

#endif //BS_ZERO




#ifdef BS_CHECK_INDEX

UT_TEST_BEGIN( BoundsN_index_OOB )
    Bounds5i o;

    try {
        o[(size_t)-1];
        return false;
    } catch( const BoundsException& ) {}

    try {
        o[10];
        return false;
    } catch( const BoundsException& ) {}

    return true;
UT_TEST_END()

UT_TEST_BEGIN( BoundsN_constIndex_OOB )
    const Bounds5i o;

    try {
        o[(size_t)-1];
        return false;
    } catch( const BoundsException& ) {}

    try {
        o[10];
        return false;
    } catch( const BoundsException& ) {}

    return true;
UT_TEST_END()

UT_TEST_BEGIN( BoundsN_setDimension_OOB )
    Bounds5i o;

    try {
        o.setDimension( (size_t)-1, 10 );
        return false;
    } catch( const BoundsException& ) {}

    try {
        o.setDimension( 5, 10 );
        return false;
    } catch( const BoundsException& ) {}

    return true;
UT_TEST_END()

UT_TEST_BEGIN( BoundsN_getDimension_OOB )
    Bounds5i o;

    try {
        o.getDimension( (size_t)(-1) );
        return false;
    } catch( const BoundsException& ) {}

    try {
        o.getDimension( 5 );
        return false;
    } catch( const BoundsException& ) {}

    return true;
UT_TEST_END()

#endif //BS_CHECK_INDEX

}