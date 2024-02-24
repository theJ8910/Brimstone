/*
test/Bounds2.cpp
----------------
Copyright (c) 2024, theJ89

Description:
    Unit tests for Bounds2
*/




//Includes
#include "../Test.hpp"
#include "../utils.hpp"         //allEqual

#include <brimstone/Bounds.hpp>




namespace {
    using ::Brimstone::Bounds2i;
    using ::Brimstone::Point2i;
    using ::Brimstone::Size2i;
    using ::Brimstone::Bounds2f;
    using ::Brimstone::BoundsException;

    const size_t cv_size             = 2;
    const int    cv_zero[4]          {  0,  0,    0,  0 };
    const int    cv_values[4]        {  1,  2,    3,  4 };
    const int    cv_valuesMins[2]    {  1,  2 };
    const int    cv_valuesMaxs[2]    {  3,  4 };
    const int    cv_valuesAlt[4]     {  5,  6,    7,  8 };
    const int    cv_valuesAltMins[2] {  5,  6 };
    const int    cv_valuesAltMaxs[2] {  7,  8 };
    const int    cv_abnormal[4]      {  3,  4,    1,  2 };
    const int    cv_includeTest[4]   { -14, -11,   3,  12 };
    const int    cv_includePt1[2]    { -14,  12 };
    const int    cv_includePt2[2]    {  2,  -11 };
    const int    cv_isTest1[4]       {  0,  1,    2,   3 };
    const int    cv_isTest2[4]       {  2,  3,    4,   5 };
    const int    cv_isTest3[4]       {  4,  5,    5,   6 };
    const int    cv_isTestCorner[4]  {  3,  4,    5,   6 };
    const int    cv_intersect1[4]    {  1,  2,    2,   3 };
    const int    cv_intersect2[4]    {  2,  3,    3,   4 };
    const int    cv_widthTest[4]     {  1,  2,   11,  4 };
    const int    cv_heightTest[4]    {  1,  2,    3, 13 };
    const int    cv_sizes[2]         { 10, 11 };
    const int    cv_width            = 10;
    const int    cv_height           = 11;
    const int    cv_dimTest[4]       {  1,  2,   11, 13 };
    const int    cv_outsideMins[2]   {  0,  1 };
    const int    cv_outsideMaxs[2]   {  4,  5 };
    const char*  cv_output           = "[ ( 1, 2 ), ( 3, 4 ) ]";

    const float  cv_valuesAltF[4]    { 5.0f, 6.0f, 7.0f, 8.0f };
}


namespace UnitTest {

UT_TEST_BEGIN( Bounds2_constructorFill )
    Bounds2i o( 1 );

    return allEqualTo( o.data, 1 );
UT_TEST_END()

UT_TEST_BEGIN( Bounds2_constructorCppRange )
    Bounds2i o( cv_values );

