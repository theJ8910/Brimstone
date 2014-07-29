/*
test/PointN.cpp
-----------------------
Copyright (c) 2014, theJ89

Description:
    Unit tests for PointN
*/

//Includes

//C4996: 'std::_Copy_impl': Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct.
//       To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'
//It's complaining because I'm copying to a pointer instead of an array, and rightfully so because it's inherently unsafe.
//However, we can ignore this warning because the program logic ensures it will never copy values outside of the object.
#define _SCL_SECURE_NO_WARNINGS
#include <algorithm>            //std::copy, std::equal
#include <strstream>            //std::ostringstream

#include "../Test.hpp"
#include "../utils.hpp"         //isWithin, allWithin, FAST_SQRT_ERR

#include <brimstone/Point.hpp>



namespace {
    typedef ::Brimstone::PointN< int, 5 > Point5i;
    const int pt5Zero[5]            {   0, 0, 0,  0,  0 };
    const int pt5Values[5]          {   1, 2, 3,  4,  5 };
    const int pt5ValuesAlt[5]       {   6, 7, 8,  9, 10 };
    const int pt5Distant[5]         { -10, 9, 8,  7,  6 };
    const int pt5Distance   = 14;   //14.3178211 -> 14
    const int pt5DistanceSq = 205;
    const int pt5Manhattan  = 27;

    typedef ::Brimstone::PointN< float, 5 > Point5f;
    const float pt5ValuesF[5]       {   1.0f, 2.0f, 3.0f, 4.0f, 5.0f };
    const float pt5DistantF[5]      { -10.0f, 9.0f, 8.0f, 7.0f, 6.0f };
    const float pt5DistanceF = 14.3178211f;
}

namespace Brimstone {
namespace UnitTest {

BS_UT_TEST_BEGIN( PointN_constructorInitializerList )
    Point5i pt( { 1, 2, 3, 4, 5 } );

    return std::equal( pt.data, pt.data + 5, pt5Values );
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( PointN_constructorArray )
    Point5i pt( pt5Values, 5 );

    return std::equal( pt.data, pt.data + 5, pt5Values );
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( PointN_constructorCopy )
    Point5f pt1( pt5ValuesF, 5 );
    Point5i pt2( pt1 );

    return std::equal( pt2.data, pt2.data + 5, pt5Values );
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( PointN_setArray )
    Point5i pt( pt5Values, 5 );

    pt.set( pt5ValuesAlt, 5 );

    return std::equal( pt.data, pt.data + 5, pt5ValuesAlt );
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( PointN_getArray )
    int data[5];
    std::copy( pt5Values, pt5Values + 5, data );

    Point5i pt( pt5ValuesAlt, 5 );
    pt.get( data, 5 );

    return std::equal( data, data + 5, pt5ValuesAlt );
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( PointN_zero )
    Point5i pt( pt5Values, 5 );

    pt.zero();

    return std::equal( pt.data, pt.data + 5, pt5Zero );
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( PointN_isZero )
    Point5i pt1( pt5Zero,   5 );
    Point5i pt2( pt5Values, 5 );

    return pt1.isZero() == true &&
           pt2.isZero() == false;
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( PointN_assignCopy )
    Point5f pt1( pt5ValuesF, 5 );
    Point5i pt2( pt5ValuesAlt, 5 );

    pt2 = pt1;

    return std::equal( pt2.data, pt2.data + 5, pt5Values );
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( PointN_equals )
    Point5i pt1( pt5Values,    5 );
    Point5i pt2( pt5Values,    5 );
    Point5i pt3( pt5ValuesAlt, 5 );

    return ( pt1 == pt2 ) == true   &&
           ( pt1 == pt3 ) == false;
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( PointN_notEquals )
    Point5i pt1( pt5Values,    5 );
    Point5i pt2( pt5Values,    5 );
    Point5i pt3( pt5ValuesAlt, 5 );

    return ( pt1 != pt2 ) == false  &&
           ( pt1 != pt3 ) == true;
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( PointN_pointer )
    Point5i pt( pt5Values, 5 );

    int* data = (int*)pt;
    std::copy( pt5ValuesAlt, pt5ValuesAlt + 5, data );

    return std::equal( pt.data, pt.data + 5, pt5ValuesAlt );
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( PointN_constPointer )
    int dataOut[5];
    std::copy( pt5Values, pt5Values + 5, dataOut );
    const Point5i pt( pt5ValuesAlt, 5 );

