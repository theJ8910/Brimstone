/*
test/Vector3.cpp
-----------------------
Copyright (c) 2014, theJ89

Description:
    Unit tests for VectorN<T,3> specialization
*/




//Includes
#include <strstream>            //std::ostringstream

#include "../Test.hpp"          //UT_TEST_BEGIN, UT_TEST_END
#include "../utils.hpp"         //allEqual, allEqualTo, copyAll, isWithin, allWithin, FAST_SQRT_ERR

#include <brimstone/Vector.hpp>




namespace {
    using ::Brimstone::Vector3i;
    using ::Brimstone::Point3i;
    using ::Brimstone::Vector3f;
    using ::Brimstone::BoundsException;

    const size_t cv_size               = 3;
    const int    cv_zero[3]            {  0,  0,   0 };
    const int    cv_values[3]          {  1,  2,   3 };
    const int    cv_valuesAlt[3]       {  4,  5,   6 };

    const int    cv_unit[3]            {  0,  0,   1 };
    const int    cv_lengthSq           = 77;
    const int    cv_length             = 8;  //Rounded down from 8.77496439f
    const int    cv_dot                = 32;

    const int    cv_arithmetic1[3]     {   6, 10,  24 };
    const int    cv_arithmetic2[3]     {   3,  5,   8 };
    const int    cv_addResult[3]       {   9, 15,  32 };
    const int    cv_subResult[3]       {   3,  5,  16 };
    const int    cv_mulResult[3]       {  18, 50, 192 };
    const int    cv_divResult[3]       {   2,  2,   3 };
    const int    cv_scalar             = 2;
    const int    cv_addScalarResult[3] {  8,  12,  26 };
    const int    cv_subScalarResult[3] {  4,   8,  22 };
    const int    cv_mulScalarResult[3] { 12,  20,  48 };
    const int    cv_divScalarResult[3] {  3,   5,  12 };
    const int    cv_negateResult[3]    { -6, -10, -24 };
    const int    cv_crossResult[3]     { -40, 24,   0 };
    const char*  cv_output             = "< 1, 2, 3 >";

    const float cv_valuesF[3]          { 1.0f, 2.0f, 3.0f };
    const float cv_valuesAltF[3]       { 4.0f, 5.0f, 6.0f };
    const float cv_lengthF             = 8.77496439f;
    const float cv_unitF[3] {
         4.0f / cv_lengthF,
         5.0f / cv_lengthF,
         6.0f / cv_lengthF
    };
    const char* cv_outputF = "< 1.00000, 2.00000, 3.00000 >";
}


namespace UnitTest {

UT_TEST_BEGIN( Vector3_constructorFill )
    Vector3i o( 1 );

    return allEqualTo( o.data, 1 );
UT_TEST_END()

UT_TEST_BEGIN( Vector3_constructorCppRange )
    Vector3i o( cv_values );

