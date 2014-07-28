/*
test/Bounds3.cpp
-----------------------
Copyright (c) 2014, theJ89

Description:
    Unit tests for Bounds3
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
    const int b3Zero[6]       {  0,  0,  0,    0,  0,  0 };
    const int b3Values[6]     {  1,  2,  3,    4,  5,  6 };
    const int b3ValuesAlt[6]  {  7,  8,  9,   10, 11, 12 };
    const int b3WidthTest[6]  {  1,  2,  3,   11,  5,  6 };
    const int b3LengthTest[6] {  1,  2,  3,   4,  13,  6 };
    const int b3HeightTest[6] {  1,  2,  3,   4,   5, 15 };
    const int b3DimTest[6]    {  1,  2,  3,   11, 13, 15 };
}

namespace Brimstone {
namespace UnitTest {

BS_UT_TEST_BEGIN( Bounds3_constructorValues )
    Bounds3i b(
        b3Values[0], b3Values[1], b3Values[2],
        b3Values[3], b3Values[4], b3Values[5]
    );

    return std::equal( b.data, b.data + 6, b3Values );
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( Bounds3_constructorPointDims )
    Point3i p( b3Values[0], b3Values[1], b3Values[2] );
    Bounds3i b( p, 10, 11, 12 );

    return std::equal( b.data, b.data + 6, b3DimTest );
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( Bounds3_constructorMinMax )
    Point3i mins( b3Values,     3 );
    Point3i maxs( b3Values + 3, 3 );
    Bounds3i b( mins, maxs );

    return std::equal( b.data, b.data + 6, b3Values );
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( Bounds3_constructorInitializerList )
    Bounds3i b( { 1, 2, 3, 4, 5, 6 } );

    return std::equal( b.data, b.data + 6, b3Values );
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( Bounds3_constructorArray )
    Bounds3i b( b3Values, 6 );

    return std::equal( b.data, b.data + 6, b3Values );
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( Bounds3_setValues )
    Bounds3i b( b3Values, 6 );

    b.set(
        b3Values[0], b3Values[1], b3Values[2],
        b3Values[3], b3Values[4], b3Values[5]
    );

    return std::equal( b.data, b.data + 6, b3Values );
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( Bounds3_setArray )
    Bounds3i b( b3Values, 6 );

    b.set( b3ValuesAlt, 6 );

    return std::equal( b.data, b.data + 6, b3ValuesAlt );
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( Bounds3_setMinMax )
    Bounds3i b( b3Values, 6 );
    Point3i mins( b3ValuesAlt,     3 );
    Point3i maxs( b3ValuesAlt + 3, 3 );

    b.set( mins, maxs );

    return std::equal( b.data, b.data + 6, b3ValuesAlt );
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( Bounds3_getValues )
    Bounds3i b( b3Values, 6 );

    int data[6];

    b.get(
        data[0], data[1], data[2],
        data[3], data[4], data[5]
    );

    return std::equal( data, data + 6, b3Values );
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( Bounds3_getArray )
    int data[6];
    std::copy( b3Values, b3Values + 6, data );

    Bounds3i b( b3ValuesAlt, 6 );
    b.get( data, 6 );

    return std::equal( data, data + 6, b3ValuesAlt );
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( Bounds3_getMinMax )
    Point3i mins( b3Values,     3 );
    Point3i maxs( b3Values + 3, 3 );

    Bounds3i b( b3ValuesAlt, 6 );
    b.get( mins, maxs );

    return std::equal( mins.data, mins.data + 3, b3ValuesAlt     ) &&
           std::equal( maxs.data, maxs.data + 3, b3ValuesAlt + 3 );
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( Bounds3_setDimensions )
    Bounds3i b( b3Values, 6 );

    b.setDimensions( 10, 11, 12 );

    return std::equal( b.data, b.data + 6, b3DimTest );
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( Bounds3_setWidth )
    Bounds3i b( b3Values, 6 );

    b.setWidth( 10 );

    return std::equal( b.data, b.data + 6, b3WidthTest );
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( Bounds3_setLength )
    Bounds3i b( b3Values, 6 );

    b.setLength( 11 );

    return std::equal( b.data, b.data + 6, b3LengthTest );
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( Bounds3_setHeight )
    Bounds3i b( b3Values, 6 );

    b.setHeight( 12 );

    return std::equal( b.data, b.data + 6, b3HeightTest );
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( Bounds3_setDimension )
    Bounds3i b( b3Values, 6 );

    for( int i = 0; i < 3; ++i )
        b.setDimension( i, 10 + i );

    return std::equal( b.data, b.data + 6, b3DimTest );
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( Bounds3_getDimensions )
    Bounds3i b( b3DimTest, 6 );
    int width, length, height;

    b.getDimensions( width, length, height );

    return width  == 10 &&
           length == 11 &&
           height == 12;
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( Bounds3_getWidth )
    Bounds3i b( b3WidthTest, 6 );

    int width = b.getWidth();

    return width == 10;
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( Bounds3_getLength )
    Bounds3i b( b3LengthTest, 6 );

    int length = b.getLength();

    return length == 11;
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( Bounds3_getHeight )
    Bounds3i b( b3HeightTest, 6 );

    int height = b.getHeight();

    return height == 12;
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( Bounds3_getDimension )
    Bounds3i b( b3DimTest, 6 );

    for( int i = 0; i < 3; ++i )
        if( b.getDimension( i ) != 10 + i )
            return false;

    return true;
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( Bounds3_getVolume )
    Bounds3i b( b3DimTest, 6 );

    return b.getVolume() == 10 * 11 * 12;
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( Bounds3_output )
    Bounds3i b( b3Values, 6 );

    std::stringstream sout;
    sout << b;

    return sout.str() == "[ ( 1, 2, 3 ), ( 4, 5, 6 ) ]";
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( Bounds3_zero )
    Bounds3i b( b3Values, 6 );

    b.zero();

    return std::equal( b.data, b.data + 6, b3Zero );
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( Bounds3_isZero )
    Bounds3i b1( b3Zero,   6 );
    Bounds3i b2( b3Values, 6 );

    return b1.isZero() == true &&
           b2.isZero() == false;
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( Bounds3_equals )
    Bounds3i b1( b3Values,    6 );
    Bounds3i b2( b3Values,    6 );
    Bounds3i b3( b3ValuesAlt, 6 );

    return ( b1 == b2 ) == true   &&
           ( b1 == b3 ) == false;
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( Bounds3_notEquals )
    Bounds3i b1( b3Values,    6 );
    Bounds3i b2( b3Values,    6 );
    Bounds3i b3( b3ValuesAlt, 6 );

    return ( b1 != b2 ) == false  &&
           ( b1 != b3 ) == true;
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( Bounds3_clamp )
    Bounds3i b( b3Values, 6 );
    Point3i p1( { 0, 1, 2 } );
    Point3i p2( { 5, 6, 7 } );

    clamp( p1, b );
    clamp( p2, b );

    return std::equal( p1.data, p1.data + 3, b3Values     ) &&
           std::equal( p2.data, p2.data + 3, b3Values + 3 );
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( Bounds3_clampedPoint )
    Bounds3i b( b3Values, 6 );
    Point3i p1( { 0, 1, 2 } );
    Point3i p2( { 5, 6, 7 } );

    p1 = clampedPoint( p1, b );
    p2 = clampedPoint( p2, b );

    return std::equal( p1.data, p1.data + 3, b3Values     ) &&
           std::equal( p2.data, p2.data + 3, b3Values + 3 );
BS_UT_TEST_END()




#ifdef BS_ZERO

BS_UT_TEST_BEGIN( Bounds3_constructorZero )
    Bounds3i b;
    return b.isZero();
BS_UT_TEST_END()

#endif //BS_ZERO




#ifdef BS_CHECK_NULLPTR

BS_UT_TEST_BEGIN( Bounds3_constructorArray_nullPointer )
    try {
        Bounds3i b( nullptr, 6 );
    } catch( const NullPointerException& ) {
        return true;
    }
    return false;
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( Bounds3_setArray_nullPointer )
    Bounds3i b( b3Values, 6 );
    try {
        b.set( nullptr, 6 );
    } catch( const NullPointerException& ) {
        return true;
    }
    return false;
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( Bounds3_getArray_nullPointer )
    Bounds3i pt( b3Values, 6 );
    try {
        pt.get( nullptr, 6 );
    } catch( const NullPointerException& ) {
        return true;
    }
    return false;
BS_UT_TEST_END()

#endif //BS_CHECK_NULLPTR




#ifdef BS_CHECK_SIZE

BS_UT_TEST_BEGIN( Bounds3_constructorArray_invalidSize )
    try {
        Bounds3i pt( b3Values, 0 );
    } catch( const SizeException& ) {
        return true;
    }
    return false;
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( Bounds3_setArray_invalidSize )
    Bounds3i b;
    try {
        b.set( b3ValuesAlt, 0 );
    } catch( const SizeException& ) {
        return true;
    }
    return false;
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( Bounds3_getArray_invalidSize )
    int data[6];
    Bounds3i b( b3Values, 6 );
    try {
        b.get( data, 0 );
    } catch( const SizeException& ) {
        return true;
    }
    return false;
BS_UT_TEST_END()

#endif //BS_CHECK_SIZE




#ifdef BS_CHECK_INDEX

BS_UT_TEST_BEGIN( Bounds3_setDimension_OOB )
    Bounds3i b;

    try {
        b.setDimension( -1, 10 );
        return false;
    } catch( const OutOfBoundsException& ) {}

    try {
        b.setDimension( 3, 10 );
        return false;
    } catch( const OutOfBoundsException& ) {}

    return true;
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( Bounds3_getDimension_OOB )
    Bounds3i b;
    int i;

    try {
        i = b.getDimension( -1 );
        return false;
    } catch( const OutOfBoundsException& ) {}

    try {
        i = b.getDimension( 3 );
        return false;
    } catch( const OutOfBoundsException& ) {}

    return true;
BS_UT_TEST_END()

#endif //BS_CHECK_INDEX

}
}