    const int* data = (const int*)pt;
    std::copy( data, data + 5, dataOut );

    return std::equal( dataOut, dataOut + 5, pt5ValuesAlt );
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( PointN_index )
    Point5i pt( pt5Values, 5 );

    for( int i = 0; i < 5; ++i )
        pt[i] = pt5ValuesAlt[i];

    return std::equal( pt.data, pt.data + 5, pt5ValuesAlt );
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( PointN_constIndex )
    int dataOut[5];
    const Point5i pt( pt5Values, 5 );

    for( int i = 0; i < 5; ++i )
        dataOut[i] = pt[i];

    return std::equal( dataOut, dataOut + 5, pt5Values );
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( PointN_output )
    Point5i pt( pt5Values, 5 );

    std::stringstream sout;
    sout << pt;

    return sout.str() == "( 1, 2, 3, 4, 5 )";
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( PointN_distance_int )
    Point5i pt1( pt5Values,  5 );
    Point5i pt2( pt5Distant, 5 );

    return distance( pt1, pt2 ) == pt5Distance;
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( PointN_distance_float )
    Point5f pt1( pt5ValuesF,  5 );
    Point5f pt2( pt5DistantF, 5 );

    return isWithin( distance( pt1, pt2 ), pt5DistanceF, FAST_SQRT_ERROR );
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( PointN_distanceSq )
    Point5i pt1( pt5Values,  5 );
    Point5i pt2( pt5Distant, 5 );

    return distanceSq( pt1, pt2 ) == pt5DistanceSq;
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( PointN_manhattan )
    Point5i pt1( pt5Values,  5 );
    Point5i pt2( pt5Distant, 5 );

    return manhattan( pt1, pt2 ) == pt5Manhattan;
BS_UT_TEST_END()




#ifdef BS_ZERO

BS_UT_TEST_BEGIN( PointN_constructorZero )
    Point5i pt;
    return pt.isZero();
BS_UT_TEST_END()

#endif //BS_ZERO




#ifdef BS_CHECK_NULLPTR

BS_UT_TEST_BEGIN( PointN_constructorArray_nullPointer )
    try {
        Point5i pt( nullptr, 5 );
    } catch( const NullPointerException& ) {
        return true;
    }
    return false;
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( PointN_setArray_nullPointer )
    Point5i pt( pt5Values, 5 );
    try {
        pt.set( nullptr, 5 );
    } catch( const NullPointerException& ) {
        return true;
    }
    return false;
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( PointN_getArray_nullPointer )
    Point5i pt( pt5Values, 5 );
    try {
        pt.get( nullptr, 5 );
    } catch( const NullPointerException& ) {
        return true;
    }
    return false;
BS_UT_TEST_END()

#endif //BS_CHECK_NULLPTR




#ifdef BS_CHECK_SIZE

BS_UT_TEST_BEGIN( PointN_constructorArray_invalidSize )
    try {
        Point5i pt( pt5Values, 0 );
    } catch( const SizeException& ) {
        return true;
    }
    return false;
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( PointN_setArray_invalidSize )
    Point5i pt;
    try {
        pt.set( pt5ValuesAlt, 0 );
    } catch( const SizeException& ) {
        return true;
    }
    return false;
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( PointN_getArray_invalidSize )
    int data[5];
    Point5i pt( pt5Values, 5 );
    try {
        pt.get( data, 0 );
    } catch( const SizeException& ) {
        return true;
    }
    return false;
BS_UT_TEST_END()

#endif //BS_CHECK_SIZE




#ifdef BS_CHECK_INDEX

BS_UT_TEST_BEGIN( PointN_index_OOB )
    Point5i pt;
    int i;

    try {
        i = pt[-1];
        return false;
    } catch( const OutOfBoundsException& ) {}

    try {
        i = pt[5];
        return false;
    } catch( const OutOfBoundsException& ) {}

    return true;
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( PointN_constIndex_OOB )
    const Point5i pt;
    int i;

    try {
        i = pt[-1];
        return false;
    } catch( const OutOfBoundsException& ) {}

    try {
        i = pt[5];
        return false;
    } catch( const OutOfBoundsException& ) {}

    return true;
BS_UT_TEST_END()

#endif //BS_CHECK_INDEX

}
}