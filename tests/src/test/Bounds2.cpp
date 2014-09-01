/*
test/Bounds2.cpp
-----------------------
Copyright (c) 2014, theJ89

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
    const int    cv_widthTest[4]     {  1,  2,   11,  4 };
    const int    cv_heightTest[4]    {  1,  2,    3, 13 };
    const int    cv_width            = 10;
    const int    cv_height           = 11;
    const int    cv_dimTest[4]       {  1,  2,   11, 13 };
    const int    cv_area             = cv_width * cv_height;
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

UT_TEST_BEGIN( Bounds2_constructorPointDims )
    Point2i p( cv_values[0], cv_values[1] );
    Bounds2i o( p, 10, 11 );

    return allEqual( o.data, cv_dimTest );
UT_TEST_END()

UT_TEST_BEGIN( Bounds2_constructorMinMax )
    Point2i mins( cv_valuesMins );
    Point2i maxs( cv_valuesMaxs );
    Bounds2i o( mins, maxs );

    return allEqual( o.data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( Bounds2_setValues )
    Bounds2i o( cv_values );

    o.set(
        cv_valuesAlt[0], cv_valuesAlt[1],
        cv_valuesAlt[2], cv_valuesAlt[3]
    );

    return allEqual( o.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( Bounds2_setMinMax )
    Bounds2i o( cv_values );
    Point2i mins( cv_valuesAltMins );
    Point2i maxs( cv_valuesAltMaxs );

    o.set( mins, maxs );

    return allEqual( o.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( Bounds2_getValues )
    Bounds2i o( cv_values );

    int data[2*cv_size];

    o.get(
        data[0], data[1],
        data[2], data[3]
    );

    return allEqual( data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( Bounds2_getMinMax )
    Point2i mins( cv_valuesMins );
    Point2i maxs( cv_valuesMaxs );

    Bounds2i o( cv_valuesAlt );
    o.get( mins, maxs );

    return allEqual( mins.data, cv_valuesAltMins ) &&
           allEqual( maxs.data, cv_valuesAltMaxs );
UT_TEST_END()

UT_TEST_BEGIN( Bounds2_setDimensions )
    Bounds2i o( cv_values );

    o.setDimensions( cv_width, cv_height );

    return allEqual( o.data, cv_dimTest );
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
        o.setDimension( i, 10 + i );

    return allEqual( o.data, cv_dimTest );
UT_TEST_END()

UT_TEST_BEGIN( Bounds2_getDimensions )
    Bounds2i o( cv_dimTest );
    int width, height;

    o.getDimensions( width, height );

    return width  == cv_width &&
           height == cv_height;
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

UT_TEST_BEGIN( Bounds2_getArea )
    Bounds2i o( cv_dimTest );

    return o.getArea() == cv_area;
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