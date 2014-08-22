/*
test/Bounds3.cpp
-----------------------
Copyright (c) 2014, theJ89

Description:
    Unit tests for Bounds3
*/




//Includes
#include "../Test.hpp"
#include "../utils.hpp"         //allEqual, allEqualTo

#include <brimstone/Bounds.hpp>




namespace {
    using ::Brimstone::Bounds3i;
    using ::Brimstone::Point3i;
    using ::Brimstone::Bounds3f;
    using ::Brimstone::BoundsException;

    const size_t cv_size             = 3;
    const int    cv_zero[6]          {  0,  0,  0,    0,  0,  0 };
    const int    cv_values[6]        {  1,  2,  3,    4,  5,  6 };
    const int    cv_valuesMins[3]    {  1,  2,  3 };
    const int    cv_valuesMaxs[3]    {  4,  5,  6 };
    const int    cv_valuesAlt[6]     {  7,  8,  9,   10, 11, 12 };
    const int    cv_valuesAltMins[3] {  7,  8,  9 };
    const int    cv_valuesAltMaxs[3] { 10, 11, 12 };
    const int    cv_widthTest[6]     {  1,  2,  3,   11,  5,  6 };
    const int    cv_lengthTest[6]    {  1,  2,  3,   4,  13,  6 };
    const int    cv_heightTest[6]    {  1,  2,  3,   4,   5, 15 };
    const int    cv_width            = 10;
    const int    cv_length           = 11;
    const int    cv_height           = 12;
    const int    cv_dimTest[6]       {  1,  2,  3,   11, 13, 15 };
    const int    cv_volume           = cv_width * cv_length * cv_height;
    const int    cv_outsideMins[3]   {  0,  1,  2 };
    const int    cv_outsideMaxs[3]   {  5,  6,  7 };
    const char*  cv_output           = "[ ( 1, 2, 3 ), ( 4, 5, 6 ) ]";

    const float  cv_valuesAltF[6]    { 7.0f, 8.0f, 9.0f,   10.0f, 11.0f, 12.0f };
}

namespace UnitTest {

UT_TEST_BEGIN( Bounds3_constructorFill )
    Bounds3i o( 1 );

    return allEqualTo( o.data, 1 );
UT_TEST_END()

UT_TEST_BEGIN( Bounds3_constructorCppRange )
    Bounds3i o( cv_values );

