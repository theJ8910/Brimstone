/*
test/BoundsN.cpp
----------------
Copyright (c) 2024, theJ89

Description:
    Unit tests for BoundsN
*/




//Includes
#include "../Test.hpp"
#include "../utils.hpp"         //allEqual, allEqualTo

#include <cstddef>              //std::size_t

#include <brimstone/Bounds.hpp>




namespace {
    typedef ::Brimstone::Bounds< int, 5 >   Bounds5i;
    typedef ::Brimstone::Point< int, 5 >    Point5i;
    typedef ::Brimstone::Size< int, 5 >     Size5i;
    typedef ::Brimstone::Bounds< float, 5 > Bounds5f;
    using   ::Brimstone::BoundsException;

    const std::size_t cv_size               = 5;
    const int         cv_zero[10]           {  0,  0,  0,  0,  0,    0,  0,  0,  0,  0 };
    const int         cv_values[10]         {  1,  2,  3,  4,  5,    6,  7,  8,  9, 10 };
    const int         cv_valuesMins[5]      {  1,  2,  3,  4,  5 };
    const int         cv_valuesMaxs[5]      {  6,  7,  8,  9, 10 };
    const int         cv_valuesAlt[10]      { 11, 12, 13, 14, 15,   16, 17, 18, 19, 20 };
    const int         cv_valuesAltMins[5]   { 11, 12, 13, 14, 15 };
    const int         cv_valuesAltMaxs[5]   { 16, 17, 18, 19, 20 };
    const int         cv_valuesAltCenter[5] { 13, 14, 15, 16, 17 };
    const int         cv_abnormal[10]       {  6,  7,  8,  9, 10,    1,  2,  3,  4,  5 };
    const int         cv_includeTest[10]    { -14, -11, -12, -4, -5,   6,  12,  8,  13, 14 };
    const int         cv_includePt1[5]      { -14,  12,   2, -4, -5 };
    const int         cv_includePt2[5]      {  2,  -11, -12, 13, 14 };
    const int         cv_isTest1[10]        { -1,  0,  1,  2,  3,    3,   4,   5,   6,   7 };
    const int         cv_isTest2[10]        {  4,  5,  6,  7,  8,    8,   9,  10,  11,  12 };
    const int         cv_isTest3[10]        {  7,  8,  9, 10, 11,   12,  13,  14,  15,  16 };
    const int         cv_isTestCorner[10]   {  6,  7,  8,  9, 10,   11,  12,  13,  14,  15 };
    const int         cv_intersect1[10]     {  1,  2,  3,  4,  5,    3,   4,   5,   6,  7  };
    const int         cv_intersect2[10]     {  4,  5,  6,  7,  8,    6,   7,   8,   9,  10 };
    const int         cv_sizes[5]           { 10, 11, 12, 13, 14 };
    const int         cv_dimTest[10]        {  1,  2,  3,  4,  5,   11, 13, 15, 17, 19 };
    const int         cv_outsideMins[5]     {  0,  1,  2,  3,  4 };
    const int         cv_outsideMaxs[5]     {  7,  8,  9, 10, 11 };
    const char*       cv_output             = "[ ( 1, 2, 3, 4, 5 ), ( 6, 7, 8, 9, 10 ) ]";

    const float       cv_valuesAltF[10]   { 11.0f, 12.0f, 13.0f, 14.0f, 15.0f,   16.0f, 17.0f, 18.0f, 19.0f, 20.0f };
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

    for( std::size_t i = 0; i < 2*cv_size; ++i )
        o[i] = cv_valuesAlt[i];

