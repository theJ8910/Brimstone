/*
test/Bounds4.cpp
----------------
Copyright (c) 2024, theJ89

Description:
    Unit tests for Bounds4
*/




//Includes
#include "../Test.hpp"
#include "../utils.hpp"         //allEqual, allEqualTo

#include <cstddef>              //std::size_t

#include <brimstone/Bounds.hpp>




namespace {
    using ::Brimstone::Bounds4i;
    using ::Brimstone::Point4i;
    using ::Brimstone::Size4i;
    using ::Brimstone::Bounds4f;
    using ::Brimstone::BoundsException;

    const std::size_t cv_size               = 4;
    const int         cv_zero[8]            {  0,  0,  0,  0,   0,  0,  0,  0 };
    const int         cv_values[8]          {  1,  2,  3,  4,   5,  6,  7,  8 };
    const int         cv_valuesMins[4]      {  1,  2,  3,  4 };
    const int         cv_valuesMaxs[4]      {  5,  6,  7,  8 };
    const int         cv_valuesAlt[8]       {  9, 10, 11, 12,  13, 14, 15, 16 };
    const int         cv_valuesAltMins[4]   {  9, 10, 11, 12 };
    const int         cv_valuesAltMaxs[4]   { 13, 14, 15, 16 };
    const int         cv_valuesAltCenter[4] { 11, 12, 13, 14 };
    const int         cv_abnormal[8]        {   5,   6,   7,  8,   1,   2,  3,   4 };
    const int         cv_includeTest[8]     { -14, -11, -12, -4,   5,  12,  7,  13 };
    const int         cv_includePt1[4]      { -14,  12,   2, -4 };
    const int         cv_includePt2[4]      {  2,  -11, -12, 13 };
    const int         cv_isTest1[8]         { -1,  0,  1,  2,    3,   4,   5,   6 };
    const int         cv_isTest2[8]         {  3,  4,  5,  6,    7,   8,   9,  10 };
    const int         cv_isTest3[8]         {  6,  7,  8,  9,   10,  11,  12,  13 };
    const int         cv_isTestCorner[8]    {  5,  6,  7,  8,    9,  10,  11,  12 };
    const int         cv_intersect1[8]      {  1,  2,  3,  4,    3,   4,   5,   6 };
    const int         cv_intersect2[8]      {  3,  4,  5,  6,    5,   6,   7,   8 };
    const int         cv_widthTest[8]       {  1,  2,  3,  4,  11,  6,  7,  8 };
    const int         cv_lengthTest[8]      {  1,  2,  3,  4,   5, 13,  7,  8 };
    const int         cv_heightTest[8]      {  1,  2,  3,  4,   5,  6, 15,  8 };
    const int         cv_depthTest[8]       {  1,  2,  3,  4,   5,  6,  7, 17 };
    const int         cv_sizes[4]           { 10, 11, 12, 13 };
    const int         cv_valuesMinsSizes[8] { 1, 2, 3, 4, 10, 11, 12, 13 };
    const int         cv_width              = 10;
    const int         cv_length             = 11;
    const int         cv_height             = 12;
    const int         cv_depth              = 13;
    const int         cv_dimTest[8]         {  1,  2,  3,  4,  11, 13, 15, 17 };
    const int         cv_outsideMins[4]     {  0,  1,  2,  3 };
    const int         cv_outsideMaxs[4]     {  6,  7,  8,  9 };
    const char*       cv_output             = "[ ( 1, 2, 3, 4 ), ( 5, 6, 7, 8 ) ]";

    const float       cv_valuesAltF[8]      { 9.0f, 10.0f, 11.0f, 12.0f,  13.0f, 14.0f, 15.0f, 16.0f };
}

namespace UnitTest {

UT_TEST_BEGIN( Bounds4_constructorFill )
    Bounds4i o( 1 );

    return allEqualTo( o.data, 1 );
UT_TEST_END()

UT_TEST_BEGIN( Bounds4_constructorCppRange )
    Bounds4i o( cv_values );