    return allEqual( o.data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( Bounds3_constructorInitializerList )
    Bounds3i o( {
        cv_values[0],
        cv_values[1],
        cv_values[2],
        cv_values[3],
        cv_values[4],
        cv_values[5]
    } );

    return allEqual( o.data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( Bounds3_setCppRange )
    Bounds3i o( cv_values );

    o.set( cv_valuesAlt );

    return allEqual( o.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( Bounds3_setInitializerList )
    Bounds3i o( cv_values );

    o.set( {
        cv_valuesAlt[0],
        cv_valuesAlt[1],
        cv_valuesAlt[2],
        cv_valuesAlt[3],
        cv_valuesAlt[4],
        cv_valuesAlt[5]
    } );

    return allEqual( o.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( Bounds3_getCppRange )
    int values[ 2*cv_size ];
    copyAll( cv_values, values );
    Bounds3i o( cv_valuesAlt );

    o.get( values );

    return allEqual( o.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( Bounds3_fill )
    Bounds3i o( cv_values );

    o.fill( 1 );

    return allEqualTo( o.data, 1 );
UT_TEST_END()

UT_TEST_BEGIN( Bounds3_begin )
    Bounds3i o( cv_values );

    return o.begin() == std::begin( o.data );
UT_TEST_END()

UT_TEST_BEGIN( Bounds3_beginConst )
    const Bounds3i o( cv_values );

    return o.begin() == std::begin( o.data );
UT_TEST_END()

UT_TEST_BEGIN( Bounds3_cbegin )
    Bounds3i o( cv_values );

    return o.cbegin() == std::cbegin( o.data );
UT_TEST_END()

UT_TEST_BEGIN( Bounds3_end )
    Bounds3i o( cv_values );

    return o.end() == std::end( o.data );
UT_TEST_END()

UT_TEST_BEGIN( Bounds3_endConst )
    const Bounds3i o( cv_values );

    return o.end() == std::end( o.data );
UT_TEST_END()

UT_TEST_BEGIN( Bounds3_cend )
    Bounds3i o( cv_values );

    return o.cend() == std::cend( o.data );
UT_TEST_END()

UT_TEST_BEGIN( Bounds3_rangedFor )
    Bounds3i o( cv_values );

    int idx = 0;
    for( int& i : o )
        i = cv_valuesAlt[idx++];

    return allEqual( o.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( Bounds3_rangedForConst )
    int data[2*cv_size];
    copyAll( cv_values, data );
    const Bounds3i o( cv_valuesAlt );

    int idx = 0;
    for( int i : o )
        data[idx++] = i;

    return allEqual( data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( Bounds3_index )
    Bounds3i o( cv_values );

    for( size_t i = 0; i < 2*cv_size; ++i )
        o[i] = cv_valuesAlt[i];

    return allEqual( o.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( Bounds3_indexConst )
    int data[2*cv_size];
    const Bounds3i o( cv_values );

    for( size_t i = 0; i < 2*cv_size; ++i )
        data[i] = o[i];

    return allEqual( data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( Bounds3_constructorCopy )
    Bounds3f o1( cv_valuesAltF );

    Bounds3i o2( o1 );

    return allEqual( o2.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( Bounds3_assignCopy )
    Bounds3f o1( cv_valuesAltF );
    Bounds3i o2( cv_values );

    o2 = o1;

    return allEqual( o2.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( Bounds3_constructorValues )
    Bounds3i o(
        cv_values[0], cv_values[1], cv_values[2],
        cv_values[3], cv_values[4], cv_values[5]
    );

    return allEqual( o.data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( Bounds3_constructorPointDims )
    Point3i p( cv_values[0], cv_values[1], cv_values[2] );
    Bounds3i o( p, cv_width, cv_length, cv_height );

    return allEqual( o.data, cv_dimTest );
UT_TEST_END()

UT_TEST_BEGIN( Bounds3_constructorMinMax )
    Point3i mins( cv_valuesMins );
    Point3i maxs( cv_valuesMaxs );
    Bounds3i o( mins, maxs );

    return allEqual( o.data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( Bounds3_setValues )
    Bounds3i o( cv_values );

    o.set(
        cv_values[0], cv_values[1], cv_values[2],
        cv_values[3], cv_values[4], cv_values[5]
    );

    return allEqual( o.data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( Bounds3_setMinMax )
    Bounds3i o( cv_values );
    Point3i mins( cv_valuesAltMins );
    Point3i maxs( cv_valuesAltMaxs );

    o.set( mins, maxs );

    return allEqual( o.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( Bounds3_getValues )
    Bounds3i o( cv_values );

    int data[2*cv_size];

    o.get(
        data[0], data[1], data[2],
        data[3], data[4], data[5]
    );

    return allEqual( data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( Bounds3_getMinMax )
    Point3i mins( cv_values );
    Point3i maxs( cv_values );

    Bounds3i o( cv_valuesAlt );
    o.get( mins, maxs );

    return allEqual( mins.data, cv_valuesAltMins ) &&
           allEqual( maxs.data, cv_valuesAltMaxs );
UT_TEST_END()

UT_TEST_BEGIN( Bounds3_setDimensions )
    Bounds3i o( cv_values );

    o.setDimensions( cv_width, cv_length, cv_height );

    return allEqual( o.data, cv_dimTest );
UT_TEST_END()

UT_TEST_BEGIN( Bounds3_setWidth )
    Bounds3i o( cv_values );

    o.setWidth( cv_width );

    return allEqual( o.data, cv_widthTest );
UT_TEST_END()

UT_TEST_BEGIN( Bounds3_setLength )
    Bounds3i o( cv_values );

    o.setLength( cv_length );

    return allEqual( o.data, cv_lengthTest );
UT_TEST_END()

UT_TEST_BEGIN( Bounds3_setHeight )
    Bounds3i o( cv_values );

    o.setHeight( cv_height );

    return allEqual( o.data, cv_heightTest );
UT_TEST_END()

UT_TEST_BEGIN( Bounds3_setDimension )
    Bounds3i o( cv_values );

    for( int i = 0; i < cv_size; ++i )
        o.setDimension( i, 10 + i );

    return allEqual( o.data, cv_dimTest );
UT_TEST_END()

UT_TEST_BEGIN( Bounds3_getDimensions )
    Bounds3i o( cv_dimTest );
    int width, length, height;

    o.getDimensions( width, length, height );

    return width  == cv_width  &&
           length == cv_length &&
           height == cv_height;
UT_TEST_END()

UT_TEST_BEGIN( Bounds3_getWidth )
    Bounds3i o( cv_widthTest );

    int width = o.getWidth();

    return width == cv_width;
UT_TEST_END()

UT_TEST_BEGIN( Bounds3_getLength )
    Bounds3i o( cv_lengthTest );

    int length = o.getLength();

    return length == cv_length;
UT_TEST_END()

UT_TEST_BEGIN( Bounds3_getHeight )
    Bounds3i b( cv_heightTest );

    int height = b.getHeight();

    return height == cv_height;
UT_TEST_END()

UT_TEST_BEGIN( Bounds3_getDimension )
    Bounds3i b( cv_dimTest );

    for( size_t i = 0; i < cv_size; ++i )
        if( b.getDimension( i ) != (int)( 10 + i ) )
            return false;

    return true;
UT_TEST_END()

UT_TEST_BEGIN( Bounds3_getVolume )
    Bounds3i b( cv_dimTest );

    return b.getVolume() == cv_volume;
UT_TEST_END()

UT_TEST_BEGIN( Bounds3_output )
    Bounds3i o( cv_values );

    std::stringstream sout;
    sout << o;

    return sout.str() == cv_output;
UT_TEST_END()

UT_TEST_BEGIN( Bounds3_zero )
    Bounds3i o( cv_values );

    o.zero();

    return allEqual( o.data, cv_zero );
UT_TEST_END()

UT_TEST_BEGIN( Bounds3_isZero )
    Bounds3i o1( cv_zero );
    Bounds3i o2( cv_values );

    return o1.isZero() == true &&
           o2.isZero() == false;
UT_TEST_END()

UT_TEST_BEGIN( Bounds3_equals )
    Bounds3i o1( cv_values );
    Bounds3i o2( cv_values );
    Bounds3i o3( cv_valuesAlt );

    return ( o1 == o2 ) == true   &&
           ( o1 == o3 ) == false;
UT_TEST_END()

UT_TEST_BEGIN( Bounds3_notEquals )
    Bounds3i o1( cv_values );
    Bounds3i o2( cv_values );
    Bounds3i o3( cv_valuesAlt );

    return ( o1 != o2 ) == false  &&
           ( o1 != o3 ) == true;
UT_TEST_END()

UT_TEST_BEGIN( Bounds3_clamp )
    Bounds3i o( cv_values );
    Point3i p1( cv_outsideMins );
    Point3i p2( cv_outsideMaxs );

    clamp( p1, o );
    clamp( p2, o );

    return allEqual( p1.data, cv_valuesMins ) &&
           allEqual( p2.data, cv_valuesMaxs );
UT_TEST_END()

UT_TEST_BEGIN( Bounds3_clampedPoint )
    Bounds3i b( cv_values );
    Point3i p1( cv_outsideMins );
    Point3i p2( cv_outsideMaxs );

    p1 = clampedPoint( p1, b );
    p2 = clampedPoint( p2, b );

    return allEqual( p1.data, cv_valuesMins ) &&
           allEqual( p2.data, cv_valuesMaxs );
UT_TEST_END()




#ifdef BS_ZERO

UT_TEST_BEGIN( Bounds3_constructorZero )
    Bounds3i o;
    return allEqual( o.data, cv_zero );
UT_TEST_END()

#endif //BS_ZERO




#ifdef BS_CHECK_INDEX

UT_TEST_BEGIN( Bounds3_index_OOB )
    Bounds3i o;
    int i;

    try {
        i = o[(size_t)-1];
        return false;
    } catch( const BoundsException& ) {}

    try {
        i = o[6];
        return false;
    } catch( const BoundsException& ) {}

    return true;
UT_TEST_END()

UT_TEST_BEGIN( Bounds3_constIndex_OOB )
    const Bounds3i o;
    int i;

    try {
        i = o[(size_t)-1];
        return false;
    } catch( const BoundsException& ) {}

    try {
        i = o[6];
        return false;
    } catch( const BoundsException& ) {}

    return true;
UT_TEST_END()

UT_TEST_BEGIN( Bounds3_setDimension_OOB )
    Bounds3i o;

    try {
        o.setDimension( (size_t)-1, 10 );
        return false;
    } catch( const BoundsException& ) {}

    try {
        o.setDimension( 3, 10 );
        return false;
    } catch( const BoundsException& ) {}

    return true;
UT_TEST_END()

UT_TEST_BEGIN( Bounds3_getDimension_OOB )
    Bounds3i o;
    int i;

    try {
        i = o.getDimension( (size_t)-1 );
        return false;
    } catch( const BoundsException& ) {}

    try {
        i = o.getDimension( 3 );
        return false;
    } catch( const BoundsException& ) {}

    return true;
UT_TEST_END()

#endif //BS_CHECK_INDEX

}