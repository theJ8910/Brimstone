/*
test/Point3.cpp
-----------------------
Copyright (c) 2014, theJ89

Description:
    Unit tests for PointN<T,3> specialization
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

#include <brimstone/Point.hpp>



namespace {
    const int pt3Zero[3]      {  0, 0, 0 };
    const int pt3Values[3]    {  1, 2, 3 };
    const int pt3ValuesAlt[3] {  4, 5, 6 };
    const int pt3Distant[3]   { -6, 5, 4 };
    const int pt3DistanceSq = 59;
    const int pt3Manhattan  = 11;

    const float pt3ValuesF[3] { 1.0f, 2.0f, 3.0f };
}

namespace Brimstone {
namespace UnitTest {

BS_UT_TEST_BEGIN( Point3_constructorValues )
    Point3i pt( pt3Values[0], pt3Values[1], pt3Values[2] );

    return std::equal( pt.data, pt.data + 3, pt3Values );
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( Point3_constructorInitializerList )
    Point3i pt( { 1, 2, 3 } );

    return std::equal( pt.data, pt.data + 3, pt3Values );
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( Point3_constructorArray )
    Point3i pt( pt3Values, 3 );

    return std::equal( pt.data, pt.data + 3, pt3Values );
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( Point3_constructorCopy )
    Point3f pt1( pt3ValuesF, 3 );
    Point3i pt2( pt1 );

    return std::equal( pt2.data, pt2.data + 3, pt3Values );
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( Point3_setValues )
    Point3i pt( pt3Values, 3 );

    pt.set( pt3ValuesAlt[0], pt3ValuesAlt[1], pt3ValuesAlt[2] );

    return std::equal( pt.data, pt.data + 3, pt3ValuesAlt );
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( Point3_setArray )
    Point3i pt( pt3Values, 3 );

    pt.set( pt3ValuesAlt, 3 );

    return std::equal( pt.data, pt.data + 3, pt3ValuesAlt );
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( Point3_getValues )
    Point3i pt( pt3Values, 3 );

    int x, y, z;
    pt.get( x, y, z );

    return x == pt3Values[0] &&
           y == pt3Values[1] &&
           z == pt3Values[2];
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( Point3_getArray )
    int data[3];
    std::copy( pt3Values, pt3Values + 3, data );

    Point3i pt( pt3ValuesAlt, 3 );
    pt.get( data, 3 );

    return std::equal( data, data + 3, pt3ValuesAlt );
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( Point3_zero )
    Point3i pt( pt3Values, 3 );

    pt.zero();

    return std::equal( pt.data, pt.data + 3, pt3Zero );
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( Point3_isZero )
    Point3i pt1( pt3Zero,   3 );
    Point3i pt2( pt3Values, 3 );

    return pt1.isZero() == true &&
           pt2.isZero() == false;
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( Point3_assignCopy )
    Point3f pt1( pt3ValuesF, 3 );
    Point3i pt2( pt3ValuesAlt, 3 );

    pt2 = pt1;

    return std::equal( pt2.data, pt2.data + 2, pt3Values );
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( Point3_equals )
    Point3i pt1( pt3Values,    3 );
    Point3i pt2( pt3Values,    3 );
    Point3i pt3( pt3ValuesAlt, 3 );

    return ( pt1 == pt2 ) == true   &&
           ( pt1 == pt3 ) == false;
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( Point3_notEquals )
    Point3i pt1( pt3Values,    3 );
    Point3i pt2( pt3Values,    3 );
    Point3i pt3( pt3ValuesAlt, 3 );

    return ( pt1 != pt2 ) == false  &&
           ( pt1 != pt3 ) == true;
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( Point3_pointer )
    Point3i pt( pt3Values, 3 );

    int* data = (int*)pt;
    std::copy( pt3ValuesAlt, pt3ValuesAlt + 3, data );

    return std::equal( pt.data, pt.data + 3, pt3ValuesAlt );
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( Point3_constPointer )
    int dataOut[3];
    std::copy( pt3Values, pt3Values + 3, dataOut );
    const Point3i pt( pt3ValuesAlt, 3 );

    const int* data = (const int*)pt;
    std::copy( data, data + 3, dataOut );

    return std::equal( dataOut, dataOut + 3, pt3ValuesAlt );
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( Point3_index )
    Point3i pt( pt3Values, 3 );

    for( int i = 0; i < 3; ++i )
        pt[i] = pt3ValuesAlt[i];

    return std::equal( pt.data, pt.data + 3, pt3ValuesAlt );
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( Point3_constIndex )
    int dataOut[3];
    const Point3i pt( pt3Values, 3 );

    for( int i = 0; i < 3; ++i )
        dataOut[i] = pt[i];

    return std::equal( dataOut, dataOut + 3, pt3Values );
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( Point3_output )
    Point3i pt( pt3Values, 3 );

    std::stringstream sout;
    sout << pt;

    return sout.str() == "( 1, 2, 3 )";
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( Point3_distanceSq )
    Point3i pt1( pt3Values,  3 );
    Point3i pt2( pt3Distant, 3 );

    return distanceSq( pt1, pt2 ) == pt3DistanceSq;
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( Point3_manhattan )
    Point3i pt1( pt3Values,  3 );
    Point3i pt2( pt3Distant, 3 );

    return manhattan( pt1, pt2 ) == pt3Manhattan;
BS_UT_TEST_END()




#ifdef BS_ZERO

BS_UT_TEST_BEGIN( Point3_constructorZero )
    Point3i pt;
    return pt.isZero();
BS_UT_TEST_END()

#endif //BS_ZERO




#ifdef BS_CHECK_NULLPTR

BS_UT_TEST_BEGIN( Point3_constructorArray_nullPointer )
    try {
        Point3i pt( nullptr, 3 );
    } catch( const NullPointerException& ) {
        return true;
    }
    return false;
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( Point3_setArray_nullPointer )
    Point3i pt( pt3Values, 3 );
    try {
        pt.set( nullptr, 3 );
    } catch( const NullPointerException& ) {
        return true;
    }
    return false;
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( Point3_getArray_nullPointer )
    Point3i pt( pt3Values, 3 );
    try {
        pt.get( nullptr, 3 );
    } catch( const NullPointerException& ) {
        return true;
    }
    return false;
BS_UT_TEST_END()

#endif //BS_CHECK_NULLPTR




#ifdef BS_CHECK_SIZE

BS_UT_TEST_BEGIN( Point3_constructorArray_invalidSize )
    try {
        Point3i pt( pt3Values, 0 );
    } catch( const SizeException& ) {
        return true;
    }
    return false;
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( Point3_setArray_invalidSize )
    Point3i pt;
    try {
        pt.set( pt3ValuesAlt, 0 );
    } catch( const SizeException& ) {
        return true;
    }
    return false;
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( Point3_getArray_invalidSize )
    int data[3];
    Point3i pt( pt3Values, 3 );
    try {
        pt.get( data, 0 );
    } catch( const SizeException& ) {
        return true;
    }
    return false;
BS_UT_TEST_END()




#ifdef BS_CHECK_INDEX

BS_UT_TEST_BEGIN( Point3_index_OOB )
    Point3i pt;
    int i;

    try {
        i = pt[-1];
        return false;
    } catch( const OutOfBoundsException& ) {}

    try {
        i = pt[3];
        return false;
    } catch( const OutOfBoundsException& ) {}

    return true;
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( Point3_constIndex_OOB )
    const Point3i pt;
    int i;

    try {
        i = pt[-1];
        return false;
    } catch( const OutOfBoundsException& ) {}

    try {
        i = pt[3];
        return false;
    } catch( const OutOfBoundsException& ) {}

    return true;
BS_UT_TEST_END()

#endif //BS_CHECK_INDEX




#endif //BS_CHECK_SIZE

}
}