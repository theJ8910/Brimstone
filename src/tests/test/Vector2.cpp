/*
test/Vector2.cpp
----------------
Copyright (c) 2024, theJ89

Description:
    Unit tests for Vector<T,2> specialization
*/




//Includes
#include "../Test.hpp"          //UT_TEST_BEGIN, UT_TEST_END
#include "../utils.hpp"         //allEqual, allEqualTo, copyAll, isWithin, allWithin, FAST_SQRT_ERR

#include <cstddef>              //std::size_t
#include <sstream>              //std::ostringstream

#include <brimstone/Vector.hpp>




namespace {
    using ::Brimstone::Vector2i;
    using ::Brimstone::Point2i;
    using ::Brimstone::Vector2f;
    using ::Brimstone::BoundsException;
    using ::Brimstone::DivideByZeroException;

    const std::size_t cv_size               = 2;
    const int         cv_zero[2]            {  0,  0 };
    const int         cv_values[2]          {  1,  2 };
    const int         cv_valuesAlt[2]       {  5,  6 };

    const int         cv_unit[2]            {  0,  1 };
    const int         cv_left[2]            { -2,  1 };
    const int         cv_right[2]           {  2, -1 };
    const int         cv_lengthSq           = 61;
    const int         cv_length             = 7;  //Rounded down from 7.81024968f
    const int         cv_dot                = 17;

    const int         cv_arithmetic1[2]     {   6, 10 };
    const int         cv_arithmetic2[2]     {   3,  5 };
    const int         cv_addResult[2]       {   9, 15 };
    const int         cv_subResult[2]       {   3,  5 };
    const int         cv_mulResult[2]       {  18, 50 };
    const int         cv_divResult[2]       {   2,  2 };
    const int         cv_scalar             = 2;
    const int         cv_addScalarResult[2] {  8,  12 };
    const int         cv_subScalarResult[2] {  4,   8 };
    const int         cv_mulScalarResult[2] { 12,  20 };
    const int         cv_divScalarResult[2] {  3,   5 };
    const int         cv_negateResult[2]    { -6, -10 };
    const char*       cv_output             = "< 1, 2 >";

    const float       cv_zeroF[2]           { 0.0f, 0.0f };
    const float       cv_valuesF[2]         { 1.0f, 2.0f };
    const float       cv_valuesAltF[2]      { 5.0f, 6.0f };
    const float       cv_lengthF            = 7.81024968f;
    const float       cv_unitF[2] {
         5.0f / cv_lengthF,
         6.0f / cv_lengthF
    };
    const float       cv_inverseF[2] {
         1.0f / 1.0f,
         1.0f / 2.0f
    };
    const char*       cv_outputF = "< 1.00000, 2.00000 >";
}


namespace UnitTest {

UT_TEST_BEGIN( Vector2_constructorFill )
    Vector2i o( 1 );

    return allEqualTo( o.data, 1 );
UT_TEST_END()

UT_TEST_BEGIN( Vector2_constructorCppRange )
    Vector2i o( cv_values );