    return allEqual( o.data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( Bounds4_constructorInitializerList )
    Bounds4i o( {
        cv_values[0],
        cv_values[1],
        cv_values[2],
        cv_values[3],
        cv_values[4],
        cv_values[5],
        cv_values[6],
        cv_values[7]
    } );

    return allEqual( o.data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( Bounds4_setCppRange )
    Bounds4i o( cv_values );

    o.set( cv_valuesAlt );

    return allEqual( o.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( Bounds4_setInitializerList )
    Bounds4i o( cv_values );

    o.set( {
        cv_valuesAlt[0],
        cv_valuesAlt[1],
        cv_valuesAlt[2],
        cv_valuesAlt[3],
        cv_valuesAlt[4],
        cv_valuesAlt[5],
        cv_valuesAlt[6],
        cv_valuesAlt[7]
    } );

    return allEqual( o.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( Bounds4_getCppRange )
    int values[ 2*cv_size ];
    copyAll( cv_values, values );
    Bounds4i o( cv_valuesAlt );

    o.get( values );

    return allEqual( o.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( Bounds4_fill )
    Bounds4i o( cv_values );

    o.fill( 1 );

    return allEqualTo( o.data, 1 );
UT_TEST_END()

UT_TEST_BEGIN( Bounds4_begin )
    Bounds4i o( cv_values );

    return o.begin() == std::begin( o.data );
UT_TEST_END()

UT_TEST_BEGIN( Bounds4_beginConst )
    const Bounds4i o( cv_values );

    return o.begin() == std::begin( o.data );
UT_TEST_END()

UT_TEST_BEGIN( Bounds4_cbegin )
    Bounds4i o( cv_values );

    return o.cbegin() == std::begin( o.data );
UT_TEST_END()

UT_TEST_BEGIN( Bounds4_end )
    Bounds4i o( cv_values );

    return o.end() == std::end( o.data );
UT_TEST_END()

UT_TEST_BEGIN( Bounds4_endConst )
    const Bounds4i o( cv_values );

    return o.end() == std::end( o.data );
UT_TEST_END()

UT_TEST_BEGIN( Bounds4_cend )
    Bounds4i o( cv_values );

    return o.cend() == std::end( o.data );
UT_TEST_END()

UT_TEST_BEGIN( Bounds4_rangedFor )
    Bounds4i o( cv_values );

    int idx = 0;
    for( int& i : o )
        i = cv_valuesAlt[idx++];

    return allEqual( o.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( Bounds4_rangedForConst )
    int data[2*cv_size];
    copyAll( cv_values, data );
    const Bounds4i o( cv_valuesAlt );

    int idx = 0;
    for( int i : o )
        data[idx++] = i;

    return allEqual( data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( Bounds4_index )
    Bounds4i o( cv_values );

    for( std::size_t i = 0; i < 2*cv_size; ++i )
        o[i] = cv_valuesAlt[i];

    return allEqual( o.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( Bounds4_indexConst )
    int data[2*cv_size];
    const Bounds4i o( cv_values );

    for( std::size_t i = 0; i < 2*cv_size; ++i )
        data[i] = o[i];

    return allEqual( data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( Bounds4_constructorCopy )
    Bounds4f o1( cv_valuesAltF );

    Bounds4i o2( o1 );

    return allEqual( o2.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( Bounds4_assignCopy )
    Bounds4f o1( cv_valuesAltF );
    Bounds4i o2( cv_values );

    o2 = o1;

    return allEqual( o2.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( Bounds4_constructorValues )
    Bounds4i o(
        cv_values[0], cv_values[1], cv_values[2], cv_values[3],
        cv_values[4], cv_values[5], cv_values[6], cv_values[7]
    );

    return allEqual( o.data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( Bounds4_constructorMinMax )
    Point4i mins( cv_valuesMins );
    Point4i maxs( cv_valuesMaxs );
    Bounds4i o( mins, maxs );

    return allEqual( o.data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( Bounds4_constructorPosSize )
    Point4i mins( cv_valuesMins );
    Size4i  sizes( cv_sizes );

    Bounds4i o( mins, sizes );

    return allEqual( o.data, cv_dimTest );
UT_TEST_END()

UT_TEST_BEGIN( Bounds4_set_minMax )
    Bounds4i o( cv_valuesAlt );
    Point4i mins( cv_valuesMins );
    Point4i maxs( cv_valuesMaxs );

    o.set( mins, maxs );

    return allEqual( o.data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( Bounds4_get_minMax )
    Point4i mins( cv_valuesMins );
    Point4i maxs( cv_valuesMaxs );

    Bounds4i o( cv_valuesAlt );
    o.get( mins, maxs );

    return allEqual( mins.data, cv_valuesAltMins ) &&
           allEqual( maxs.data, cv_valuesAltMaxs );
UT_TEST_END()

UT_TEST_BEGIN( Bounds4_set_values )
    Bounds4i o( cv_values );

    o.set(
        cv_values[0], cv_values[1], cv_values[2], cv_values[3],
        cv_values[4], cv_values[5], cv_values[6], cv_values[7]
    );

    return allEqual( o.data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( Bounds4_get_values )
    Bounds4i o( cv_values );

    int data[2*cv_size] = {
        cv_valuesAlt[0], cv_valuesAlt[1], cv_valuesAlt[2], cv_valuesAlt[3],
        cv_valuesAlt[4], cv_valuesAlt[5], cv_valuesAlt[6], cv_valuesAlt[7]
    };;

    o.get(
        data[0], data[1], data[2], data[3],
        data[4], data[5], data[6], data[7]
    );

    return allEqual( data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( Bounds4_setPosition )
    Bounds4i o( cv_values );
    Point4i  mins( cv_valuesAltMins );

    o.setPosition( mins );

    return allEqual( o.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( Bounds4_getPosition )
    Bounds4i o( cv_values );
    Point4i mins( cv_valuesAltMins );

    mins = o.getPosition();

    return allEqual( mins.data, cv_valuesMins );
UT_TEST_END()

UT_TEST_BEGIN( Bounds4_setPosition_values )
    Bounds4i o( cv_values );

    o.setPosition( cv_valuesAltMins[0], cv_valuesAltMins[1], cv_valuesAltMins[2], cv_valuesAltMins[3] );

    return allEqual( o.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( Bounds4_getPosition_values )
    Bounds4i o( cv_values );
    int data[cv_size];
    copyAll( cv_valuesAltMins, data );

    o.getPosition( data[0], data[1], data[2], data[3] );

    return allEqual( data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( Bounds4_setSize )
    Bounds4i o( cv_values );
    Size4i sizes( cv_sizes );

    o.setSize( sizes );

    return allEqual( o.data, cv_dimTest );
UT_TEST_END()

UT_TEST_BEGIN( Bounds4_getSize )
    Bounds4i o( cv_dimTest );
    Size4i sizes( cv_valuesAltMaxs );

    sizes = o.getSize();

    return allEqual( sizes.data, cv_sizes );
UT_TEST_END()

UT_TEST_BEGIN( Bounds4_setSize_values )
    Bounds4i o( cv_values );

    o.setSize( cv_width, cv_length, cv_height, cv_depth );

    return allEqual( o.data, cv_dimTest );
UT_TEST_END()

UT_TEST_BEGIN( Bounds4_getSize_values )
    Bounds4i o( cv_dimTest );
    int width, length, height, depth;

    o.getSize( width, length, height, depth );

    return width  == cv_width   &&
           length == cv_length  &&
           height == cv_height  &&
           depth  == cv_depth;
UT_TEST_END()

UT_TEST_BEGIN( Bounds4_setWidth )
    Bounds4i o( cv_values );

    o.setWidth( cv_width );

    return allEqual( o.data, cv_widthTest );
UT_TEST_END()

UT_TEST_BEGIN( Bounds4_setLength )
    Bounds4i o( cv_values );

    o.setLength( cv_length );

    return allEqual( o.data, cv_lengthTest );
UT_TEST_END()

UT_TEST_BEGIN( Bounds4_setHeight )
    Bounds4i o( cv_values );

    o.setHeight( cv_height );

    return allEqual( o.data, cv_heightTest );
UT_TEST_END()

UT_TEST_BEGIN( Bounds4_setDepth )
    Bounds4i o( cv_values );

    o.setDepth( cv_depth );

    return allEqual( o.data, cv_depthTest );
UT_TEST_END()

UT_TEST_BEGIN( Bounds4_setDimension )
    Bounds4i o( cv_values );

    for( std::size_t i = 0; i < cv_size; ++i )
        o.setDimension( i, (int)(10 + i) );

    return allEqual( o.data, cv_dimTest );
UT_TEST_END()

UT_TEST_BEGIN( Bounds4_getWidth )
    Bounds4i o( cv_widthTest );

    int width = o.getWidth();

    return width == cv_width;
UT_TEST_END()

UT_TEST_BEGIN( Bounds4_getLength )
    Bounds4i o( cv_lengthTest );

    int length = o.getLength();

    return length == cv_length;
UT_TEST_END()

UT_TEST_BEGIN( Bounds4_getHeight )
    Bounds4i b( cv_heightTest );

    int height = b.getHeight();

    return height == cv_height;
UT_TEST_END()

UT_TEST_BEGIN( Bounds4_getDepth )
    Bounds4i b( cv_depthTest );

    int depth = b.getDepth();

    return depth == cv_depth;
UT_TEST_END()

UT_TEST_BEGIN( Bounds4_getDimension )
    Bounds4i b( cv_dimTest );

    for( std::size_t i = 0; i < cv_size; ++i )
        if( b.getDimension( i ) != (int)( 10 + i ) )
            return false;

    return true;
UT_TEST_END()

UT_TEST_BEGIN( Bounds4_setPositionAndSize_point_size )
    Bounds4i o( cv_valuesAlt );
    Point4i  mins( cv_valuesMins );
    Size4i   sizes( cv_sizes );

    o.setPositionAndSize( mins, sizes );

    return allEqual( o.data, cv_dimTest );
UT_TEST_END()

UT_TEST_BEGIN( Bounds4_getPositionAndSize_point_size )
    Point4i mins( cv_valuesAltMins );
    Size4i  sizes( cv_valuesAltMaxs );

    Bounds4i o( cv_dimTest );
    o.getPositionAndSize( mins, sizes );

    return allEqual( mins.data,  cv_valuesMins ) &&
           allEqual( sizes.data, cv_sizes      );
UT_TEST_END()

UT_TEST_BEGIN( Bounds4_setPositionAndSize_point_w_l_h_d )
    Bounds4i o( cv_valuesAlt );
    Point4i  mins( cv_valuesMins );
    o.setPositionAndSize(
        mins,
        cv_sizes[0], cv_sizes[1], cv_sizes[2], cv_sizes[3]
    );

    return allEqual( o.data, cv_dimTest );
UT_TEST_END()

UT_TEST_BEGIN( Bounds4_getPositionAndSize_point_w_l_h_d )
    Point4i mins( cv_valuesAltMins );
    int     data[cv_size] { cv_valuesAlt[4], cv_valuesAlt[5], cv_valuesAlt[6], cv_valuesAlt[7] };

    Bounds4i o( cv_dimTest );
    o.getPositionAndSize(
        mins,
        data[0], data[1], data[2], data[3]
    );

    return allEqual( mins.data, cv_valuesMins ) &&
           allEqual( data,      cv_sizes      );
UT_TEST_END()

UT_TEST_BEGIN( Bounds4_setPositionAndSize_x_y_z_w_size )
    Bounds4i o( cv_valuesAlt );
    Size4i   sizes( cv_sizes );

    o.setPositionAndSize( cv_values[0], cv_values[1], cv_values[2], cv_values[3], sizes );

    return allEqual( o.data, cv_dimTest );
UT_TEST_END()

UT_TEST_BEGIN( Bounds4_getPositionAndSize_x_y_z_w_size )
    int     data[cv_size] { cv_valuesAlt[0], cv_valuesAlt[1], cv_valuesAlt[2], cv_valuesAlt[3] };
    Size4i  sizes( cv_valuesAltMaxs );

    Bounds4i o( cv_dimTest );
    o.getPositionAndSize( data[0], data[1], data[2], data[3], sizes );

    return allEqual( data,       cv_valuesMins ) &&
           allEqual( sizes.data, cv_sizes      );
UT_TEST_END()

UT_TEST_BEGIN( Bounds4_setPositionAndSize_x_y_z_w_w_l_h_d )
    Bounds4i o( cv_valuesAlt );
    o.setPositionAndSize(
        cv_valuesMinsSizes[0], cv_valuesMinsSizes[1], cv_valuesMinsSizes[2], cv_valuesMinsSizes[3],
        cv_valuesMinsSizes[4], cv_valuesMinsSizes[5], cv_valuesMinsSizes[6], cv_valuesMinsSizes[7]
    );

    return allEqual( o.data, cv_dimTest );
UT_TEST_END()

UT_TEST_BEGIN( Bounds4_getPositionAndSize_x_y_z_w_w_l_h_d )
    int data[2*cv_size] {
        cv_valuesAlt[0], cv_valuesAlt[1], cv_valuesAlt[2], cv_valuesAlt[3],
        cv_valuesAlt[4], cv_valuesAlt[5], cv_valuesAlt[6], cv_valuesAlt[7]
    };

    Bounds4i o( cv_dimTest );
    o.getPositionAndSize(
        data[0], data[1], data[2], data[3],
        data[4], data[5], data[6], data[7]
    );

    return allEqual( data, cv_valuesMinsSizes );
UT_TEST_END()

UT_TEST_BEGIN( Bounds4_getCenter )
    Bounds4i o( cv_valuesAlt );
    Point4i center = o.getCenter();

    return allEqual( center.data, cv_valuesAltCenter );
UT_TEST_END()

UT_TEST_BEGIN( Bounds4_output )
    Bounds4i o( cv_values );

    std::stringstream sout;
    sout << o;

    return sout.str() == cv_output;
UT_TEST_END()

UT_TEST_BEGIN( Bounds4_zero )
    Bounds4i o( cv_values );

    o.zero();

    return allEqual( o.data, cv_zero );
UT_TEST_END()

UT_TEST_BEGIN( Bounds4_isZero )
    Bounds4i o1( cv_zero );
    Bounds4i o2( cv_values );

    return o1.isZero() == true &&
           o2.isZero() == false;
UT_TEST_END()

UT_TEST_BEGIN( Bounds4_normalize )
    Bounds4i o( cv_abnormal );

    o.normalize();

    return allEqual( o.data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( Bounds4_isNormal )
    Bounds4i o1( cv_values );
    Bounds4i o2( cv_abnormal );

    return o1.isNormal() == true &&
           o2.isNormal() == false;
UT_TEST_END()

UT_TEST_BEGIN( Bounds4_include )
    Bounds4i o( cv_values );
    Point4i point1( cv_includePt1 );
    Point4i point2( cv_includePt2 );

    o.include( point1 );
    o.include( point2 );

    return allEqual( o.data, cv_includeTest );
UT_TEST_END()

UT_TEST_BEGIN( Bounds4_contains )
    Bounds4i o( cv_values );
    Point4i  point1( cv_valuesMaxs );
    Point4i  point2( cv_valuesAltMaxs );

    return o.contains( point1 ) == true &&
           o.contains( point2 ) == false;
UT_TEST_END()

UT_TEST_BEGIN( Bounds4_contains_mIME )
    Bounds4i o( cv_values );
    Point4i  point1( cv_valuesMins );
    Point4i  point2( cv_valuesMaxs );

    return o.contains_mIME( point1 ) == true &&
           o.contains_mIME( point2 ) == false;
UT_TEST_END()

UT_TEST_BEGIN( Bounds4_intersects )
    Bounds4i o( cv_values );
    Bounds4i bounds1( cv_isTest1 );
    Bounds4i bounds2( cv_isTest2 );
    Bounds4i bounds3( cv_isTest3 );

    return o.intersects( bounds1 ) == true &&
           o.intersects( bounds2 ) == true &&
           o.intersects( bounds3 ) == false;
UT_TEST_END()

UT_TEST_BEGIN( Bounds4_intersects_EE )
    Bounds4i o( cv_values );
    Bounds4i bounds1( cv_isTest1 );
    Bounds4i bounds2( cv_isTestCorner );

    return o.intersects_EE( bounds1 ) == true &&
           o.intersects_EE( bounds2 ) == false;
UT_TEST_END()

UT_TEST_BEGIN( Bounds4_equals )
    Bounds4i o1( cv_values );
    Bounds4i o2( cv_values );
    Bounds4i o3( cv_valuesAlt );

    return ( o1 == o2 ) == true   &&
           ( o1 == o3 ) == false;
UT_TEST_END()

UT_TEST_BEGIN( Bounds4_notEquals )
    Bounds4i o1( cv_values );
    Bounds4i o2( cv_values );
    Bounds4i o3( cv_valuesAlt );

    return ( o1 != o2 ) == false  &&
           ( o1 != o3 ) == true;
UT_TEST_END()

UT_TEST_BEGIN( Bounds4_clamp )
    Bounds4i o( cv_values );
    Point4i p1( cv_outsideMins );
    Point4i p2( cv_outsideMaxs );

    clamp( p1, o );
    clamp( p2, o );

    return allEqual( p1.data, cv_valuesMins ) &&
           allEqual( p2.data, cv_valuesMaxs );
UT_TEST_END()

UT_TEST_BEGIN( Bounds4_clampedPoint )
    Bounds4i b( cv_values );
    Point4i p1( cv_outsideMins );
    Point4i p2( cv_outsideMaxs );

    p1 = clampedPoint( p1, b );
    p2 = clampedPoint( p2, b );

    return allEqual( p1.data, cv_valuesMins ) &&
           allEqual( p2.data, cv_valuesMaxs );
UT_TEST_END()

UT_TEST_BEGIN( Bounds4_intersect )
    Bounds4i o( cv_values );
    Bounds4i bounds1( cv_isTest1 );
    Bounds4i bounds2( cv_isTest2 );

    Bounds4i intersect1 = intersect( o, bounds1 );
    Bounds4i intersect2 = intersect( o, bounds2 );

    return allEqual( intersect1.data, cv_intersect1 ) &&
           allEqual( intersect2.data, cv_intersect2 );
UT_TEST_END()




#ifdef BS_ZERO

UT_TEST_BEGIN( Bounds4_constructorZero )
    Bounds4i o;
    return allEqual( o.data, cv_zero );
UT_TEST_END()

#endif //BS_ZERO




#ifdef BS_CHECK_INDEX

UT_TEST_BEGIN( Bounds4_index_OOB )
    Bounds4i o;

    try {
        o[(std::size_t)-1];
        return false;
    } catch( const BoundsException& ) {}

    try {
        o[8];
        return false;
    } catch( const BoundsException& ) {}

    return true;
UT_TEST_END()

UT_TEST_BEGIN( Bounds4_constIndex_OOB )
    const Bounds4i o;

    try {
        o[(std::size_t)-1];
        return false;
    } catch( const BoundsException& ) {}

    try {
        o[8];
        return false;
    } catch( const BoundsException& ) {}

    return true;
UT_TEST_END()

UT_TEST_BEGIN( Bounds4_setDimension_OOB )
    Bounds4i o;

    try {
        o.setDimension( (std::size_t)-1, 10 );
        return false;
    } catch( const BoundsException& ) {}

    try {
        o.setDimension( 4, 10 );
        return false;
    } catch( const BoundsException& ) {}

    return true;
UT_TEST_END()

UT_TEST_BEGIN( Bounds4_getDimension_OOB )
    Bounds4i o;

    try {
        o.getDimension( (std::size_t)-1 );
        return false;
    } catch( const BoundsException& ) {}

    try {
        o.getDimension( 4 );
        return false;
    } catch( const BoundsException& ) {}

    return true;
UT_TEST_END()

#endif //BS_CHECK_INDEX

}
