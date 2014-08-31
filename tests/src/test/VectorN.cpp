/*
test/VectorN.cpp
-----------------------
Copyright (c) 2014, theJ89

Description:
    Unit tests for Vector
*/




//Includes
#include <sstream>              //std::ostringstream

#include "../Test.hpp"          //UT_TEST_BEGIN, UT_TEST_END
#include "../utils.hpp"         //allEqual, allEqualTo, copyAll, isWithin, allWithin, FAST_SQRT_ERR

#include <brimstone/Vector.hpp>




namespace {
    typedef ::Brimstone::Point< int, 5 >    Point5i;
    typedef ::Brimstone::Vector< float, 5 > Vector5f;
    typedef ::Brimstone::Vector< int, 5 >   Vector5i;
    using   ::Brimstone::BoundsException;
    using   ::Brimstone::DivideByZeroException;

    const size_t cv_size               = 5;
    const int    cv_zero[5]            {  0,  0,   0,  0,   0 };
    const int    cv_values[5]          {  1,  2,   3,  4,   5 };
    const int    cv_valuesAlt[5]       {  6,  7,   8,  9,  10 };

    const int    cv_unit[5]            {  0,  0,   0,  0,   1 };
    const int    cv_lengthSq           = 330;
    const int    cv_length             = 18;  //Rounded down from 18.1659021f
    const int    cv_dot                = 130;

    const int    cv_arithmetic1[5]     {  6, 10,  24,  9,  30 };
    const int    cv_arithmetic2[5]     {  3,  5,   8,  9,   6 };
    const int    cv_addResult[5]       {  9, 15,  32, 18,  36 };
    const int    cv_subResult[5]       {  3,  5,  16,  0,  24 };
    const int    cv_mulResult[5]       { 18, 50, 192, 81, 180 };
    const int    cv_divResult[5]       {  2,  2,   3,  1,   5 };
    const int    cv_scalar             = 2;
    const int    cv_addScalarResult[5] {  8,  12,  26, 11,  32 };
    const int    cv_subScalarResult[5] {  4,   8,  22,  7,  28 };
    const int    cv_mulScalarResult[5] { 12,  20,  48, 18,  60 };
    const int    cv_divScalarResult[5] {  3,   5,  12,  4,  15 };   //Note: 9 / 2 = 4.5 -> 4
    const int    cv_negateResult[5]    { -6, -10, -24, -9, -30 };
    const char*  cv_output             = "< 1, 2, 3, 4, 5 >";

    const float cv_zeroF[5]            { 0.0f, 0.0f, 0.0f, 0.0f,  0.0f };
    const float cv_valuesF[5]          { 1.0f, 2.0f, 3.0f, 4.0f,  5.0f };
    const float cv_valuesAltF[5]       { 6.0f, 7.0f, 8.0f, 9.0f, 10.0f };
    const float cv_lengthF             = 18.1659021f;
    const float cv_unitF[5] {
         6.0f / cv_lengthF,
         7.0f / cv_lengthF,
         8.0f / cv_lengthF,
         9.0f / cv_lengthF,
        10.0f / cv_lengthF
    };
    const float cv_inverseF[5] {
         1.0f / 1.0f,
         1.0f / 2.0f,
         1.0f / 3.0f,
         1.0f / 4.0f,
         1.0f / 5.0f
    };
    const char* cv_outputF = "< 1.00000, 2.00000, 3.00000, 4.00000, 5.00000 >";
}

namespace UnitTest {

UT_TEST_BEGIN( VectorN_constructorFill )
    Vector5i o( 1 );

    return allEqualTo( o.data, 1 );
UT_TEST_END()

UT_TEST_BEGIN( VectorN_constructorCppRange )
    Vector5i o( cv_values );