    return allEqual( o.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( BoundsN_indexConst )
    int data[2*cv_size];
    const Bounds5i o( cv_values );

    for( std::size_t i = 0; i < 2*cv_size; ++i )
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

UT_TEST_BEGIN( BoundsN_constructorPosSize )
    Point5i mins( cv_valuesMins );
    Size5i  sizes( cv_sizes );

    Bounds5i o( mins, sizes );

    return allEqual( o.data, cv_dimTest );
UT_TEST_END()

UT_TEST_BEGIN( BoundsN_set_minMax )
    Bounds5i o( cv_valuesAlt );
    Point5i mins( cv_valuesMins );
    Point5i maxs( cv_valuesMaxs );

    o.set( mins, maxs );

    return allEqual( o.data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( BoundsN_get_minMax )
    Point5i mins( cv_valuesMins );
    Point5i maxs( cv_valuesMaxs );

    Bounds5i o( cv_valuesAlt );
    o.get( mins, maxs );

    return allEqual( mins.data, cv_valuesAltMins ) &&
           allEqual( maxs.data, cv_valuesAltMaxs );
UT_TEST_END()

UT_TEST_BEGIN( BoundsN_setPosition )
    Bounds5i o( cv_values );
    Point5i  mins( cv_valuesAltMins );

    o.setPosition( mins );

    return allEqual( o.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( BoundsN_getPosition )
    Bounds5i o( cv_values );
    Point5i mins( cv_valuesAltMins );

    mins = o.getPosition();

    return allEqual( mins.data, cv_valuesMins );
UT_TEST_END()

UT_TEST_BEGIN( BoundsN_setSize )
    Bounds5i o( cv_values );
    Size5i sizes( cv_sizes );

    o.setSize( sizes );

    return allEqual( o.data, cv_dimTest );
UT_TEST_END()

UT_TEST_BEGIN( BoundsN_getSize )
    Bounds5i o( cv_dimTest );
    Size5i sizes( cv_valuesAltMaxs );

    sizes = o.getSize();

    return allEqual( sizes.data, cv_sizes );
UT_TEST_END()

UT_TEST_BEGIN( BoundsN_setDimension )
    Bounds5i o( cv_values );

    for( std::size_t i = 0; i < cv_size; ++i )
        o.setDimension( i, (int)(10 + i) );

    return allEqual( o.data, cv_dimTest );
UT_TEST_END()

UT_TEST_BEGIN( BoundsN_getDimension )
    Bounds5i o( cv_dimTest );

    for( std::size_t i = 0; i < cv_size; ++i )
        if( o.getDimension( i ) != (int)( 10 + i ) )
            return false;

    return true;
UT_TEST_END()

UT_TEST_BEGIN( BoundsN_setPositionAndSize_point_size )
    Bounds5i o( cv_valuesAlt );
    Point5i  mins( cv_valuesMins );
    Size5i   sizes( cv_sizes );

    o.setPositionAndSize( mins, sizes );

    return allEqual( o.data, cv_dimTest );
UT_TEST_END()

UT_TEST_BEGIN( BoundsN_getPositionAndSize_point_size )
    Point5i mins( cv_valuesAltMins );
    Size5i sizes( cv_valuesAltMaxs );

    Bounds5i o( cv_dimTest );
    o.getPositionAndSize( mins, sizes );

    return allEqual( mins.data,  cv_valuesMins ) &&
           allEqual( sizes.data, cv_sizes      );
UT_TEST_END()

UT_TEST_BEGIN( BoundsN_getCenter )
    Bounds5i o( cv_valuesAlt );
    Point5i center = o.getCenter();

    return allEqual( center.data, cv_valuesAltCenter );
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

UT_TEST_BEGIN( BoundsN_normalize )
    Bounds5i o( cv_abnormal );

    o.normalize();

    return allEqual( o.data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( BoundsN_isNormal )
    Bounds5i o1( cv_values );
    Bounds5i o2( cv_abnormal );

    return o1.isNormal() == true &&
           o2.isNormal() == false;
UT_TEST_END()

UT_TEST_BEGIN( BoundsN_include )
    Bounds5i o( cv_values );
    Point5i point1( cv_includePt1 );
    Point5i point2( cv_includePt2 );

    o.include( point1 );
    o.include( point2 );

    return allEqual( o.data, cv_includeTest );
UT_TEST_END()

UT_TEST_BEGIN( BoundsN_contains )
    Bounds5i o( cv_values );
    Point5i  point1( cv_valuesMaxs );
    Point5i  point2( cv_valuesAltMaxs );

    return o.contains( point1 ) == true &&
           o.contains( point2 ) == false;
UT_TEST_END()

UT_TEST_BEGIN( BoundsN_contains_mIME )
    Bounds5i o( cv_values );
    Point5i  point1( cv_valuesMins );
    Point5i  point2( cv_valuesMaxs );

    return o.contains_mIME( point1 ) == true &&
           o.contains_mIME( point2 ) == false;
UT_TEST_END()

UT_TEST_BEGIN( BoundsN_intersects )
    Bounds5i o( cv_values );
    Bounds5i bounds1( cv_isTest1 );
    Bounds5i bounds2( cv_isTest2 );
    Bounds5i bounds3( cv_isTest3 );

    return o.intersects( bounds1 ) == true &&
           o.intersects( bounds2 ) == true &&
           o.intersects( bounds3 ) == false;
UT_TEST_END()

UT_TEST_BEGIN( BoundsN_intersects_EE )
    Bounds5i o( cv_values );
    Bounds5i bounds1( cv_isTest1 );
    Bounds5i bounds2( cv_isTestCorner );

    return o.intersects_EE( bounds1 ) == true &&
           o.intersects_EE( bounds2 ) == false;
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

UT_TEST_BEGIN( BoundsN_intersect )
    Bounds5i o( cv_values );
    Bounds5i bounds1( cv_isTest1 );
    Bounds5i bounds2( cv_isTest2 );

    Bounds5i intersect1 = intersect( o, bounds1 );
    Bounds5i intersect2 = intersect( o, bounds2 );

    return allEqual( intersect1.data, cv_intersect1 ) &&
           allEqual( intersect2.data, cv_intersect2 );
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
        o[(std::size_t)-1];
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
        o[(std::size_t)-1];
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
        o.setDimension( (std::size_t)-1, 10 );
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
        o.getDimension( (std::size_t)(-1) );
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
