/*
test/Bounds2.cpp
-----------------------
Copyright (c) 2014, theJ89

Description:
    Unit tests for Bounds2
*/

//Includes

//C4996: 'std::_Copy_impl': Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct.
//       To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'
//It's complaining because I'm copying to a pointer instead of an array, and rightfully so because it's inherently unsafe.
//However, we can ignore this warning because the program logic ensures it will never copy values outside of the object.
#define _SCL_SECURE_NO_WARNINGS
#include <algorithm>

#include "../Test.hpp"

#include <brimstone/Bounds.hpp>

namespace {
    const int b2Zero[4]       {  0,  0,    0,  0 };
    const int b2Values[4]     {  1,  2,    3,  4 };
    const int b2ValuesAlt[4]  {  5,  6,    7,  8 };
    const int b2WidthTest[4]  {  1,  2,   11,  4 };
    const int b2HeightTest[4] {  1,  2,    3, 13 };
    const int b2DimTest[4]    {  1,  2,   11, 13 };
}

namespace Brimstone {
namespace UnitTest {

BS_UT_TEST_BEGIN( Bounds2_constructorValues )
    Bounds2i b(
        b2Values[0], b2Values[1],
        b2Values[2], b2Values[3]
    );

    return std::equal( b.data, b.data + 4, b2Values );
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( Bounds2_constructorPointDims )
    Point2i p( b2Values[0], b2Values[1] );
    Bounds2i b( p, 10, 11 );

    return std::equal( b.data, b.data + 4, b2DimTest );
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( Bounds2_constructorMinMax )
    Point2i mins( b2Values,     2 );
    Point2i maxs( b2Values + 2, 2 );
    Bounds2i b( mins, maxs );

    return std::equal( b.data, b.data + 4, b2Values );
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( Bounds2_constructorInitializerList )
    Bounds2i b( { 1, 2, 3, 4 } );

    return std::equal( b.data, b.data + 4, b2Values );
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( Bounds2_constructorArray )
    Bounds2i b( b2Values, 4 );

    return std::equal( b.data, b.data + 4, b2Values );
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( Bounds2_setValues )
    Bounds2i b( b2Values, 4 );

    b.set(
        b2Values[0], b2Values[1],
        b2Values[2], b2Values[3]
    );

    return std::equal( b.data, b.data + 4, b2Values );
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( Bounds2_setArray )
    Bounds2i b( b2Values, 4 );

    b.set( b2ValuesAlt, 4 );

    return std::equal( b.data, b.data + 4, b2ValuesAlt );
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( Bounds2_setMinMax )
    Bounds2i b( b2Values, 4 );
    Point2i mins( b2ValuesAlt,     2 );
    Point2i maxs( b2ValuesAlt + 2, 2 );

    b.set( mins, maxs );

    return std::equal( b.data, b.data + 4, b2ValuesAlt );
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( Bounds2_getValues )
    Bounds2i b( b2Values, 4 );

    int data[4];

    b.get(
        data[0], data[1],
        data[2], data[3]
    );

    return std::equal( data, data + 4, b2Values );
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( Bounds2_getArray )
    int data[4];
    std::copy( b2Values, b2Values + 4, data );

    Bounds2i b( b2ValuesAlt, 4 );
    b.get( data, 4 );

    return std::equal( data, data + 4, b2ValuesAlt );
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( Bounds2_getMinMax )
    Point2i mins( b2Values,     2 );
    Point2i maxs( b2Values + 2, 2 );

    Bounds2i b( b2ValuesAlt, 4 );
    b.get( mins, maxs );

    return std::equal( mins.data, mins.data + 2, b2ValuesAlt     ) &&
           std::equal( maxs.data, maxs.data + 2, b2ValuesAlt + 2 );
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( Bounds2_setDimensions )
    Bounds2i b( b2Values, 4 );

    b.setDimensions( 10, 11 );

    return std::equal( b.data, b.data + 4, b2DimTest );
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( Bounds2_setWidth )
    Bounds2i b( b2Values, 4 );

    b.setWidth( 10 );

    return std::equal( b.data, b.data + 4, b2WidthTest );
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( Bounds2_setHeight )
    Bounds2i b( b2Values, 4 );

    b.setHeight( 11 );

    return std::equal( b.data, b.data + 4, b2HeightTest );
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( Bounds2_setDimension )
    Bounds2i b( b2Values, 4 );

    for( int i = 0; i < 2; ++i )
        b.setDimension( i, 10 + i );

    return std::equal( b.data, b.data + 4, b2DimTest );
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( Bounds2_getDimensions )
    Bounds2i b( b2DimTest, 4 );
    int width, height;

    b.getDimensions( width, height );

    return width  == 10 &&
           height == 11;
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( Bounds2_getWidth )
    Bounds2i b( b2WidthTest, 4 );

    int width = b.getWidth();

    return width == 10;
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( Bounds2_getHeight )
    Bounds2i b( b2HeightTest, 4 );

