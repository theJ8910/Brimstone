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

#include <brimstone/Point.hpp>



namespace {
    typedef ::Brimstone::PointN< int, 5 > Point5i;
    const int pt5Zero[5]      { 0, 0, 0, 0,  0 };
    const int pt5Values[5]    { 1, 2, 3, 4,  5 };
    const int pt5ValuesAlt[5] { 6, 7, 8, 9, 10 };
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
    Point5i pt1( pt5Zero, 5 );
    Point5i pt2( pt5Values, 5 );

    return pt1.isZero() == true &&
           pt2.isZero() == false;
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( PointN_equals )
    Point5i pt1( pt5Values, 5 );
    Point5i pt2( pt5Values, 5 );
    Point5i pt3( pt5ValuesAlt, 5 );

    return ( pt1 == pt2 ) == true   &&
           ( pt1 == pt3 ) == false;
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( PointN_notEquals )
    Point5i pt1( pt5Values, 5 );
    Point5i pt2( pt5Values, 5 );
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




#ifdef BS_ZERO

BS_UT_TEST_BEGIN( PointN_ZeroConstructor )
    Point5i pt;
    return pt.isZero();
BS_UT_TEST_END()

#endif //BS_ZERO




#ifdef BS_CHECK_NULLPTR

BS_UT_TEST_BEGIN( PointN_ArrayConstructor_NullPointer )
    try {
        Point5i pt( nullptr, 5 );
    } catch( const NullPointerException& ) {
        return true;
    }
    return false;
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( PointN_ArraySet_NullPointer )
    Point5i pt( pt5Values, 5 );
    try {
        pt.set( nullptr, 5 );
    } catch( const NullPointerException& ) {
        return true;
    }
    return false;
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( PointN_ArrayGet_NullPointer )
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

BS_UT_TEST_BEGIN( PointN_ArrayConstructor_InvalidSize )
    try {
        Point5i pt( pt5Values, 0 );
    } catch( const SizeException& ) {
        return true;
    }
    return false;
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( PointN_ArraySet_InvalidSize )
    Point5i pt( pt5Values, 5 );
    try {
        pt.set( pt5ValuesAlt, 0 );
    } catch( const SizeException& ) {
        return true;
    }
    return false;
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( PointN_ArrayGet_InvalidSize )
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

}
}