    return allEqual( o.data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( Vector2_constructorInitializerList )
    Vector2i o( { cv_values[0], cv_values[1] } );

    return allEqual( o.data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( Vector2_setCppRange )
    Vector2i o( cv_values );

    o.set( cv_valuesAlt );

    return allEqual( o.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( Vector2_setInitializerList )
    Vector2i o( cv_values );

    o.set( { cv_valuesAlt[0], cv_valuesAlt[1] } );

    return allEqual( o.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( Vector2_getCppRange )
    int values[ cv_size ];
    copyAll( cv_values, values );
    Vector2i o( cv_valuesAlt );

    o.get( values );

    return allEqual( o.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( Vector2_fill )
    Vector2i o( cv_values );

    o.fill( 1 );

    return allEqualTo( o.data, 1 );
UT_TEST_END()

UT_TEST_BEGIN( Vector2_begin )
    Vector2i o( cv_values );

    return o.begin() == std::begin( o.data );
UT_TEST_END()

UT_TEST_BEGIN( Vector2_beginConst )
    const Vector2i o( cv_values );

    return o.begin() == std::begin( o.data );
UT_TEST_END()

UT_TEST_BEGIN( Vector2_cbegin )
    Vector2i o( cv_values );

    return o.cbegin() == std::begin( o.data );
UT_TEST_END()

UT_TEST_BEGIN( Vector2_end )
    Vector2i o( cv_values );

    return o.end() == std::end( o.data );
UT_TEST_END()

UT_TEST_BEGIN( Vector2_endConst )
    const Vector2i o( cv_values );

    return o.end() == std::end( o.data );
UT_TEST_END()

UT_TEST_BEGIN( Vector2_cend )
    Vector2i o( cv_values );

    return o.cend() == std::end( o.data );
UT_TEST_END()

UT_TEST_BEGIN( Vector2_rangedFor )
    Vector2i o( cv_values );

    int idx = 0;
    for( int& i : o )
        i = cv_valuesAlt[idx++];

    return allEqual( o.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( Vector2_rangedForConst )
    int data[cv_size];
    copyAll( cv_values, data );
    const Vector2i o( cv_valuesAlt );

    int idx = 0;
    for( int i : o )
        data[idx++] = i;

    return allEqual( data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( Vector2_index )
    Vector2i o( cv_values );

    for( std::size_t i = 0; i < cv_size; ++i )
        o[i] = cv_valuesAlt[i];

    return allEqual( o.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( Vector2_indexConst )
    int data[cv_size];
    const Vector2i o( cv_values );

    for( std::size_t i = 0; i < cv_size; ++i )
        data[i] = o[i];

    return allEqual( data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( Vector2_constructorValues )
    Vector2i pt( cv_values[0], cv_values[1] );

    return allEqual( pt.data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( Vector2_setValues )
    Vector2i o( cv_values );

    o.set( cv_valuesAlt[0], cv_valuesAlt[1] );

    return allEqual( o.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( Vector2_getValues )
    Vector2i o( cv_values );

    int x, y;
    o.get( x, y );

    return x == cv_values[0] &&
           y == cv_values[1];
UT_TEST_END()

UT_TEST_BEGIN( Vector2_constructorCopy )
    Vector2f o1( cv_valuesF );

    Vector2i o2( o1 );

    return allEqual( o2.data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( Vector2_zero )
    Vector2i o( cv_values );

    o.zero();

    return allEqual( o.data, cv_zero );
UT_TEST_END()

UT_TEST_BEGIN( Vector2_isZero )
    Vector2i o1( cv_zero );
    Vector2i o2( cv_values );

    return o1.isZero() == true &&
           o2.isZero() == false;
UT_TEST_END()

UT_TEST_BEGIN( Vector2_constructorTo )
    Point2i p( cv_values );
    Vector2i o( p );

    return allEqual( o.data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( Vector2_constructorFromTo )
    Point2i p1( cv_arithmetic2 );
    Point2i p2( cv_arithmetic1 );
    Vector2i o( p1, p2 );

    return allEqual( o.data, cv_subResult );
UT_TEST_END()

UT_TEST_BEGIN( Vector2_getLengthSq )
    Vector2i o( cv_valuesAlt );

    return o.getLengthSq() == cv_lengthSq;
UT_TEST_END()

UT_TEST_BEGIN( Vector2_getLength_int )
    Vector2i o( cv_valuesAlt );

    return o.getLength() == cv_length;
UT_TEST_END()

UT_TEST_BEGIN( Vector2_getLength_float )
    Vector2f o( cv_valuesAltF );

    return isWithin( o.getLength(), cv_lengthF, FAST_SQRT_ERROR );
UT_TEST_END()

UT_TEST_BEGIN( Vector2_normalize_int )
    Vector2i o( cv_values );

    o.normalize();

    return allEqual( o.data, cv_unit );
UT_TEST_END()

UT_TEST_BEGIN( Vector2_normalize_float )
    Vector2f o( cv_valuesAltF );

    o.normalize();

    return allWithin( o.data, cv_unitF, FAST_SQRT_ERROR, cv_size );
UT_TEST_END()

UT_TEST_BEGIN( Vector2_getNormal_int )
    Vector2i o1( cv_values );
    Vector2i o2 = o1.getNormal();

    return allEqual( o2.data, cv_unit );
UT_TEST_END()

UT_TEST_BEGIN( Vector2_getNormal_float )
    Vector2f o1( cv_valuesAltF );
    Vector2f o2 = o1.getNormal();

    return allWithin( o2.data, cv_unitF, FAST_SQRT_ERROR, cv_size );
UT_TEST_END()

UT_TEST_BEGIN( Vector2_isUnitVec_int )
    Vector2i o1( cv_unit );
    Vector2i o2( cv_valuesAlt );

    return o1.isUnitVec() == true &&
           o2.isUnitVec() == false;
UT_TEST_END()

UT_TEST_BEGIN( Vector2_isUnitVec_float )
    Vector2f v1( cv_unitF );
    Vector2f v2( cv_valuesAltF );

    return  v1.isUnitVec() &&
           !v2.isUnitVec();
UT_TEST_END()

UT_TEST_BEGIN( Vector2_invert )
    Vector2f o( cv_valuesF );

    o.invert();

    return allEqual( o.data, cv_inverseF );
UT_TEST_END()

UT_TEST_BEGIN( Vector2_negate )
    Vector2i o( cv_arithmetic1 );

    o.negate();

    return allEqual( o.data, cv_negateResult );
UT_TEST_END()

UT_TEST_BEGIN( Vector2_left )
    Vector2i o( cv_values );

    o.left();

    return allEqual( o.data, cv_left );
UT_TEST_END()

UT_TEST_BEGIN( Vector2_right )
    Vector2i o( cv_values );

    o.right();

    return allEqual( o.data, cv_right );
UT_TEST_END()

UT_TEST_BEGIN( Vector2_equals )
    Vector2i o1( cv_values );
    Vector2i o2( cv_values );
    Vector2i o3( cv_valuesAlt );

    return ( o1 == o2 ) == true   &&
           ( o1 == o3 ) == false;
UT_TEST_END()

UT_TEST_BEGIN( Vector2_notEquals )
    Vector2i o1( cv_values );
    Vector2i o2( cv_values );
    Vector2i o3( cv_valuesAlt );

    return ( o1 != o2 ) == false  &&
           ( o1 != o3 ) == true;
UT_TEST_END()

UT_TEST_BEGIN( Vector2_assignCopy )
    Vector2f o1( cv_valuesF );
    Vector2i o2( cv_valuesAlt );

    o2 = o1;

    return allEqual( o2.data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( Vector2_output_int )
    Vector2i o( cv_values );

    std::stringstream sout;
    sout << o;

    return sout.str() == cv_output;
UT_TEST_END()

UT_TEST_BEGIN( Vector2_output_float )
    Vector2f o( cv_valuesF );

    std::stringstream sout;
    sout << o;

    return sout.str() == cv_outputF;
UT_TEST_END()

UT_TEST_BEGIN( Vector2_addAssign_vector )
    Vector2i o1( cv_arithmetic1 );
    Vector2i o2( cv_arithmetic2 );

    o1 += o2;

    return allEqual( o1.data, cv_addResult );
UT_TEST_END()

UT_TEST_BEGIN( Vector2_subAssign_vector )
    Vector2i o1( cv_arithmetic1 );
    Vector2i o2( cv_arithmetic2 );

    o1 -= o2;

    return allEqual( o1.data, cv_subResult );
UT_TEST_END()

UT_TEST_BEGIN( Vector2_mulAssign_vector )
    Vector2i o1( cv_arithmetic1 );
    Vector2i o2( cv_arithmetic2 );

    o1 *= o2;

    return allEqual( o1.data, cv_mulResult );
UT_TEST_END()

UT_TEST_BEGIN( Vector2_divAssign_vector )
    Vector2i o1( cv_arithmetic1 );
    Vector2i o2( cv_arithmetic2 );

    o1 /= o2;

    return allEqual( o1.data, cv_divResult );
UT_TEST_END()

UT_TEST_BEGIN( Vector2_addAssign_scalar )
    Vector2i o( cv_arithmetic1 );

    o += cv_scalar;

    return allEqual( o.data, cv_addScalarResult );
UT_TEST_END()

UT_TEST_BEGIN( Vector2_subAssign_scalar )
    Vector2i o( cv_arithmetic1 );

    o -= cv_scalar;

    return allEqual( o.data, cv_subScalarResult );
UT_TEST_END()

UT_TEST_BEGIN( Vector2_mulAssign_scalar )
    Vector2i o( cv_arithmetic1 );

    o *= cv_scalar;

    return allEqual( o.data, cv_mulScalarResult );
UT_TEST_END()

UT_TEST_BEGIN( Vector2_divAssign_scalar )
    Vector2i o( cv_arithmetic1 );

    o /= cv_scalar;

    return allEqual( o.data, cv_divScalarResult );
UT_TEST_END()

UT_TEST_BEGIN( Vector2_typecastPoint )
    Vector2i o( cv_values );
    Point2i p( cv_valuesAlt );

    p = (Point2i&)o;

    return allEqual( o.data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( Vector2_typecastPoint_const )
    const Vector2i o( cv_values );
    Point2i p( cv_valuesAlt );

    p = (const Point2i&)o;

    return allEqual( o.data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( Vector2_unaryPlus )
    Vector2i o1( cv_arithmetic1 );
    Vector2i o2( cv_arithmetic2 );

    o2 = +o1;

    return allEqual( o2.data, cv_arithmetic1 );
UT_TEST_END()

UT_TEST_BEGIN( Vector2_unaryMinus )
    Vector2i o1( cv_arithmetic1 );
    Vector2i o2( cv_arithmetic2 );

    o2 = -o1;

    return allEqual( o2.data, cv_negateResult );
UT_TEST_END()

UT_TEST_BEGIN( Vector2_add_vector )
    Vector2i o1( cv_arithmetic1 );
    Vector2i o2( cv_arithmetic2 );
    Vector2i o3( cv_values );

    o3 = o1 + o2;

    return allEqual( o3.data, cv_addResult );
UT_TEST_END()

UT_TEST_BEGIN( Vector2_sub_vector )
    Vector2i o1( cv_arithmetic1 );
    Vector2i o2( cv_arithmetic2 );
    Vector2i o3( cv_values );

    o3 = o1 - o2;

    return allEqual( o3.data, cv_subResult );
UT_TEST_END()

UT_TEST_BEGIN( Vector2_mul_vector )
    Vector2i o1( cv_arithmetic1 );
    Vector2i o2( cv_arithmetic2 );
    Vector2i o3( cv_values     );

    o3 = o1 * o2;

    return allEqual( o3.data, cv_mulResult );
UT_TEST_END()

UT_TEST_BEGIN( Vector2_div_vector )
    Vector2i o1( cv_arithmetic1 );
    Vector2i o2( cv_arithmetic2 );
    Vector2i o3( cv_values     );

    o3 = o1 / o2;

    return allEqual( o3.data, cv_divResult );
UT_TEST_END()

UT_TEST_BEGIN( Vector2_add_scalar_left )
    Vector2i o1( cv_arithmetic1 );
    Vector2i o2( cv_values     );

    o2 = cv_scalar + o1;

    return allEqual( o2.data, cv_addScalarResult );
UT_TEST_END()

UT_TEST_BEGIN( Vector2_add_scalar_right )
    Vector2i o1( cv_arithmetic1 );
    Vector2i o2( cv_values     );

    o2 = o1 + cv_scalar;

    return allEqual( o2.data, cv_addScalarResult );
UT_TEST_END()

UT_TEST_BEGIN( Vector2_sub_scalar )
    Vector2i o1( cv_arithmetic1 );
    Vector2i o2( cv_values     );

    o2 = o1 - cv_scalar;

    return allEqual( o2.data, cv_subScalarResult );
UT_TEST_END()

UT_TEST_BEGIN( Vector2_mul_scalar_left )
    Vector2i o1( cv_arithmetic1 );
    Vector2i o2( cv_values      );

    o2 = cv_scalar * o1;

    return allEqual( o2.data, cv_mulScalarResult );
UT_TEST_END()

UT_TEST_BEGIN( Vector2_mul_scalar_right )
    Vector2i o1( cv_arithmetic1 );
    Vector2i o2( cv_values      );

    o2 = o1 * cv_scalar;

    return allEqual( o2.data, cv_mulScalarResult );
UT_TEST_END()

UT_TEST_BEGIN( Vector2_div_scalar )
    Vector2i o1( cv_arithmetic1 );
    Vector2i o2( cv_values      );

    o2 = o1 / cv_scalar;

    return allEqual( o2.data, cv_divScalarResult );
UT_TEST_END()

UT_TEST_BEGIN( Vector2_point_sub_point )
    Point2i  pt1( cv_arithmetic1 );
    Point2i  pt2( cv_arithmetic2 );
    Vector2i o(  cv_values      );

    o = pt1 - pt2;

    return allEqual( o.data, cv_subResult );
UT_TEST_END()

UT_TEST_BEGIN( Vector2_point_add_vec )
    Point2i  pt1( cv_arithmetic1 );
    Vector2i o(   cv_arithmetic2 );
    Point2i  pt2( cv_values );

    pt2 = pt1 + o;

    return allEqual( pt2.data, cv_addResult );
UT_TEST_END()

UT_TEST_BEGIN( Vector2_vec_add_point )
    Vector2i o(   cv_arithmetic1 );
    Point2i  pt1( cv_arithmetic2 );
    Point2i  pt2( cv_values );

    pt2 = o + pt1;

    return allEqual( pt2.data, cv_addResult );
UT_TEST_END()

UT_TEST_BEGIN( Vector2_point_sub_vec )
    Point2i  pt1( cv_arithmetic1 );
    Vector2i o(   cv_arithmetic2 );
    Point2i  pt2( cv_values );

    pt2 = pt1 - o;

    return allEqual( pt2.data, cv_subResult );
UT_TEST_END()

UT_TEST_BEGIN( Vector2_invert_free )
    Vector2f o( cv_valuesF );
    Vector2f o2( cv_valuesAltF );

    o2 = invert( o );

    return allEqual( o2.data, cv_inverseF );
UT_TEST_END()

UT_TEST_BEGIN( Vector2_left_free )
    Vector2i o1( cv_values );

    Vector2i o2 = left( o1 );

    return allEqual( o2.data, cv_left );
UT_TEST_END()

UT_TEST_BEGIN( Vector2_right_free )
    Vector2i o1( cv_values );

    Vector2i o2 = right( o1 );

    return allEqual( o2.data, cv_right );
UT_TEST_END()

UT_TEST_BEGIN( Vector2_dot )
    Vector2i o1( cv_values    );
    Vector2i o2( cv_valuesAlt );

    return dot( o1, o2 ) == cv_dot;
UT_TEST_END()




#ifdef BS_ZERO

UT_TEST_BEGIN( Vector2_constructorZero )
    Vector2i o;
    return allEqual( o.data, cv_zero );
UT_TEST_END()

#endif //BS_ZERO




#ifdef BS_CHECK_DIVBYZERO

UT_TEST_BEGIN( Vector2_normalize_divByZero )
    Vector2i o1( cv_zero );
    try {
        o1.normalize();
        return false;
    } catch( const DivideByZeroException& ) {}

    Vector2f o2( cv_zeroF );
    try {
        o2.normalize();
        return false;
    } catch( const DivideByZeroException& ) {}

    return true;
UT_TEST_END()

UT_TEST_BEGIN( Vector2_divAssign_vector_divByZero )
    Vector2i o1( cv_values );
    Vector2i o2( cv_zero );

    try {
        o1 /= o2;
        return false;
    } catch( const DivideByZeroException& ) {}

    return true;
UT_TEST_END()

UT_TEST_BEGIN( Vector2_divAssign_scalar_divByZero )
    Vector2i o( cv_values );

    try {
        o /= 0;
        return false;
    } catch( const DivideByZeroException& ) {}

    return true;
UT_TEST_END()

UT_TEST_BEGIN( Vector2_div_vector_divByZero )
    Vector2i o1( cv_values );
    Vector2i o2( cv_zero );
    Vector2i o3;

    try {
        o3 = o1 / o2;
        return false;
    } catch( const DivideByZeroException& ) {}

    return true;
UT_TEST_END()

UT_TEST_BEGIN( Vector2_div_scalar_divByZero )
    Vector2i o1( cv_values );
    Vector2i o2;

    try {
        o2 = o1 / 0;
        return false;
    } catch( const DivideByZeroException& ) {}

    return true;
UT_TEST_END()

UT_TEST_BEGIN( Vector2_invert_divByZero )
    Vector2f o( cv_zeroF );

    try {
        o.invert();
        return false;
    } catch( const DivideByZeroException& ) {}

    return true;
UT_TEST_END()

UT_TEST_BEGIN( Vector2_invert_free_divByZero )
    Vector2f o1( cv_zeroF );
    Vector2f o2;

    try {
        o2 = invert( o1 );
        return false;
    } catch( const DivideByZeroException& ) {}

    return true;
UT_TEST_END()

#endif //BS_CHECK_DIVBYZERO




#ifdef BS_CHECK_INDEX

UT_TEST_BEGIN( Vector2_index_OOB )
    Vector2i o;

    try {
        o[(std::size_t)-1];
        return false;
    } catch( const BoundsException& ) {}

    try {
        o[3];
        return false;
    } catch( const BoundsException& ) {}

    return true;
UT_TEST_END()

UT_TEST_BEGIN( Vector2_constIndex_OOB )
    const Vector2i o;

    try {
        o[(std::size_t)-1];
        return false;
    } catch( const BoundsException& ) {}

    try {
        o[3];
        return false;
    } catch( const BoundsException& ) {}

    return true;
UT_TEST_END()

#endif //BS_CHECK_INDEX

}
