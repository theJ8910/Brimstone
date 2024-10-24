/*
test/Vector4.cpp
----------------
Copyright (c) 2024, theJ89

Description:
    Unit tests for Vector< T, 4 > specialization
*/




//Includes
#include "../Test.hpp"              //UT_TEST_BEGIN, UT_TEST_END
#include "../utils.hpp"             //UnitTest::allEqual, UnitTest::allEqualTo, UnitTest::copyAll, UnitTest::isWithin, UnitTest::allWithin, UnitTest::FAST_SQRT_ERR

#include <brimstone/Vector.hpp>     //Brimstone::Vector4i
#include <brimstone/Point.hpp>      //Brimstone::Point4i
#include <brimstone/Exception.hpp>  //Brimstone::BoundsException, Brimstone::DivideByZeroException

#include <cstddef>                  //std::size_t
#include <sstream>                  //std::ostringstream




namespace {




//Types
using ::Brimstone::Vector4i;
using ::Brimstone::Point4i;
using ::Brimstone::Vector4f;
using ::Brimstone::BoundsException;
using ::Brimstone::DivideByZeroException;




//Constants
const std::size_t cv_size               = 4;
const int         cv_zero[4]            {  0,  0,   0,  0 };
const int         cv_values[4]          {  1,  2,   3,  4 };
const int         cv_valuesAlt[4]       {  5,  6,   7,  8 };

const int         cv_unit[4]            {  0,  0,   0,  1 };
const int         cv_lengthSq           = 174;
const int         cv_length             = 13;  //Rounded down from 3.74165739f
const int         cv_dot                = 70;

const int         cv_arithmetic1[4]     {  6,  10,  24,  9 };
const int         cv_arithmetic2[4]     {  3,   5,   8,  9 };
const int         cv_addResult[4]       {  9,  15,  32, 18 };
const int         cv_subResult[4]       {  3,   5,  16,  0 };
const int         cv_mulResult[4]       { 18,  50, 192, 81 };
const int         cv_divResult[4]       {  2,   2,   3,  1 };
const int         cv_scalar             = 2;
const int         cv_addScalarResult[4] {  8,  12,  26, 11 };
const int         cv_subScalarResult[4] {  4,   8,  22,  7 };
const int         cv_mulScalarResult[4] { 12,  20,  48, 18 };
const int         cv_divScalarResult[4] {  3,   5,  12,  4 };   //Note: 9 / 2 = 4.5 -> 4
const int         cv_negateResult[4]    { -6, -10, -24, -9 };
const char*       cv_output             = "< 1, 2, 3, 4 >";

const float       cv_zeroF[4]           { 0.0f, 0.0f, 0.0f, 0.0f };
const float       cv_valuesF[4]         { 1.0f, 2.0f, 3.0f, 4.0f };
const float       cv_valuesAltF[4]      { 5.0f, 6.0f, 7.0f, 8.0f };
const float       cv_lengthF            = 13.190905958272919170936807732722f;
const float cv_unitF[4] {
     5.0f / cv_lengthF,
     6.0f / cv_lengthF,
     7.0f / cv_lengthF,
     8.0f / cv_lengthF
};
const float       cv_inverseF[4] {
     1.0f / 1.0f,
     1.0f / 2.0f,
     1.0f / 3.0f,
     1.0f / 4.0f
};
const char*       cv_outputF            = "< 1.00000, 2.00000, 3.00000, 4.00000 >";




} //namespace




namespace UnitTest {




UT_TEST_BEGIN( Vector4_constructorFill )
    Vector4i o( 1 );

    return allEqualTo( o.data, 1 );
UT_TEST_END()

UT_TEST_BEGIN( Vector4_constructorCppRange )
    Vector4i o( cv_values );

