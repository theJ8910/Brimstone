/*
test/Point2.cpp
-----------------------
Copyright (c) 2014, theJ89

Description:
    Unit tests for PointN<T,2> specialization
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
    const int pt2Zero[2]      { 0, 0 };
    const int pt2Values[2]    { 1, 2 };
    const int pt2ValuesAlt[2] { 3, 4 };
}

namespace Brimstone {
namespace UnitTest {

BS_UT_TEST_BEGIN( Point2_constructorValues )
    Point2i pt( pt2Values[0], pt2Values[1] );

    return std::equal( pt.data, pt.data + 2, pt2Values );
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( Point2_constructorInitializerList )
    Point2i pt( { 1, 2 } );

    return std::equal( pt.data, pt.data + 2, pt2Values );
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( Point2_constructorArray )
    Point2i pt( pt2Values, 2 );

    return std::equal( pt.data, pt.data + 2, pt2Values );
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( Point2_setValues )
    Point2i pt( pt2Values, 2 );

    pt.set( pt2ValuesAlt[0], pt2ValuesAlt[1] );

    return std::equal( pt.data, pt.data + 2, pt2ValuesAlt );
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( Point2_setArray )
    Point2i pt( pt2Values, 2 );

    pt.set( pt2ValuesAlt, 2 );

    return std::equal( pt.data, pt.data + 2, pt2ValuesAlt );
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( Point2_getValues )
    Point2i pt( pt2Values, 2 );

    int x, y;
    pt.get( x, y );

    return x == pt2Values[0] &&
           y == pt2Values[1];
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( Point2_getArray )
    int data[2];
    std::copy( pt2Values, pt2Values + 2, data );

    Point2i pt( pt2ValuesAlt, 2 );
    pt.get( data, 2 );

    return std::equal( data, data + 2, pt2ValuesAlt );
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( Point2_zero )
    Point2i pt( pt2Values, 2 );

    pt.zero();

    return std::equal( pt.data, pt.data + 2, pt2Zero );
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( Point2_isZero )
    Point2i pt1( pt2Zero,   2 );
    Point2i pt2( pt2Values, 2 );

    return pt1.isZero() == true &&
           pt2.isZero() == false;
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( Point2_equals )
    Point2i pt1( pt2Values,    2 );
    Point2i pt2( pt2Values,    2 );
    Point2i pt3( pt2ValuesAlt, 2 );

    return ( pt1 == pt2 ) == true   &&
           ( pt1 == pt3 ) == false;
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( Point2_notEquals )
    Point2i pt1( pt2Values,    2 );
    Point2i pt2( pt2Values,    2 );
    Point2i pt3( pt2ValuesAlt, 2 );

    return ( pt1 != pt2 ) == false  &&
           ( pt1 != pt3 ) == true;
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( Point2_pointer )
    Point2i pt( pt2Values, 2 );

    int* data = (int*)pt;
    std::copy( pt2ValuesAlt, pt2ValuesAlt + 2, data );

    return std::equal( pt.data, pt.data + 2, pt2ValuesAlt );
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( Point2_constPointer )
    int dataOut[2];
    std::copy( pt2Values, pt2Values + 2, dataOut );
    const Point2i pt( pt2ValuesAlt, 2 );

    const int* data = (const int*)pt;
    std::copy( data, data + 2, dataOut );

    return std::equal( dataOut, dataOut + 2, pt2ValuesAlt );
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( Point2_index )
    Point2i pt( pt2Values, 2 );

    for( int i = 0; i < 2; ++i )
        pt[i] = pt2ValuesAlt[i];

    return std::equal( pt.data, pt.data + 2, pt2ValuesAlt );
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( Point2_constIndex )
    int dataOut[2];
    const Point2i pt( pt2Values, 2 );

    for( int i = 0; i < 2; ++i )
        dataOut[i] = pt[i];

    return std::equal( dataOut, dataOut + 2, pt2Values );
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( Point2_output )
    Point2i pt( pt2Values, 2 );

    std::stringstream sout;
    sout << pt;

    return sout.str() == "( 1, 2 )";
BS_UT_TEST_END()




#ifdef BS_ZERO

BS_UT_TEST_BEGIN( Point2_constructorZero )
    Point2i pt;
    return pt.isZero();
BS_UT_TEST_END()

#endif //BS_ZERO




#ifdef BS_CHECK_NULLPTR

BS_UT_TEST_BEGIN( Point2_constructorArray_nullPointer )
    try {
        Point2i pt( nullptr, 2 );
    } catch( const NullPointerException& ) {
        return true;
    }
    return false;
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( Point2_setArray_nullPointer )
    Point2i pt( pt2Values, 2 );
    try {
        pt.set( nullptr, 2 );
    } catch( const NullPointerException& ) {
        return true;
    }
    return false;
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( Point2_getArray_nullPointer )
    Point2i pt( pt2Values, 2 );
    try {
        pt.get( nullptr, 2 );
    } catch( const NullPointerException& ) {
        return true;
    }
    return false;
BS_UT_TEST_END()

#endif //BS_CHECK_NULLPTR




#ifdef BS_CHECK_SIZE

BS_UT_TEST_BEGIN( Point2_constructorArray_invalidSize )
    try {
        Point2i pt( pt2Values, 0 );
    } catch( const SizeException& ) {
        return true;
    }
    return false;
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( Point2_setArray_invalidSize )
    Point2i pt;
    try {
        pt.set( pt2ValuesAlt, 0 );
    } catch( const SizeException& ) {
        return true;
    }
    return false;
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( Point2_getArray_invalidSize )
    int data[2];
    Point2i pt( pt2Values, 2 );
    try {
        pt.get( data, 0 );
    } catch( const SizeException& ) {
        return true;
    }
    return false;
BS_UT_TEST_END()




#ifdef BS_CHECK_INDEX

BS_UT_TEST_BEGIN( Point2_index_OOB )
    Point2i pt;
    int i;

    bool negative = false;
    try {
        i = pt[-1];
    } catch( const OutOfBoundsException& ) {
        negative = true;
    }

    bool positive = false;
    try {
        i = pt[2];
    } catch( const OutOfBoundsException& ) {
        positive = true;
    }

    return negative && positive;
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( Point2_constIndex_OOB )
    const Point2i pt;

    int i;

    bool negative = false;
    try {
        i = pt[-1];
    } catch( const OutOfBoundsException& ) {
        negative = true;
    }

    bool positive = false;
    try {
        i = pt[2];
    } catch( const OutOfBoundsException& ) {
        positive = true;
    }

    return negative && positive;
BS_UT_TEST_END()

#endif //BS_CHECK_INDEX




#endif //BS_CHECK_SIZE

}
}