    return allEqual( o.data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( Bounds2_constructorInitializerList )
    Bounds2i o( {
        cv_values[0],
        cv_values[1],
        cv_values[2],
        cv_values[3]
    } );

    return allEqual( o.data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( Bounds2_setCppRange )
    Bounds2i o( cv_values );

    o.set( cv_valuesAlt );

    return allEqual( o.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( Bounds2_setInitializerList )
    Bounds2i o( cv_values );

    o.set( {
        cv_valuesAlt[0],
        cv_valuesAlt[1],
        cv_valuesAlt[2],
        cv_valuesAlt[3]
    } );

    return allEqual( o.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( Bounds2_getCppRange )
    int values[ 2*cv_size ];
    copyAll( cv_values, values );
    Bounds2i o( cv_valuesAlt );

    o.get( values );

    return allEqual( o.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( Bounds2_fill )
    Bounds2i o( cv_values );

    o.fill( 1 );

    return allEqualTo( o.data, 1 );
UT_TEST_END()

UT_TEST_BEGIN( Bounds2_begin )
    Bounds2i o( cv_values );

    return o.begin() == std::begin( o.data );
UT_TEST_END()

UT_TEST_BEGIN( Bounds2_beginConst )
    const Bounds2i o( cv_values );

    return o.begin() == std::begin( o.data );
UT_TEST_END()

UT_TEST_BEGIN( Bounds2_cbegin )
    Bounds2i o( cv_values );

    return o.cbegin() == std::begin( o.data );
UT_TEST_END()

UT_TEST_BEGIN( Bounds2_end )
    Bounds2i o( cv_values );

    return o.end() == std::end( o.data );
UT_TEST_END()

UT_TEST_BEGIN( Bounds2_endConst )
    const Bounds2i o( cv_values );

    return o.end() == std::end( o.data );
UT_TEST_END()

UT_TEST_BEGIN( Bounds2_cend )
    Bounds2i o( cv_values );

    return o.cend() == std::end( o.data );
UT_TEST_END()

UT_TEST_BEGIN( Bounds2_rangedFor )
    Bounds2i o( cv_values );

    int idx = 0;
    for( int& i : o )
        i = cv_valuesAlt[idx++];

    return allEqual( o.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( Bounds2_rangedForConst )
    int data[2*cv_size];
    copyAll( cv_values, data );
    const Bounds2i o( cv_valuesAlt );

    int idx = 0;
    for( int i : o )
        data[idx++] = i;

    return allEqual( data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( Bounds2_index )
    Bounds2i o( cv_values );

    for( size_t i = 0; i < 2*cv_size; ++i )
        o[i] = cv_valuesAlt[i];

    return allEqual( o.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( Bounds2_indexConst )
    int data[2*cv_size];
    const Bounds2i o( cv_values );

    for( size_t i = 0; i < 2*cv_size; ++i )
        data[i] = o[i];

    return allEqual( data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( Bounds2_constructorCopy )
    Bounds2f o1( cv_valuesAltF );

    Bounds2i o2( o1 );

    return allEqual( o2.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( Bounds2_assignCopy )
    Bounds2f o1( cv_valuesAltF );
    Bounds2i o2( cv_values );

    o2 = o1;

    return allEqual( o2.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( Bounds2_constructorValues )
    Bounds2i o(
        cv_values[0], cv_values[1],
        cv_values[2], cv_values[3]
    );

    return allEqual( o.data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( Bounds2_constructorMinMax )
    Point2i mins( cv_valuesMins );
    Point2i maxs( cv_valuesMaxs );
    Bounds2i o( mins, maxs );

    return allEqual( o.data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( Bounds2_constructorPosSize )
    Point2i mins( cv_valuesMins );
    Size2i  sizes( cv_sizes );

    Bounds2i o( mins, sizes );

    return allEqual( o.data, cv_dimTest );
UT_TEST_END()

UT_TEST_BEGIN( Bounds2_set_minMax )
    Bounds2i o( cv_valuesAlt );
    Point2i mins( cv_valuesMins );
    Point2i maxs( cv_valuesMaxs );

    o.set( mins, maxs );

    return allEqual( o.data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( Bounds2_get_minMax )
    Point2i mins( cv_valuesMins );
    Point2i maxs( cv_valuesMaxs );

    Bounds2i o( cv_valuesAlt );
    o.get( mins, maxs );

    return allEqual( mins.data, cv_valuesAltMins ) &&
           allEqual( maxs.data, cv_valuesAltMaxs );
UT_TEST_END()

UT_TEST_BEGIN( Bounds2_set_posSize )
    Bounds2i o( cv_valuesAlt );
    Point2i mins( cv_valuesMins );
    Size2i  sizes( cv_sizes );

    o.set( mins, sizes );

    return allEqual( o.data, cv_dimTest );
UT_TEST_END()

UT_TEST_BEGIN( Bounds2_get_posSize )
    Point2i mins( cv_valuesAltMins );
    Size2i  sizes( cv_valuesAltMaxs );

    Bounds2i o( cv_dimTest );
    o.get( mins, sizes );

    return allEqual( mins.data,  cv_valuesMins ) &&
           allEqual( sizes.data, cv_sizes      );
UT_TEST_END()

UT_TEST_BEGIN( Bounds2_set_values )
    Bounds2i o( cv_values );

    o.set(
        cv_valuesAlt[0], cv_valuesAlt[1],
        cv_valuesAlt[2], cv_valuesAlt[3]
    );

    return allEqual( o.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( Bounds2_get_values )
    Bounds2i o( cv_values );

    int data[2*cv_size];

    o.get(
        data[0], data[1],
        data[2], data[3]
    );

    return allEqual( data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( Bounds2_setPosition )
    Bounds2i o( cv_values );
    Point2i  mins( cv_valuesAltMins );

    o.setPosition( mins );

    return allEqual( o.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( Bounds2_getPosition )
    Bounds2i o( cv_values );
    Point2i mins( cv_valuesAltMins );

    mins = o.getPosition();

    return allEqual( mins.data, cv_valuesMins );
UT_TEST_END()

UT_TEST_BEGIN( Bounds2_setPosition_values )
    Bounds2i o( cv_values );

    o.setPosition( cv_valuesAltMins[0], cv_valuesAltMins[1] );

    return allEqual( o.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( Bounds2_getPosition_values )
    Bounds2i o( cv_values );
    int data[cv_size];
    copyAll( cv_valuesAltMins, data );

    o.getPosition( data[0], data[1] );

    return allEqual( data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( Bounds2_setSize )
    Bounds2i o( cv_values );
    Size2i sizes( cv_sizes );

    o.setSize( sizes );

    return allEqual( o.data, cv_dimTest );
UT_TEST_END()

UT_TEST_BEGIN( Bounds2_getSize )
    Bounds2i o( cv_dimTest );
    Size2i sizes( cv_valuesAltMaxs );

    sizes = o.getSize();

    return allEqual( sizes.data, cv_sizes );
UT_TEST_END()

UT_TEST_BEGIN( Bounds2_setSize_values )
    Bounds2i o( cv_values );

    o.setSize( cv_width, cv_height );

    return allEqual( o.data, cv_dimTest );
UT_TEST_END()

UT_TEST_BEGIN( Bounds2_getSize_values )
    Bounds2i o( cv_dimTest );
    int width, height;

    o.getSize( width, height );

    return width  == cv_width &&
           height == cv_height;
UT_TEST_END()

UT_TEST_BEGIN( Bounds2_setWidth )
    Bounds2i o( cv_values );

    o.setWidth( cv_width );

    return allEqual( o.data, cv_widthTest );
UT_TEST_END()

UT_TEST_BEGIN( Bounds2_setHeight )
    Bounds2i o( cv_values );

    o.setHeight( cv_height );

    return allEqual( o.data, cv_heightTest );
UT_TEST_END()

UT_TEST_BEGIN( Bounds2_setDimension )
    Bounds2i o( cv_values );

    for( size_t i = 0; i < cv_size; ++i )
        o.setDimension( i, (int)(10 + i) );

    return allEqual( o.data, cv_dimTest );
UT_TEST_END()

UT_TEST_BEGIN( Bounds2_getWidth )
    Bounds2i o( cv_widthTest );

    int width = o.getWidth();

    return width == cv_width;
UT_TEST_END()

UT_TEST_BEGIN( Bounds2_getHeight )
    Bounds2i o( cv_heightTest );

    int height = o.getHeight();

    return height == cv_height;
UT_TEST_END()

UT_TEST_BEGIN( Bounds2_getDimension )
    Bounds2i o( cv_dimTest );

    for( size_t i = 0; i < cv_size; ++i )
        if( o.getDimension( i ) != (int)( 10 + i ) )
            return false;

    return true;
UT_TEST_END()

UT_TEST_BEGIN( Bounds2_output )
    Bounds2i o( cv_values );

    std::stringstream sout;
    sout << o;

    return sout.str() == cv_output;
UT_TEST_END()

UT_TEST_BEGIN( Bounds2_zero )
    Bounds2i o( cv_values );

    o.zero();

    return allEqual( o.data, cv_zero );
UT_TEST_END()

UT_TEST_BEGIN( Bounds2_isZero )
    Bounds2i o1( cv_zero );
    Bounds2i o2( cv_values );

    return o1.isZero() == true &&
           o2.isZero() == false;
UT_TEST_END()

UT_TEST_BEGIN( Bounds2_normalize )
    Bounds2i o( cv_abnormal );

    o.normalize();

    return allEqual( o.data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( Bounds2_isNormal )
    Bounds2i o1( cv_values );
    Bounds2i o2( cv_abnormal );

    return o1.isNormal() == true &&
           o2.isNormal() == false;
UT_TEST_END()

UT_TEST_BEGIN( Bounds2_include )
    Bounds2i o( cv_values );
    Point2i point1( cv_includePt1 );
    Point2i point2( cv_includePt2 );

    o.include( point1 );
    o.include( point2 );

    return allEqual( o.data, cv_includeTest );
UT_TEST_END()

UT_TEST_BEGIN( Bounds2_contains )
    Bounds2i o( cv_values );
    Point2i  point1( cv_valuesMaxs );
    Point2i  point2( cv_valuesAltMaxs );

    return o.contains( point1 ) == true &&
           o.contains( point2 ) == false;
UT_TEST_END()

UT_TEST_BEGIN( Bounds2_contains_mIME )
    Bounds2i o( cv_values );
    Point2i  point1( cv_valuesMins );
    Point2i  point2( cv_valuesMaxs );

    return o.contains_mIME( point1 ) == true &&
           o.contains_mIME( point2 ) == false;
UT_TEST_END()

UT_TEST_BEGIN( Bounds2_intersects )
    Bounds2i o( cv_values );
    Bounds2i bounds1( cv_isTest1 );
    Bounds2i bounds2( cv_isTest2 );
    Bounds2i bounds3( cv_isTest3 );

    return o.intersects( bounds1 ) == true &&
           o.intersects( bounds2 ) == true &&
           o.intersects( bounds3 ) == false;
UT_TEST_END()

UT_TEST_BEGIN( Bounds2_intersects_EE )
    Bounds2i o( cv_values );
    Bounds2i bounds1( cv_isTest1 );
    Bounds2i bounds2( cv_isTestCorner );

    return o.intersects_EE( bounds1 ) == true &&
           o.intersects_EE( bounds2 ) == false;
UT_TEST_END()

UT_TEST_BEGIN( Bounds2_equals )
    Bounds2i o1( cv_values );
    Bounds2i o2( cv_values );
    Bounds2i o3( cv_valuesAlt );

    return ( o1 == o2 ) == true   &&
           ( o1 == o3 ) == false;
UT_TEST_END()

UT_TEST_BEGIN( Bounds2_notEquals )
    Bounds2i o1( cv_values );
    Bounds2i o2( cv_values );
    Bounds2i o3( cv_valuesAlt );

    return ( o1 != o2 ) == false  &&
           ( o1 != o3 ) == true;
UT_TEST_END()

UT_TEST_BEGIN( Bounds2_clamp )
    Bounds2i o( cv_values );
    Point2i p1( cv_outsideMins );
    Point2i p2( cv_outsideMaxs );

    clamp( p1, o );
    clamp( p2, o );

    return allEqual( p1.data, cv_valuesMins ) &&
           allEqual( p2.data, cv_valuesMaxs );
UT_TEST_END()

UT_TEST_BEGIN( Bounds2_clampedPoint )
    Bounds2i o( cv_values );
    Point2i p1( cv_outsideMins );
    Point2i p2( cv_outsideMaxs );

    p1 = clampedPoint( p1, o );
    p2 = clampedPoint( p2, o );

    return allEqual( p1.data, cv_valuesMins ) &&
           allEqual( p2.data, cv_valuesMaxs );
UT_TEST_END()

UT_TEST_BEGIN( Bounds2_intersect )
    Bounds2i o( cv_values );
    Bounds2i bounds1( cv_isTest1 );
    Bounds2i bounds2( cv_isTest2 );

    Bounds2i intersect1 = intersect( o, bounds1 );
    Bounds2i intersect2 = intersect( o, bounds2 );

    return allEqual( intersect1.data, cv_intersect1 ) &&
           allEqual( intersect2.data, cv_intersect2 );
UT_TEST_END()




#ifdef BS_ZERO

UT_TEST_BEGIN( Bounds2_constructorZero )
    Bounds2i o;
    return allEqual( o.data, cv_zero );
UT_TEST_END()

#endif //BS_ZERO




#ifdef BS_CHECK_INDEX

UT_TEST_BEGIN( Bounds2_index_OOB )
    Bounds2i o;

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

UT_TEST_BEGIN( Bounds2_constIndex_OOB )
    const Bounds2i o;

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

UT_TEST_BEGIN( Bounds2_setDimension_OOB )
    Bounds2i o;

    try {
        o.setDimension( (size_t)-1, 10 );
        return false;
    } catch( const BoundsException& ) {}

    try {
        o.setDimension( 2, 10 );
        return false;
    } catch( const BoundsException& ) {}

    return true;
UT_TEST_END()

UT_TEST_BEGIN( Bounds2_getDimension_OOB )
    Bounds2i o;

    try {
        o.getDimension( (size_t)-1 );
        return false;
    } catch( const BoundsException& ) {}

    try {
        o.getDimension( 2 );
        return false;
    } catch( const BoundsException& ) {}

    return true;
UT_TEST_END()

#endif //BS_CHECK_INDEX

}