    return allEqual( o.data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( VectorN_constructorInitializerList )
    Vector5i o( { cv_values[0], cv_values[1], cv_values[2], cv_values[3], cv_values[4] } );

    return allEqual( o.data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( VectorN_setCppRange )
    Vector5i o( cv_values );

    o.set( cv_valuesAlt );

    return allEqual( o.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( VectorN_setInitializerList )
    Vector5i o( cv_values );

    o.set( { cv_valuesAlt[0], cv_valuesAlt[1], cv_valuesAlt[2], cv_valuesAlt[3], cv_valuesAlt[4] } );

    return allEqual( o.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( VectorN_getCppRange )
    int values[ cv_size ];
    copyAll( cv_values, values );
    Vector5i o( cv_valuesAlt );

    o.get( values );

    return allEqual( o.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( VectorN_fill )
    Vector5i o( cv_values );

    o.fill( 1 );

    return allEqualTo( o.data, 1 );
UT_TEST_END()

UT_TEST_BEGIN( VectorN_begin )
    Vector5i o( cv_values );

    return o.begin() == std::begin( o.data );
UT_TEST_END()

UT_TEST_BEGIN( VectorN_beginConst )
    const Vector5i o( cv_values );

    return o.begin() == std::begin( o.data );
UT_TEST_END()

UT_TEST_BEGIN( VectorN_cbegin )
    Vector5i o( cv_values );

    return o.cbegin() == std::begin( o.data );
UT_TEST_END()

UT_TEST_BEGIN( VectorN_end )
    Vector5i o( cv_values );

    return o.end() == std::end( o.data );
UT_TEST_END()

UT_TEST_BEGIN( VectorN_endConst )
    const Vector5i o( cv_values );

    return o.end() == std::end( o.data );
UT_TEST_END()

UT_TEST_BEGIN( VectorN_cend )
    Vector5i o( cv_values );

    return o.cend() == std::end( o.data );
UT_TEST_END()

UT_TEST_BEGIN( VectorN_rangedFor )
    Vector5i o( cv_values );

    int idx = 0;
    for( int& i : o )
        i = cv_valuesAlt[idx++];

    return allEqual( o.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( VectorN_rangedForConst )
    int data[cv_size];
    copyAll( cv_values, data );
    const Vector5i o( cv_valuesAlt );

    int idx = 0;
    for( int i : o )
        data[idx++] = i;

    return allEqual( data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( VectorN_index )
    Vector5i o( cv_values );

    for( int i = 0; i < cv_size; ++i )
        o[i] = cv_valuesAlt[i];

    return allEqual( o.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( VectorN_indexConst )
    int data[cv_size];
    const Vector5i o( cv_values );

    for( int i = 0; i < cv_size; ++i )
        data[i] = o[i];

    return allEqual( data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( VectorN_constructorCopy )
    Vector5f o1( cv_valuesAltF );

    Vector5i o2( o1 );

    return allEqual( o2.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( VectorN_zero )
    Vector5i o( cv_values );

    o.zero();

    return allEqual( o.data, cv_zero );
UT_TEST_END()

UT_TEST_BEGIN( VectorN_isZero )
    Vector5i o1( cv_zero );
    Vector5i o2( cv_values );

    return o1.isZero() == true &&
           o2.isZero() == false;
UT_TEST_END()

UT_TEST_BEGIN( VectorN_constructorTo )
    Point5i p( cv_values );
    Vector5i o( p );

    return allEqual( o.data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( VectorN_constructorFromTo )
    Point5i p1( cv_arithmetic2 );
    Point5i p2( cv_arithmetic1 );
    Vector5i o( p1, p2 );

    return allEqual( o.data, cv_subResult );
UT_TEST_END()

UT_TEST_BEGIN( VectorN_getLengthSq )
    Vector5i o( cv_valuesAlt );

    return o.getLengthSq() == cv_lengthSq;
UT_TEST_END()

UT_TEST_BEGIN( VectorN_getLength_int )
    Vector5i o( cv_valuesAlt );

    return o.getLength() == cv_length;
UT_TEST_END()

UT_TEST_BEGIN( VectorN_getLength_float )
    Vector5f o( cv_valuesAltF );

    return isWithin( o.getLength(), cv_lengthF, FAST_SQRT_ERROR );
UT_TEST_END()

UT_TEST_BEGIN( VectorN_normalize_int )
    Vector5i o( cv_values );

    o.normalize();

    return allEqual( o.data, cv_unit );
UT_TEST_END()

UT_TEST_BEGIN( VectorN_normalize_float )
    Vector5f o( cv_valuesAltF );

    o.normalize();

    return allWithin( o.data, cv_unitF, FAST_SQRT_ERROR, cv_size );
UT_TEST_END()

UT_TEST_BEGIN( VectorN_isUnitVec_int )
    Vector5i o1( cv_unit );
    Vector5i o2( cv_valuesAlt );

    return o1.isUnitVec() == true &&
           o2.isUnitVec() == false;
UT_TEST_END()

UT_TEST_BEGIN( VectorN_isUnitVec_float )
    Vector5f o1( cv_unitF );
    Vector5f o2( cv_valuesAltF );

    return  o1.isUnitVec() &&
           !o2.isUnitVec();
UT_TEST_END()

UT_TEST_BEGIN( VectorN_invert )
    Vector5f o( cv_valuesF );

    o.invert();

    return allEqual( o.data, cv_inverseF );
UT_TEST_END()

UT_TEST_BEGIN( VectorN_negate )
    Vector5i o( cv_arithmetic1 );

    o.negate();

    return allEqual( o.data, cv_negateResult );
UT_TEST_END()

UT_TEST_BEGIN( VectorN_equals )
    Vector5i o1( cv_values );
    Vector5i o2( cv_values );
    Vector5i o3( cv_valuesAlt );

    return ( o1 == o2 ) == true   &&
           ( o1 == o3 ) == false;
UT_TEST_END()

UT_TEST_BEGIN( VectorN_notEquals )
    Vector5i o1( cv_values );
    Vector5i o2( cv_values );
    Vector5i o3( cv_valuesAlt );

    return ( o1 != o2 ) == false  &&
           ( o1 != o3 ) == true;
UT_TEST_END()

UT_TEST_BEGIN( VectorN_assignCopy )
    Vector5f o1( cv_valuesAltF );
    Vector5i o2( cv_values );

    o2 = o1;

    return allEqual( o2.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( VectorN_output_int )
    Vector5i o( cv_values );

    std::stringstream sout;
    sout << o;

    return sout.str() == cv_output;
UT_TEST_END()

UT_TEST_BEGIN( VectorN_output_float )
    Vector5f o( cv_valuesF );

    std::stringstream sout;
    sout << o;

    return sout.str() == cv_outputF;
UT_TEST_END()

UT_TEST_BEGIN( VectorN_addAssign_vector )
    Vector5i o1( cv_arithmetic1 );
    Vector5i o2( cv_arithmetic2 );

    o1 += o2;

    return allEqual( o1.data, cv_addResult );
UT_TEST_END()

UT_TEST_BEGIN( VectorN_subAssign_vector )
    Vector5i o1( cv_arithmetic1 );
    Vector5i o2( cv_arithmetic2 );

    o1 -= o2;

    return allEqual( o1.data, cv_subResult );
UT_TEST_END()

UT_TEST_BEGIN( VectorN_mulAssign_vector )
    Vector5i o1( cv_arithmetic1 );
    Vector5i o2( cv_arithmetic2 );

    o1 *= o2;

    return allEqual( o1.data, cv_mulResult );
UT_TEST_END()

UT_TEST_BEGIN( VectorN_divAssign_vector )
    Vector5i o1( cv_arithmetic1 );
    Vector5i o2( cv_arithmetic2 );

    o1 /= o2;

    return allEqual( o1.data, cv_divResult );
UT_TEST_END()

UT_TEST_BEGIN( VectorN_addAssign_scalar )
    Vector5i o( cv_arithmetic1 );

    o += cv_scalar;

    return allEqual( o.data, cv_addScalarResult );
UT_TEST_END()

UT_TEST_BEGIN( VectorN_subAssign_scalar )
    Vector5i o( cv_arithmetic1 );

    o -= cv_scalar;

    return allEqual( o.data, cv_subScalarResult );
UT_TEST_END()

UT_TEST_BEGIN( VectorN_mulAssign_scalar )
    Vector5i o( cv_arithmetic1 );

    o *= cv_scalar;

    return allEqual( o.data, cv_mulScalarResult );
UT_TEST_END()

UT_TEST_BEGIN( VectorN_divAssign_scalar )
    Vector5i o( cv_arithmetic1 );

    o /= cv_scalar;

    return allEqual( o.data, cv_divScalarResult );
UT_TEST_END()

UT_TEST_BEGIN( VectorN_typecastPoint )
    Vector5i o( cv_values );
    Point5i p( cv_valuesAlt );

    p = (Point5i&)o;

    return allEqual( o.data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( VectorN_typecastPoint_const )
    const Vector5i o( cv_values );
    Point5i p( cv_valuesAlt );

    p = (const Point5i&)o;

    return allEqual( o.data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( VectorN_unaryPlus )
    Vector5i o1( cv_arithmetic1 );
    Vector5i o2( cv_arithmetic2 );

    o2 = +o1;

    return allEqual( o2.data, cv_arithmetic1 );
UT_TEST_END()

UT_TEST_BEGIN( VectorN_unaryMinus )
    Vector5i o1( cv_arithmetic1 );
    Vector5i o2( cv_arithmetic2 );

    o2 = -o1;

    return allEqual( o2.data, cv_negateResult );
UT_TEST_END()

UT_TEST_BEGIN( VectorN_add_vector )
    Vector5i o1( cv_arithmetic1 );
    Vector5i o2( cv_arithmetic2 );
    Vector5i o3( cv_values );

    o3 = o1 + o2;

    return allEqual( o3.data, cv_addResult );
UT_TEST_END()

UT_TEST_BEGIN( VectorN_sub_vector )
    Vector5i o1( cv_arithmetic1 );
    Vector5i o2( cv_arithmetic2 );
    Vector5i o3( cv_values );

    o3 = o1 - o2;

    return allEqual( o3.data, cv_subResult );
UT_TEST_END()

UT_TEST_BEGIN( VectorN_mul_vector )
    Vector5i o1( cv_arithmetic1 );
    Vector5i o2( cv_arithmetic2 );
    Vector5i o3( cv_values     );

    o3 = o1 * o2;

    return allEqual( o3.data, cv_mulResult );
UT_TEST_END()

UT_TEST_BEGIN( VectorN_div_vector )
    Vector5i o1( cv_arithmetic1 );
    Vector5i o2( cv_arithmetic2 );
    Vector5i o3( cv_values     );

    o3 = o1 / o2;

    return allEqual( o3.data, cv_divResult );
UT_TEST_END()

UT_TEST_BEGIN( VectorN_add_scalar_left )
    Vector5i o1( cv_arithmetic1 );
    Vector5i o2( cv_values     );

    o2 = cv_scalar + o1;

    return allEqual( o2.data, cv_addScalarResult );
UT_TEST_END()

UT_TEST_BEGIN( VectorN_add_scalar_right )
    Vector5i o1( cv_arithmetic1 );
    Vector5i o2( cv_values     );

    o2 = o1 + cv_scalar;

    return allEqual( o2.data, cv_addScalarResult );
UT_TEST_END()

UT_TEST_BEGIN( VectorN_sub_scalar )
    Vector5i o1( cv_arithmetic1 );
    Vector5i o2( cv_values     );

    o2 = o1 - cv_scalar;

    return allEqual( o2.data, cv_subScalarResult );
UT_TEST_END()

UT_TEST_BEGIN( VectorN_mul_scalar_left )
    Vector5i o1( cv_arithmetic1 );
    Vector5i o2( cv_values     );

    o2 = cv_scalar * o1;

    return allEqual( o2.data, cv_mulScalarResult );
UT_TEST_END()

UT_TEST_BEGIN( VectorN_mul_scalar_right )
    Vector5i o1( cv_arithmetic1 );
    Vector5i o2( cv_values     );

    o2 = o1 * cv_scalar;

    return allEqual( o2.data, cv_mulScalarResult );
UT_TEST_END()

UT_TEST_BEGIN( VectorN_div_scalar )
    Vector5i o1( cv_arithmetic1 );
    Vector5i o2( cv_values     );

    o2 = o1 / cv_scalar;

    return allEqual( o2.data, cv_divScalarResult );
UT_TEST_END()

UT_TEST_BEGIN( VectorN_point_sub_point )
    Point5i  pt1( cv_arithmetic1 );
    Point5i  pt2( cv_arithmetic2 );
    Vector5i o(  cv_values      );

    o = pt1 - pt2;

    return allEqual( o.data, cv_subResult );
UT_TEST_END()

UT_TEST_BEGIN( VectorN_point_add_vec )
    Point5i  pt1( cv_arithmetic1 );
    Vector5i o(   cv_arithmetic2 );
    Point5i  pt2( cv_values );

    pt2 = pt1 + o;

    return allEqual( pt2.data, cv_addResult );
UT_TEST_END()

UT_TEST_BEGIN( VectorN_vec_add_point )
    Vector5i o(   cv_arithmetic1 );
    Point5i  pt1( cv_arithmetic2 );
    Point5i  pt2( cv_values );

    pt2 = o + pt1;

    return allEqual( pt2.data, cv_addResult );
UT_TEST_END()

UT_TEST_BEGIN( VectorN_point_sub_vec )
    Point5i  pt1( cv_arithmetic1 );
    Vector5i o(   cv_arithmetic2 );
    Point5i  pt2( cv_values );

    pt2 = pt1 - o;

    return allEqual( pt2.data, cv_subResult );
UT_TEST_END()

UT_TEST_BEGIN( VectorN_invert_free )
    Vector5f o( cv_valuesF );
    Vector5f o2( cv_valuesAltF );

    o2 = invert( o );

    return allEqual( o2.data, cv_inverseF );
UT_TEST_END()

UT_TEST_BEGIN( VectorN_dot )
    Vector5i o1( cv_values    );
    Vector5i o2( cv_valuesAlt );

    return dot( o1, o2 ) == cv_dot;
UT_TEST_END()




#ifdef BS_ZERO

UT_TEST_BEGIN( VectorN_constructorZero )
    Vector5i o;
    return allEqual( o.data, cv_zero );
UT_TEST_END()

#endif //BS_ZERO




#ifdef BS_CHECK_DIVBYZERO

UT_TEST_BEGIN( VectorN_normalize_divByZero )
    Vector5i o1( cv_zero );
    try {
        o1.normalize();
        return false;
    } catch( const DivideByZeroException& ) {}

    Vector5f o2( cv_zeroF );
    try {
        o2.normalize();
        return false;
    } catch( const DivideByZeroException& ) {}

    return true;
UT_TEST_END()

UT_TEST_BEGIN( VectorN_divAssign_vector_divByZero )
    Vector5i o1( cv_values );
    Vector5i o2( cv_zero );

    try {
        o1 /= o2;
        return false;
    } catch( const DivideByZeroException& ) {}

    return true;
UT_TEST_END()

UT_TEST_BEGIN( VectorN_divAssign_scalar_divByZero )
    Vector5i o( cv_values );

    try {
        o /= 0;
        return false;
    } catch( const DivideByZeroException& ) {}

    return true;
UT_TEST_END()

UT_TEST_BEGIN( VectorN_div_vector_divByZero )
    Vector5i o1( cv_values );
    Vector5i o2( cv_zero );
    Vector5i o3;

    try {
        o3 = o1 / o2;
        return false;
    } catch( const DivideByZeroException& ) {}

    return true;
UT_TEST_END()

UT_TEST_BEGIN( VectorN_div_scalar_divByZero )
    Vector5i o1( cv_values );
    Vector5i o2;

    try {
        o2 = o1 / 0;
        return false;
    } catch( const DivideByZeroException& ) {}

    return true;
UT_TEST_END()

UT_TEST_BEGIN( VectorN_invert_divByZero )
    Vector5f o( cv_zeroF );

    try {
        o.invert();
        return false;
    } catch( const DivideByZeroException& ) {}

    return true;
UT_TEST_END()

UT_TEST_BEGIN( VectorN_invert_free_divByZero )
    Vector5f o1( cv_zeroF );
    Vector5f o2;

    try {
        o2 = invert( o1 );
        return false;
    } catch( const DivideByZeroException& ) {}

    return true;
UT_TEST_END()

#endif //BS_CHECK_DIVBYZERO




#ifdef BS_CHECK_INDEX

UT_TEST_BEGIN( VectorN_index_OOB )
    Vector5i o;
    int i;

    try {
        i = o[(size_t)-1];
        return false;
    } catch( const BoundsException& ) {}

    try {
        i = o[5];
        return false;
    } catch( const BoundsException& ) {}

    return true;
UT_TEST_END()

UT_TEST_BEGIN( VectorN_constIndex_OOB )
    const Vector5i o;
    int i;

    try {
        i = o[(size_t)-1];
        return false;
    } catch( const BoundsException& ) {}

    try {
        i = o[5];
        return false;
    } catch( const BoundsException& ) {}

    return true;
UT_TEST_END()

#endif //BS_CHECK_INDEX

}