    return allEqual( o.data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( Vector3_constructorInitializerList )
    Vector3i o( { cv_values[0], cv_values[1], cv_values[2] } );

    return allEqual( o.data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( Vector3_setCppRange )
    Vector3i o( cv_values );

    o.set( cv_valuesAlt );

    return allEqual( o.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( Vector3_setInitializerList )
    Vector3i o( cv_values );

    o.set( { cv_valuesAlt[0], cv_valuesAlt[1], cv_valuesAlt[2] } );

    return allEqual( o.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( Vector3_getCppRange )
    int values[ cv_size ];
    copyAll( cv_values, values );
    Vector3i o( cv_valuesAlt );

    o.get( values );

    return allEqual( o.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( Vector3_fill )
    Vector3i o( cv_values );

    o.fill( 1 );

    return allEqualTo( o.data, 1 );
UT_TEST_END()

UT_TEST_BEGIN( Vector3_begin )
    Vector3i o( cv_values );

    return o.begin() == std::begin( o.data );
UT_TEST_END()

UT_TEST_BEGIN( Vector3_beginConst )
    const Vector3i o( cv_values );

    return o.begin() == std::begin( o.data );
UT_TEST_END()

UT_TEST_BEGIN( Vector3_cbegin )
    Vector3i o( cv_values );

    return o.cbegin() == std::cbegin( o.data );
UT_TEST_END()

UT_TEST_BEGIN( Vector3_end )
    Vector3i o( cv_values );

    return o.end() == std::end( o.data );
UT_TEST_END()

UT_TEST_BEGIN( Vector3_endConst )
    const Vector3i o( cv_values );

    return o.end() == std::end( o.data );
UT_TEST_END()

UT_TEST_BEGIN( Vector3_cend )
    Vector3i o( cv_values );

    return o.cend() == std::cend( o.data );
UT_TEST_END()

UT_TEST_BEGIN( Vector3_rangedFor )
    Vector3i o( cv_values );

    int idx = 0;
    for( int& i : o )
        i = cv_valuesAlt[idx++];

    return allEqual( o.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( Vector3_rangedForConst )
    int data[cv_size];
    copyAll( cv_values, data );
    const Vector3i o( cv_valuesAlt );

    int idx = 0;
    for( int i : o )
        data[idx++] = i;

    return allEqual( data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( Vector3_index )
    Vector3i o( cv_values );

    for( int i = 0; i < cv_size; ++i )
        o[i] = cv_valuesAlt[i];

    return allEqual( o.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( Vector3_indexConst )
    int data[cv_size];
    const Vector3i o( cv_values );

    for( int i = 0; i < cv_size; ++i )
        data[i] = o[i];

    return allEqual( data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( Vector3_constructorValues )
    Vector3i pt( cv_values[0], cv_values[1], cv_values[2] );

    return allEqual( pt.data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( Vector3_setValues )
    Vector3i o( cv_values );

    o.set( cv_valuesAlt[0], cv_valuesAlt[1], cv_valuesAlt[2] );

    return allEqual( o.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( Vector3_getValues )
    Vector3i o( cv_values );

    int x, y, z;
    o.get( x, y, z );

    return x == cv_values[0] &&
           y == cv_values[1] &&
           z == cv_values[2];
UT_TEST_END()

UT_TEST_BEGIN( Vector3_constructorCopy )
    Vector3f o1( cv_valuesAltF );

    Vector3i o2( o1 );

    return allEqual( o2.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( Vector3_zero )
    Vector3i o( cv_values );

    o.zero();

    return allEqual( o.data, cv_zero );
UT_TEST_END()

UT_TEST_BEGIN( Vector3_isZero )
    Vector3i o1( cv_zero );
    Vector3i o2( cv_values );

    return o1.isZero() == true &&
           o2.isZero() == false;
UT_TEST_END()

UT_TEST_BEGIN( Vector3_constructorTo )
    Point3i p( cv_values );
    Vector3i o( p );

    return allEqual( o.data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( Vector3_constructorFromTo )
    Point3i p1( cv_arithmetic2 );
    Point3i p2( cv_arithmetic1 );
    Vector3i o( p1, p2 );

    return allEqual( o.data, cv_subResult );
UT_TEST_END()

UT_TEST_BEGIN( Vector3_getLengthSq )
    Vector3i o( cv_valuesAlt );

    return o.getLengthSq() == cv_lengthSq;
UT_TEST_END()

UT_TEST_BEGIN( Vector3_getLength_int )
    Vector3i o( cv_valuesAlt );

    return o.getLength() == cv_length;
UT_TEST_END()

UT_TEST_BEGIN( Vector3_getLength_float )
    Vector3f o( cv_valuesAltF );

    return isWithin( o.getLength(), cv_lengthF, FAST_SQRT_ERROR );
UT_TEST_END()

UT_TEST_BEGIN( Vector3_normalize_int )
    Vector3i o( cv_values );

    o.normalize();

    return allEqual( o.data, cv_unit );
UT_TEST_END()

UT_TEST_BEGIN( Vector3_normalize_float )
    Vector3f o( cv_valuesAltF );

    o.normalize();

    return allWithin( o.data, cv_unitF, FAST_SQRT_ERROR, cv_size );
UT_TEST_END()

UT_TEST_BEGIN( Vector3_isUnitVec_int )
    Vector3i o1( cv_unit );
    Vector3i o2( cv_valuesAlt );

    return o1.isUnitVec() == true &&
           o2.isUnitVec() == false;
UT_TEST_END()

UT_TEST_BEGIN( Vector3_isUnitVec_float )
    Vector3f v1( cv_unitF );
    Vector3f v2( cv_valuesAltF );

    return  v1.isUnitVec() &&
           !v2.isUnitVec();
UT_TEST_END()

UT_TEST_BEGIN( Vector3_equals )
    Vector3i o1( cv_values );
    Vector3i o2( cv_values );
    Vector3i o3( cv_valuesAlt );

    return ( o1 == o2 ) == true   &&
           ( o1 == o3 ) == false;
UT_TEST_END()

UT_TEST_BEGIN( Vector3_notEquals )
    Vector3i o1( cv_values );
    Vector3i o2( cv_values );
    Vector3i o3( cv_valuesAlt );

    return ( o1 != o2 ) == false  &&
           ( o1 != o3 ) == true;
UT_TEST_END()

UT_TEST_BEGIN( Vector3_assignCopy )
    Vector3f o1( cv_valuesAltF );
    Vector3i o2( cv_values );

    o2 = o1;

    return allEqual( o2.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( Vector3_output_int )
    Vector3i o( cv_values );

    std::stringstream sout;
    sout << o;

    return sout.str() == cv_output;
UT_TEST_END()

UT_TEST_BEGIN( Vector3_output_float )
    Vector3f o( cv_valuesF );

    std::stringstream sout;
    sout << o;

    return sout.str() == cv_outputF;
UT_TEST_END()

UT_TEST_BEGIN( Vector3_addAssign_vector )
    Vector3i o1( cv_arithmetic1 );
    Vector3i o2( cv_arithmetic2 );

    o1 += o2;

    return allEqual( o1.data, cv_addResult );
UT_TEST_END()

UT_TEST_BEGIN( Vector3_subAssign_vector )
    Vector3i o1( cv_arithmetic1 );
    Vector3i o2( cv_arithmetic2 );

    o1 -= o2;

    return allEqual( o1.data, cv_subResult );
UT_TEST_END()

UT_TEST_BEGIN( Vector3_mulAssign_vector )
    Vector3i o1( cv_arithmetic1 );
    Vector3i o2( cv_arithmetic2 );

    o1 *= o2;

    return allEqual( o1.data, cv_mulResult );
UT_TEST_END()

UT_TEST_BEGIN( Vector3_divAssign_vector )
    Vector3i o1( cv_arithmetic1 );
    Vector3i o2( cv_arithmetic2 );

    o1 /= o2;

    return allEqual( o1.data, cv_divResult );
UT_TEST_END()

UT_TEST_BEGIN( Vector3_addAssign_scalar )
    Vector3i o( cv_arithmetic1 );

    o += cv_scalar;

    return allEqual( o.data, cv_addScalarResult );
UT_TEST_END()

UT_TEST_BEGIN( Vector3_subAssign_scalar )
    Vector3i o( cv_arithmetic1 );

    o -= cv_scalar;

    return allEqual( o.data, cv_subScalarResult );
UT_TEST_END()

UT_TEST_BEGIN( Vector3_mulAssign_scalar )
    Vector3i o( cv_arithmetic1 );

    o *= cv_scalar;

    return allEqual( o.data, cv_mulScalarResult );
UT_TEST_END()

UT_TEST_BEGIN( Vector3_divAssign_scalar )
    Vector3i o( cv_arithmetic1 );

    o /= cv_scalar;

    return allEqual( o.data, cv_divScalarResult );
UT_TEST_END()

UT_TEST_BEGIN( Vector3_unaryPlus )
    Vector3i o1( cv_arithmetic1 );
    Vector3i o2( cv_arithmetic2 );

    o2 = +o1;

    return allEqual( o2.data, cv_arithmetic1 );
UT_TEST_END()

UT_TEST_BEGIN( Vector3_unaryMinus )
    Vector3i o1( cv_arithmetic1 );
    Vector3i o2( cv_arithmetic2 );

    o2 = -o1;

    return allEqual( o2.data, cv_negateResult );
UT_TEST_END()

UT_TEST_BEGIN( Vector3_add_vector )
    Vector3i o1( cv_arithmetic1 );
    Vector3i o2( cv_arithmetic2 );
    Vector3i o3( cv_values );

    o3 = o1 + o2;

    return allEqual( o3.data, cv_addResult );
UT_TEST_END()

UT_TEST_BEGIN( Vector3_sub_vector )
    Vector3i o1( cv_arithmetic1 );
    Vector3i o2( cv_arithmetic2 );
    Vector3i o3( cv_values );

    o3 = o1 - o2;

    return allEqual( o3.data, cv_subResult );
UT_TEST_END()

UT_TEST_BEGIN( Vector3_mul_vector )
    Vector3i o1( cv_arithmetic1 );
    Vector3i o2( cv_arithmetic2 );
    Vector3i o3( cv_values     );

    o3 = o1 * o2;

    return allEqual( o3.data, cv_mulResult );
UT_TEST_END()

UT_TEST_BEGIN( Vector3_div_vector )
    Vector3i o1( cv_arithmetic1 );
    Vector3i o2( cv_arithmetic2 );
    Vector3i o3( cv_values     );

    o3 = o1 / o2;

    return allEqual( o3.data, cv_divResult );
UT_TEST_END()

UT_TEST_BEGIN( Vector3_add_scalar_left )
    Vector3i o1( cv_arithmetic1 );
    Vector3i o2( cv_values     );

    o2 = cv_scalar + o1;

    return allEqual( o2.data, cv_addScalarResult );
UT_TEST_END()

UT_TEST_BEGIN( Vector3_add_scalar_right )
    Vector3i o1( cv_arithmetic1 );
    Vector3i o2( cv_values     );

    o2 = o1 + cv_scalar;

    return allEqual( o2.data, cv_addScalarResult );
UT_TEST_END()

UT_TEST_BEGIN( Vector3_sub_scalar )
    Vector3i o1( cv_arithmetic1 );
    Vector3i o2( cv_values     );

    o2 = o1 - cv_scalar;

    return allEqual( o2.data, cv_subScalarResult );
UT_TEST_END()

UT_TEST_BEGIN( Vector3_mul_scalar_left )
    Vector3i o1( cv_arithmetic1 );
    Vector3i o2( cv_values      );

    o2 = cv_scalar * o1;

    return allEqual( o2.data, cv_mulScalarResult );
UT_TEST_END()

UT_TEST_BEGIN( Vector3_mul_scalar_right )
    Vector3i o1( cv_arithmetic1 );
    Vector3i o2( cv_values      );

    o2 = o1 * cv_scalar;

    return allEqual( o2.data, cv_mulScalarResult );
UT_TEST_END()

UT_TEST_BEGIN( Vector3_div_scalar )
    Vector3i o1( cv_arithmetic1 );
    Vector3i o2( cv_values      );

    o2 = o1 / cv_scalar;

    return allEqual( o2.data, cv_divScalarResult );
UT_TEST_END()

UT_TEST_BEGIN( Vector3_dot )
    Vector3i o1( cv_values    );
    Vector3i o2( cv_valuesAlt );

    return dot( o1, o2 ) == cv_dot;
UT_TEST_END()

UT_TEST_BEGIN( Vector3_cross )
    Vector3i o1( cv_arithmetic1 );
    Vector3i o2( cv_arithmetic2 );

    return allEqual( cross( o1, o2 ).data, cv_crossResult );
UT_TEST_END()




#ifdef BS_ZERO

UT_TEST_BEGIN( Vector3_constructorZero )
    Vector3i o;
    return allEqual( o.data, cv_zero );
UT_TEST_END()

#endif //BS_ZERO




#ifdef BS_CHECK_INDEX

UT_TEST_BEGIN( Vector3_index_OOB )
    Vector3i o;
    int i;

    try {
        i = o[(size_t)-1];
        return false;
    } catch( const BoundsException& ) {}

    try {
        i = o[3];
        return false;
    } catch( const BoundsException& ) {}

    return true;
UT_TEST_END()

UT_TEST_BEGIN( Vector3_constIndex_OOB )
    const Vector3i o;
    int i;

    try {
        i = o[(size_t)-1];
        return false;
    } catch( const BoundsException& ) {}

    try {
        i = o[3];
        return false;
    } catch( const BoundsException& ) {}

    return true;
UT_TEST_END()

#endif //BS_CHECK_INDEX

}