    return allEqual( o.data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( Vector4_constructorInitializerList )
    Vector4i o( { cv_values[0], cv_values[1], cv_values[2], cv_values[3] } );

    return allEqual( o.data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( Vector4_setCppRange )
    Vector4i o( cv_values );

    o.set( cv_valuesAlt );

    return allEqual( o.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( Vector4_setInitializerList )
    Vector4i o( cv_values );

    o.set( { cv_valuesAlt[0], cv_valuesAlt[1], cv_valuesAlt[2], cv_valuesAlt[3] } );

    return allEqual( o.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( Vector4_getCppRange )
    int values[ cv_size ];
    copyAll( cv_values, values );
    Vector4i o( cv_valuesAlt );

    o.get( values );

    return allEqual( o.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( Vector4_fill )
    Vector4i o( cv_values );

    o.fill( 1 );

    return allEqualTo( o.data, 1 );
UT_TEST_END()

UT_TEST_BEGIN( Vector4_begin )
    Vector4i o( cv_values );

    return o.begin() == std::begin( o.data );
UT_TEST_END()

UT_TEST_BEGIN( Vector4_beginConst )
    const Vector4i o( cv_values );

    return o.begin() == std::begin( o.data );
UT_TEST_END()

UT_TEST_BEGIN( Vector4_cbegin )
    Vector4i o( cv_values );

    return o.cbegin() == std::begin( o.data );
UT_TEST_END()

UT_TEST_BEGIN( Vector4_end )
    Vector4i o( cv_values );

    return o.end() == std::end( o.data );
UT_TEST_END()

UT_TEST_BEGIN( Vector4_endConst )
    const Vector4i o( cv_values );

    return o.end() == std::end( o.data );
UT_TEST_END()

UT_TEST_BEGIN( Vector4_cend )
    Vector4i o( cv_values );

    return o.cend() == std::end( o.data );
UT_TEST_END()

UT_TEST_BEGIN( Vector4_rangedFor )
    Vector4i o( cv_values );

    int idx = 0;
    for( int& i : o )
        i = cv_valuesAlt[idx++];

    return allEqual( o.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( Vector4_rangedForConst )
    int data[cv_size];
    copyAll( cv_values, data );
    const Vector4i o( cv_valuesAlt );

    int idx = 0;
    for( int i : o )
        data[idx++] = i;

    return allEqual( data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( Vector4_index )
    Vector4i o( cv_values );

    for( std::size_t i = 0; i < cv_size; ++i )
        o[i] = cv_valuesAlt[i];

    return allEqual( o.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( Vector4_indexConst )
    int data[cv_size];
    const Vector4i o( cv_values );

    for( std::size_t i = 0; i < cv_size; ++i )
        data[i] = o[i];

    return allEqual( data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( Vector4_constructorValues )
    Vector4i pt( cv_values[0], cv_values[1], cv_values[2], cv_values[3] );

    return allEqual( pt.data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( Vector4_setValues )
    Vector4i o( cv_values );

    o.set( cv_valuesAlt[0], cv_valuesAlt[1], cv_valuesAlt[2], cv_valuesAlt[3] );

    return allEqual( o.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( Vector4_getValues )
    Vector4i o( cv_values );

    int x, y, z, w;
    o.get( x, y, z, w );

    return x == cv_values[0] &&
           y == cv_values[1] &&
           z == cv_values[2] &&
           w == cv_values[3];
UT_TEST_END()

UT_TEST_BEGIN( Vector4_constructorCopy )
    Vector4f o1( cv_valuesAltF );

    Vector4i o2( o1 );

    return allEqual( o2.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( Vector4_zero )
    Vector4i o( cv_values );

    o.zero();

    return allEqual( o.data, cv_zero );
UT_TEST_END()

UT_TEST_BEGIN( Vector4_isZero )
    Vector4i o1( cv_zero );
    Vector4i o2( cv_values );

    return o1.isZero() == true &&
           o2.isZero() == false;
UT_TEST_END()

UT_TEST_BEGIN( Vector4_constructorTo )
    Point4i p( cv_values );
    Vector4i o( p );

    return allEqual( o.data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( Vector4_constructorFromTo )
    Point4i p1( cv_arithmetic2 );
    Point4i p2( cv_arithmetic1 );
    Vector4i o( p1, p2 );

    return allEqual( o.data, cv_subResult );
UT_TEST_END()

UT_TEST_BEGIN( Vector4_getLengthSq )
    Vector4i o( cv_valuesAlt );

    return o.getLengthSq() == cv_lengthSq;
UT_TEST_END()

UT_TEST_BEGIN( Vector4_getLength_int )
    Vector4i o( cv_valuesAlt );

    return o.getLength() == cv_length;
UT_TEST_END()

UT_TEST_BEGIN( Vector4_getLength_float )
    Vector4f o( cv_valuesAltF );

    return isWithin( o.getLength(), cv_lengthF, FAST_SQRT_ERROR );
UT_TEST_END()

UT_TEST_BEGIN( Vector4_normalize_int )
    Vector4i o( cv_values );

    o.normalize();

    return allEqual( o.data, cv_unit );
UT_TEST_END()

UT_TEST_BEGIN( Vector4_normalize_float )
    Vector4f o( cv_valuesAltF );

    o.normalize();

    return allWithin( o.data, cv_unitF, FAST_SQRT_ERROR, cv_size );
UT_TEST_END()

UT_TEST_BEGIN( Vector4_getNormal_int )
    Vector4i o1( cv_values );
    Vector4i o2 = o1.getNormal();

    return allEqual( o2.data, cv_unit );
UT_TEST_END()

UT_TEST_BEGIN( Vector4_getNormal_float )
    Vector4f o1( cv_valuesAltF );
    Vector4f o2 = o1.getNormal();

    return allWithin( o2.data, cv_unitF, FAST_SQRT_ERROR, cv_size );
UT_TEST_END()

UT_TEST_BEGIN( Vector4_isUnitVec_int )
    Vector4i o1( cv_unit );
    Vector4i o2( cv_valuesAlt );

    return o1.isUnitVec() == true &&
           o2.isUnitVec() == false;
UT_TEST_END()

UT_TEST_BEGIN( Vector4_isUnitVec_float )
    Vector4f v1( cv_unitF );
    Vector4f v2( cv_valuesAltF );

    return  v1.isUnitVec() &&
           !v2.isUnitVec();
UT_TEST_END()

UT_TEST_BEGIN( Vector4_invert )
    Vector4f o( cv_valuesF );

    o.invert();

    return allEqual( o.data, cv_inverseF );
UT_TEST_END()

UT_TEST_BEGIN( Vector4_negate )
    Vector4i o( cv_arithmetic1 );

    o.negate();

    return allEqual( o.data, cv_negateResult );
UT_TEST_END()

UT_TEST_BEGIN( Vector4_equals )
    Vector4i o1( cv_values );
    Vector4i o2( cv_values );
    Vector4i o3( cv_valuesAlt );

    return ( o1 == o2 ) == true   &&
           ( o1 == o3 ) == false;
UT_TEST_END()

UT_TEST_BEGIN( Vector4_notEquals )
    Vector4i o1( cv_values );
    Vector4i o2( cv_values );
    Vector4i o3( cv_valuesAlt );

    return ( o1 != o2 ) == false  &&
           ( o1 != o3 ) == true;
UT_TEST_END()

UT_TEST_BEGIN( Vector4_assignCopy )
    Vector4f o1( cv_valuesAltF );
    Vector4i o2( cv_values );

    o2 = o1;

    return allEqual( o2.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( Vector4_output_int )
    Vector4i o( cv_values );

    std::ostringstream sout;
    sout << o;

    return sout.str() == cv_output;
UT_TEST_END()

UT_TEST_BEGIN( Vector4_output_float )
    Vector4f o( cv_valuesF );

    std::ostringstream sout;
    sout << o;

    return sout.str() == cv_outputF;
UT_TEST_END()

UT_TEST_BEGIN( Vector4_addAssign_vector )
    Vector4i o1( cv_arithmetic1 );
    Vector4i o2( cv_arithmetic2 );

    o1 += o2;

    return allEqual( o1.data, cv_addResult );
UT_TEST_END()

UT_TEST_BEGIN( Vector4_subAssign_vector )
    Vector4i o1( cv_arithmetic1 );
    Vector4i o2( cv_arithmetic2 );

    o1 -= o2;

    return allEqual( o1.data, cv_subResult );
UT_TEST_END()

UT_TEST_BEGIN( Vector4_mulAssign_vector )
    Vector4i o1( cv_arithmetic1 );
    Vector4i o2( cv_arithmetic2 );

    o1 *= o2;

    return allEqual( o1.data, cv_mulResult );
UT_TEST_END()

UT_TEST_BEGIN( Vector4_divAssign_vector )
    Vector4i o1( cv_arithmetic1 );
    Vector4i o2( cv_arithmetic2 );

    o1 /= o2;

    return allEqual( o1.data, cv_divResult );
UT_TEST_END()

UT_TEST_BEGIN( Vector4_addAssign_scalar )
    Vector4i o( cv_arithmetic1 );

    o += cv_scalar;

    return allEqual( o.data, cv_addScalarResult );
UT_TEST_END()

UT_TEST_BEGIN( Vector4_subAssign_scalar )
    Vector4i o( cv_arithmetic1 );

    o -= cv_scalar;

    return allEqual( o.data, cv_subScalarResult );
UT_TEST_END()

UT_TEST_BEGIN( Vector4_mulAssign_scalar )
    Vector4i o( cv_arithmetic1 );

    o *= cv_scalar;

    return allEqual( o.data, cv_mulScalarResult );
UT_TEST_END()

UT_TEST_BEGIN( Vector4_divAssign_scalar )
    Vector4i o( cv_arithmetic1 );

    o /= cv_scalar;

    return allEqual( o.data, cv_divScalarResult );
UT_TEST_END()

UT_TEST_BEGIN( Vector4_typecastPoint )
    Vector4i o( cv_values );
    Point4i p( cv_valuesAlt );

    p = (Point4i&)o;

    return allEqual( o.data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( Vector4_typecastPoint_const )
    const Vector4i o( cv_values );
    Point4i p( cv_valuesAlt );

    p = (const Point4i&)o;

    return allEqual( o.data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( Vector4_unaryPlus )
    Vector4i o1( cv_arithmetic1 );
    Vector4i o2( cv_arithmetic2 );

    o2 = +o1;

    return allEqual( o2.data, cv_arithmetic1 );
UT_TEST_END()

UT_TEST_BEGIN( Vector4_unaryMinus )
    Vector4i o1( cv_arithmetic1 );
    Vector4i o2( cv_arithmetic2 );

    o2 = -o1;

    return allEqual( o2.data, cv_negateResult );
UT_TEST_END()

UT_TEST_BEGIN( Vector4_add_vector )
    Vector4i o1( cv_arithmetic1 );
    Vector4i o2( cv_arithmetic2 );
    Vector4i o3( cv_values );

    o3 = o1 + o2;

    return allEqual( o3.data, cv_addResult );
UT_TEST_END()

UT_TEST_BEGIN( Vector4_sub_vector )
    Vector4i o1( cv_arithmetic1 );
    Vector4i o2( cv_arithmetic2 );
    Vector4i o3( cv_values );

    o3 = o1 - o2;

    return allEqual( o3.data, cv_subResult );
UT_TEST_END()

UT_TEST_BEGIN( Vector4_mul_vector )
    Vector4i o1( cv_arithmetic1 );
    Vector4i o2( cv_arithmetic2 );
    Vector4i o3( cv_values     );

    o3 = o1 * o2;

    return allEqual( o3.data, cv_mulResult );
UT_TEST_END()

UT_TEST_BEGIN( Vector4_div_vector )
    Vector4i o1( cv_arithmetic1 );
    Vector4i o2( cv_arithmetic2 );
    Vector4i o3( cv_values     );

    o3 = o1 / o2;

    return allEqual( o3.data, cv_divResult );
UT_TEST_END()

UT_TEST_BEGIN( Vector4_add_scalar_left )
    Vector4i o1( cv_arithmetic1 );
    Vector4i o2( cv_values     );

    o2 = cv_scalar + o1;

    return allEqual( o2.data, cv_addScalarResult );
UT_TEST_END()

UT_TEST_BEGIN( Vector4_add_scalar_right )
    Vector4i o1( cv_arithmetic1 );
    Vector4i o2( cv_values     );

    o2 = o1 + cv_scalar;

    return allEqual( o2.data, cv_addScalarResult );
UT_TEST_END()

UT_TEST_BEGIN( Vector4_sub_scalar )
    Vector4i o1( cv_arithmetic1 );
    Vector4i o2( cv_values     );

    o2 = o1 - cv_scalar;

    return allEqual( o2.data, cv_subScalarResult );
UT_TEST_END()

UT_TEST_BEGIN( Vector4_mul_scalar_left )
    Vector4i o1( cv_arithmetic1 );
    Vector4i o2( cv_values      );

    o2 = cv_scalar * o1;

    return allEqual( o2.data, cv_mulScalarResult );
UT_TEST_END()

UT_TEST_BEGIN( Vector4_mul_scalar_right )
    Vector4i o1( cv_arithmetic1 );
    Vector4i o2( cv_values      );

    o2 = o1 * cv_scalar;

    return allEqual( o2.data, cv_mulScalarResult );
UT_TEST_END()

UT_TEST_BEGIN( Vector4_div_scalar )
    Vector4i o1( cv_arithmetic1 );
    Vector4i o2( cv_values      );

    o2 = o1 / cv_scalar;

    return allEqual( o2.data, cv_divScalarResult );
UT_TEST_END()

UT_TEST_BEGIN( Vector4_point_sub_point )
    Point4i  pt1( cv_arithmetic1 );
    Point4i  pt2( cv_arithmetic2 );
    Vector4i o(  cv_values      );

    o = pt1 - pt2;

    return allEqual( o.data, cv_subResult );
UT_TEST_END()

UT_TEST_BEGIN( Vector4_point_add_vec )
    Point4i  pt1( cv_arithmetic1 );
    Vector4i o(   cv_arithmetic2 );
    Point4i  pt2( cv_values );

    pt2 = pt1 + o;

    return allEqual( pt2.data, cv_addResult );
UT_TEST_END()

UT_TEST_BEGIN( Vector4_vec_add_point )
    Vector4i o(   cv_arithmetic1 );
    Point4i  pt1( cv_arithmetic2 );
    Point4i  pt2( cv_values );

    pt2 = o + pt1;

    return allEqual( pt2.data, cv_addResult );
UT_TEST_END()

UT_TEST_BEGIN( Vector4_point_sub_vec )
    Point4i  pt1( cv_arithmetic1 );
    Vector4i o(   cv_arithmetic2 );
    Point4i  pt2( cv_values );

    pt2 = pt1 - o;

    return allEqual( pt2.data, cv_subResult );
UT_TEST_END()

UT_TEST_BEGIN( Vector4_invert_free )
    Vector4f o( cv_valuesF );
    Vector4f o2( cv_valuesAltF );

    o2 = invert( o );

    return allEqual( o2.data, cv_inverseF );
UT_TEST_END()

UT_TEST_BEGIN( Vector4_dot )
    Vector4i o1( cv_values    );
    Vector4i o2( cv_valuesAlt );

    return dot( o1, o2 ) == cv_dot;
UT_TEST_END()




#ifdef BS_ZERO

UT_TEST_BEGIN( Vector4_constructorZero )
    Vector4i o;
    return allEqual( o.data, cv_zero );
UT_TEST_END()

#endif //BS_ZERO




#ifdef BS_CHECK_DIVBYZERO

UT_TEST_BEGIN( Vector4_normalize_divByZero )
    Vector4i o1( cv_zero );
    try {
        o1.normalize();
        return false;
    } catch( const DivideByZeroException& ) {}

    Vector4f o2( cv_zeroF );
    try {
        o2.normalize();
        return false;
    } catch( const DivideByZeroException& ) {}

    return true;
UT_TEST_END()

UT_TEST_BEGIN( Vector4_divAssign_vector_divByZero )
    Vector4i o1( cv_values );
    Vector4i o2( cv_zero );

    try {
        o1 /= o2;
        return false;
    } catch( const DivideByZeroException& ) {}

    return true;
UT_TEST_END()

UT_TEST_BEGIN( Vector4_divAssign_scalar_divByZero )
    Vector4i o( cv_values );

    try {
        o /= 0;
        return false;
    } catch( const DivideByZeroException& ) {}

    return true;
UT_TEST_END()

UT_TEST_BEGIN( Vector4_div_vector_divByZero )
    Vector4i o1( cv_values );
    Vector4i o2( cv_zero );
    Vector4i o3;

    try {
        o3 = o1 / o2;
        return false;
    } catch( const DivideByZeroException& ) {}

    return true;
UT_TEST_END()

UT_TEST_BEGIN( Vector4_div_scalar_divByZero )
    Vector4i o1( cv_values );
    Vector4i o2;

    try {
        o2 = o1 / 0;
        return false;
    } catch( const DivideByZeroException& ) {}

    return true;
UT_TEST_END()

UT_TEST_BEGIN( Vector4_invert_divByZero )
    Vector4f o( cv_zeroF );

    try {
        o.invert();
        return false;
    } catch( const DivideByZeroException& ) {}

    return true;
UT_TEST_END()

UT_TEST_BEGIN( Vector4_invert_free_divByZero )
    Vector4f o1( cv_zeroF );
    Vector4f o2;

    try {
        o2 = invert( o1 );
        return false;
    } catch( const DivideByZeroException& ) {}

    return true;
UT_TEST_END()

#endif //BS_CHECK_DIVBYZERO




#ifdef BS_CHECK_INDEX

UT_TEST_BEGIN( Vector4_index_OOB )
    Vector4i o;

    try {
        o[(std::size_t)-1];
        return false;
    } catch( const BoundsException& ) {}

    try {
        o[4];
        return false;
    } catch( const BoundsException& ) {}

    return true;
UT_TEST_END()

UT_TEST_BEGIN( Vector4_constIndex_OOB )
    const Vector4i o;

    try {
        o[(std::size_t)-1];
        return false;
    } catch( const BoundsException& ) {}

    try {
        o[4];
        return false;
    } catch( const BoundsException& ) {}

    return true;
UT_TEST_END()

#endif //BS_CHECK_INDEX




} //namespace UnitTest
