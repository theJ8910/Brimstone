/*
test/BoundsN.cpp
-----------------------
Copyright (c) 2014, theJ89

Description:
    Unit tests for BoundsN
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
    typedef ::Brimstone::BoundsN< int, 5 > Bounds5i;
    const int b5Zero[10]      {  0,  0,  0,  0,  0,    0,  0,  0,  0,  0 };
    const int b5Values[10]    {  1,  2,  3,  4,  5,    6,  7,  8,  9, 10 };
    const int b5ValuesAlt[10] { 11, 12, 13, 14, 15,   16, 17, 18, 19, 20 };
    const int b5DimTest[10]   {  1,  2,  3,  4,  5,   11, 12, 13, 14, 15 };
}

namespace Brimstone {
namespace UnitTest {

BS_UT_TEST_BEGIN( BoundsN_constructorMinMax )
    PointN< int, 5 > mins( b5Values,     5 );
    PointN< int, 5 > maxs( b5Values + 5, 5 );
    Bounds5i b( mins, maxs );

    return std::equal( b.data, b.data + 10, b5Values );
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( BoundsN_constructorInitializerList )
    Bounds5i b( { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 } );

    return std::equal( b.data, b.data + 10, b5Values );
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( BoundsN_constructorArray )
    Bounds5i b( b5Values, 10 );

    return std::equal( b.data, b.data + 10, b5Values );
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( BoundsN_setArray )
    Bounds5i b( b5Values, 10 );

    b.set( b5ValuesAlt, 10 );

    return std::equal( b.data, b.data + 10, b5ValuesAlt );
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( BoundsN_setMinMax )
    Bounds5i b( b5Values, 10 );
    PointN< int, 5 > mins( b5ValuesAlt,     5 );
    PointN< int, 5 > maxs( b5ValuesAlt + 5, 5 );

    b.set( mins, maxs );

    return std::equal( b.data, b.data + 10, b5ValuesAlt );
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( BoundsN_getArray )
    int data[10];
    std::copy( b5Values, b5Values + 10, data );

    Bounds5i b( b5ValuesAlt, 10 );
    b.get( data, 10 );

    return std::equal( data, data + 10, b5ValuesAlt );
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( BoundsN_getMinMax )
    PointN< int, 5 > mins( b5Values,     5 );
    PointN< int, 5 > maxs( b5Values + 5, 5 );

    Bounds5i b( b5ValuesAlt, 10 );
    b.get( mins, maxs );

    return std::equal( b.data, b.data + 10, b5ValuesAlt );
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( BoundsN_setDimension )
    Bounds5i b( b5Values, 10 );

    for( int i = 0; i < 5; ++i )
        b.setDimension( i, 10 );

    return std::equal( b.data, b.data + 10, b5DimTest );
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( BoundsN_getDimension )
    Bounds5i b( b5DimTest, 10 );

    for( int i = 0; i < 5; ++i )
        if( b.getDimension( i ) != 10 )
            return false;

    return true;
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( BoundsN_output )
    Bounds5i b( b5Values, 10 );

    std::stringstream sout;
    sout << b;

    return sout.str() == "[ ( 1, 2, 3, 4, 5 ), ( 6, 7, 8, 9, 10 ) ]";
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( BoundsN_zero )
    Bounds5i b( b5Values, 10 );

    b.zero();

    return std::equal( b.data, b.data + 10, b5Zero );
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( BoundsN_isZero )
    Bounds5i b1( b5Zero,   10 );
    Bounds5i b2( b5Values, 10 );

    return b1.isZero() == true &&
           b2.isZero() == false;
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( BoundsN_equals )
    Bounds5i b1( b5Values,    10 );
    Bounds5i b2( b5Values,    10 );
    Bounds5i b3( b5ValuesAlt, 10 );

    return ( b1 == b2 ) == true   &&
           ( b1 == b3 ) == false;
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( BoundsN_notEquals )
    Bounds5i b1( b5Values,    10 );
    Bounds5i b2( b5Values,    10 );
    Bounds5i b3( b5ValuesAlt, 10 );

    return ( b1 != b2 ) == false  &&
           ( b1 != b3 ) == true;
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( BoundsN_clamp )
    Bounds5i b( b5Values, 10 );
    PointN< int, 5 > p1( { 0, 1, 2,  3,  4 } );
    PointN< int, 5 > p2( { 7, 8, 9, 10, 11 } );

    clamp( p1, b );
    clamp( p2, b );

    return std::equal( p1.data, p1.data + 5, b5Values     ) &&
           std::equal( p2.data, p2.data + 5, b5Values + 5 );
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( BoundsN_clampedPoint )
    Bounds5i b( b5Values, 10 );
    PointN< int, 5 > p1( { 0, 1, 2,  3,  4 } );
    PointN< int, 5 > p2( { 7, 8, 9, 10, 11 } );

    p1 = clampedPoint( p1, b );
    p2 = clampedPoint( p2, b );

    return std::equal( p1.data, p1.data + 5, b5Values     ) &&
           std::equal( p2.data, p2.data + 5, b5Values + 5 );
BS_UT_TEST_END()




#ifdef BS_ZERO

BS_UT_TEST_BEGIN( BoundsN_constructorZero )
    Bounds5i b;
    return b.isZero();
BS_UT_TEST_END()

#endif //BS_ZERO




#ifdef BS_CHECK_NULLPTR

BS_UT_TEST_BEGIN( BoundsN_constructorArray_nullPointer )
    try {
        Bounds5i b( nullptr, 10 );
    } catch( const NullPointerException& ) {
        return true;
    }
    return false;
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( BoundsN_setArray_nullPointer )
    Bounds5i b( b5Values, 10 );
    try {
        b.set( nullptr, 10 );
    } catch( const NullPointerException& ) {
        return true;
    }
    return false;
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( BoundsN_getArray_nullPointer )
    Bounds5i pt( b5Values, 10 );
    try {
        pt.get( nullptr, 10 );
    } catch( const NullPointerException& ) {
        return true;
    }
    return false;
BS_UT_TEST_END()

#endif //BS_CHECK_NULLPTR




#ifdef BS_CHECK_SIZE

BS_UT_TEST_BEGIN( BoundsN_constructorArray_invalidSize )
    try {
        Bounds5i pt( b5Values, 0 );
    } catch( const SizeException& ) {
        return true;
    }
    return false;
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( BoundsN_setArray_invalidSize )
    Bounds5i b;
    try {
        b.set( b5ValuesAlt, 0 );
    } catch( const SizeException& ) {
        return true;
    }
    return false;
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( BoundsN_getArray_invalidSize )
    int data[10];
    Bounds5i b( b5Values, 10 );
    try {
        b.get( data, 0 );
    } catch( const SizeException& ) {
        return true;
    }
    return false;
BS_UT_TEST_END()

#endif //BS_CHECK_SIZE




#ifdef BS_CHECK_INDEX

BS_UT_TEST_BEGIN( BoundsN_setDimension_OOB )
    Bounds5i b;

    try {
        b.setDimension( -1, 10 );
        return false;
    } catch( const OutOfBoundsException& ) {}

    try {
        b.setDimension( 5, 10 );
        return false;
    } catch( const OutOfBoundsException& ) {}

    return true;
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( BoundsN_getDimension_OOB )
    Bounds5i b;
    int i;

    try {
        i = b.getDimension( -1 );
        return false;
    } catch( const OutOfBoundsException& ) {}

    try {
        i = b.getDimension( 5 );
        return false;
    } catch( const OutOfBoundsException& ) {}

    return true;
BS_UT_TEST_END()

#endif //BS_CHECK_INDEX

}
}