    int height = b.getHeight();

    return height == 11;
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( Bounds2_getDimension )
    Bounds2i b( b2DimTest, 4 );

    for( int i = 0; i < 2; ++i )
        if( b.getDimension( i ) != 10 + i )
            return false;

    return true;
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( Bounds2_getVolume )
    Bounds2i b( b2DimTest, 4 );

    return b.getArea() == 10 * 11;
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( Bounds2_output )
    Bounds2i b( b2Values, 4 );

    std::stringstream sout;
    sout << b;

    return sout.str() == "[ ( 1, 2 ), ( 3, 4 ) ]";
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( Bounds2_zero )
    Bounds2i b( b2Values, 4 );

    b.zero();

    return std::equal( b.data, b.data + 4, b2Zero );
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( Bounds2_isZero )
    Bounds2i b1( b2Zero,   4 );
    Bounds2i b2( b2Values, 4 );

    return b1.isZero() == true &&
           b2.isZero() == false;
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( Bounds2_equals )
    Bounds2i b1( b2Values,    4 );
    Bounds2i b2( b2Values,    4 );
    Bounds2i b3( b2ValuesAlt, 4 );

    return ( b1 == b2 ) == true   &&
           ( b1 == b3 ) == false;
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( Bounds2_notEquals )
    Bounds2i b1( b2Values,    4 );
    Bounds2i b2( b2Values,    4 );
    Bounds2i b3( b2ValuesAlt, 4 );

    return ( b1 != b2 ) == false  &&
           ( b1 != b3 ) == true;
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( Bounds2_clamp )
    Bounds2i b( b2Values, 4 );
    Point2i p1( { 0, 1 } );
    Point2i p2( { 3, 4 } );

    clamp( p1, b );
    clamp( p2, b );

    return std::equal( p1.data, p1.data + 2, b2Values     ) &&
           std::equal( p2.data, p2.data + 2, b2Values + 2 );
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( Bounds2_clampedPoint )
    Bounds2i b( b2Values, 4 );
    Point2i p1( { 0, 1 } );
    Point2i p2( { 3, 4 } );

    p1 = clampedPoint( p1, b );
    p2 = clampedPoint( p2, b );

    return std::equal( p1.data, p1.data + 2, b2Values     ) &&
           std::equal( p2.data, p2.data + 2, b2Values + 2 );
BS_UT_TEST_END()




#ifdef BS_ZERO

BS_UT_TEST_BEGIN( Bounds2_constructorZero )
    Bounds2i b;
    return b.isZero();
BS_UT_TEST_END()

#endif //BS_ZERO




#ifdef BS_CHECK_NULLPTR

BS_UT_TEST_BEGIN( Bounds2_constructorArray_nullPointer )
    try {
        Bounds2i b( nullptr, 4 );
    } catch( const NullPointerException& ) {
        return true;
    }
    return false;
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( Bounds2_setArray_nullPointer )
    Bounds2i b( b2Values, 4 );
    try {
        b.set( nullptr, 4 );
    } catch( const NullPointerException& ) {
        return true;
    }
    return false;
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( Bounds2_getArray_nullPointer )
    Bounds2i pt( b2Values, 4 );
    try {
        pt.get( nullptr, 4 );
    } catch( const NullPointerException& ) {
        return true;
    }
    return false;
BS_UT_TEST_END()

#endif //BS_CHECK_NULLPTR




#ifdef BS_CHECK_SIZE

BS_UT_TEST_BEGIN( Bounds2_constructorArray_invalidSize )
    try {
        Bounds2i pt( b2Values, 0 );
    } catch( const SizeException& ) {
        return true;
    }
    return false;
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( Bounds2_setArray_invalidSize )
    Bounds2i b;
    try {
        b.set( b2ValuesAlt, 0 );
    } catch( const SizeException& ) {
        return true;
    }
    return false;
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( Bounds2_getArray_invalidSize )
    int data[4];
    Bounds2i b( b2Values, 4 );
    try {
        b.get( data, 0 );
    } catch( const SizeException& ) {
        return true;
    }
    return false;
BS_UT_TEST_END()

#endif //BS_CHECK_SIZE




#ifdef BS_CHECK_INDEX

BS_UT_TEST_BEGIN( Bounds2_setDimension_OOB )
    Bounds2i b;

    try {
        b.setDimension( -1, 10 );
        return false;
    } catch( const OutOfBoundsException& ) {}

    try {
        b.setDimension( 2, 10 );
        return false;
    } catch( const OutOfBoundsException& ) {}

    return true;
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( Bounds2_getDimension_OOB )
    Bounds2i b;
    int i;

    try {
        i = b.getDimension( -1 );
        return false;
    } catch( const OutOfBoundsException& ) {}

    try {
        i = b.getDimension( 2 );
        return false;
    } catch( const OutOfBoundsException& ) {}

    return true;
BS_UT_TEST_END()

#endif //BS_CHECK_INDEX

}
}