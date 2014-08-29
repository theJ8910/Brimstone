/*
test/Matrix3x3.cpp
-----------------------
Copyright (c) 2014, theJ89

Description:
    Unit tests for Matrix<T,3,3> specialization
*/




//Includes
#include "../Test.hpp"
#include "../utils.hpp"         //allEqual, allEqualTo

#include <brimstone/Matrix.hpp>




namespace {
    using ::Brimstone::Matrix3x3i;
    using ::Brimstone::Matrix3x3f;
    using ::Brimstone::Vector3i;
    using ::Brimstone::BoundsException;

    const size_t cv_rows             = 3;
    const size_t cv_cols             = 3;
    const size_t cv_size             = cv_rows * cv_cols;
    const int    cv_zero[9] {
        0, 0, 0,
        0, 0, 0,
        0, 0, 0
    };
    const int    cv_identity[9] {
        1, 0, 0,
        0, 1, 0,
        0, 0, 1
    };
    const int    cv_values[9] {
          1,   2,  3,
          4,   5,  6,
          7,   8,  9
    };
    const int    cv_valuesAlt[9] {
         10,  11, 12,
         13,  14, 15,
         16,  17, 18
    };
    const int    cv_valuesDiagonal[9] {
         10,   2,  3,
          4,  14,  6,
          7,   8, 18
    };
    const int   cv_diagonal[3] { 10, 14, 18 };
    const int   cv_trace               = 15;
    const int   cv_valuesVector[3]     = {   10,   11,   12 };
    const int   cv_mulVectorResult1[3] = {  138,  171,  204 };
    const int   cv_mulVectorResult2[3] = {   68,  167,  266 };
    const int   cv_lowerTriangular[9] {
          1,    0,     0,
          5,    6,     0,
          9,   10,    11
    };
    const int    cv_upperTriangular[9] {
          1,    2,     3,
          0,    6,     7,
          0,    0,    11
    };
    const int    cv_arithmetic1[9] {
          2,    8,    16,
          9,   13,    21,
        -15,  -10,     0
    };
    const int    cv_arithmetic2[9] {
          1,    2,    -2,
          9,  -13,     7,
         -5,    3,   100
    };
    const int    cv_addResult[9] {
          3,   10,    14,
         18,    0,    28,
        -20,   -7,   100
    };
    const int    cv_subResult[9] {
          1,    6,    18,
          0,   26,    14,
        -10,  -13,  -100
    };
    const int    cv_mulResult[9] {
          -6,  -52,  1652,
          21,  -88,  2173,
        -105,  100,   -40
    };
    const int    cv_scalar = 2;
    const int    cv_addScalarResult[9] {
          4,   10,    18,
         11,   15,    23,
        -13,   -8,     2
    };
    const int    cv_subScalarResult[9] {
          0,    6,    14,
          7,   11,    19,
        -17,  -12,    -2
    };
    const int    cv_mulScalarResult[9] {
          4,   16,    32,
         18,   26,    42,
        -30,  -20,     0
    };
    const int    cv_divScalarResult[9] {
          1,    4,     8,
          4,    6,    10,
         -7,   -5,     0
    };
    const int    cv_negateResult[9] {
         -2,   -8,   -16,
         -9,  -13,   -21,
         15,   10,     0
    };
    const int    cv_transpose[9] {
          1,    4,     7,
          2,    5,     8,
          3,    6,     9
    };

    const char*  cv_output =
        "1 2 3\n"
        "4 5 6\n"
        "7 8 9\n";

    const float  cv_valuesF[9] {
         1.0f,  2.0f,  3.0f,
         4.0f,  5.0f,  6.0f,
         7.0f,  8.0f,  9.0f
    };
    const float  cv_valuesAltF[9] {
        10.0f, 11.0f, 12.0f,
        13.0f, 14.0f, 15.0f,
        16.0f, 17.0f, 18.0f
    };
    const float  cv_invertF[9] {
         -1.0f,   2.0f,  13.0f,
         -6.0f,  17.0f,   8.0f,
         11.0f, -22.0f,  -3.0f
    };
    const float  cv_inverseF[9] {
          -25.0f / 140.0f,    56.0f / 140.0f,    41.0f / 140.0f,
          -14.0f / 140.0f,    28.0f / 140.0f,    14.0f / 140.0f,
           11.0f / 140.0f,     0.0f / 140.0f,     1.0f / 140.0f
    };

    const float  cv_determinantF = -700.0f;
    const char*  cv_outputF =
        "1.00000 2.00000 3.00000\n"
        "4.00000 5.00000 6.00000\n"
        "7.00000 8.00000 9.00000\n";
}

namespace UnitTest {

UT_TEST_BEGIN( Matrix3x3_constructorFill )
    Matrix3x3i o( 1 );

    return allEqualTo( o.data, 1 );
UT_TEST_END()

UT_TEST_BEGIN( Matrix3x3_constructorCppRange )
    Matrix3x3i o( cv_values );

    return allEqual( o.data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( Matrix3x3_constructorInitializerList )
    Matrix3x3i o( {
        cv_values[0],  cv_values[1],  cv_values[2],
        cv_values[3],  cv_values[4],  cv_values[5],
        cv_values[6],  cv_values[7],  cv_values[8]
    } );

    return allEqual( o.data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( Matrix3x3_setCppRange )
    Matrix3x3i o( cv_values );

    o.set( cv_valuesAlt );

    return allEqual( o.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( Matrix3x3_setInitializerList )
    Matrix3x3i o( cv_values );

    o.set( {
        cv_valuesAlt[0],  cv_valuesAlt[1],  cv_valuesAlt[2],
        cv_valuesAlt[3],  cv_valuesAlt[4],  cv_valuesAlt[5],
        cv_valuesAlt[6],  cv_valuesAlt[7],  cv_valuesAlt[8]
    } );

    return allEqual( o.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( Matrix3x3_getCppRange )
    int values[ cv_size ];
    copyAll( cv_values, values );
    Matrix3x3i o( cv_valuesAlt );

    o.get( values );

    return allEqual( o.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( Matrix3x3_fill )
    Matrix3x3i o( cv_values );

    o.fill( 1 );

    return allEqualTo( o.data, 1 );
UT_TEST_END()

UT_TEST_BEGIN( Matrix3x3_begin )
    Matrix3x3i o( cv_values );

    return o.begin() == std::begin( o.data );
UT_TEST_END()

UT_TEST_BEGIN( Matrix3x3_beginConst )
    const Matrix3x3i o( cv_values );

    return o.begin() == std::begin( o.data );
UT_TEST_END()

UT_TEST_BEGIN( Matrix3x3_cbegin )
    Matrix3x3i o( cv_values );

    return o.cbegin() == std::cbegin( o.data );
UT_TEST_END()

UT_TEST_BEGIN( Matrix3x3_end )
    Matrix3x3i o( cv_values );

    return o.end() == std::end( o.data );
UT_TEST_END()

UT_TEST_BEGIN( Matrix3x3_endConst )
    const Matrix3x3i o( cv_values );

    return o.end() == std::end( o.data );
UT_TEST_END()

UT_TEST_BEGIN( Matrix3x3_cend )
    Matrix3x3i o( cv_values );

    return o.cend() == std::cend( o.data );
UT_TEST_END()

UT_TEST_BEGIN( Matrix3x3_rangedFor )
    Matrix3x3i o( cv_values );

    int idx = 0;
    for( int& i : o )
        i = cv_valuesAlt[idx++];

    return allEqual( o.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( Matrix3x3_rangedForConst )
    int data[cv_size];
    copyAll( cv_values, data );
    const Matrix3x3i o( cv_valuesAlt );

    int idx = 0;
    for( int i : o )
        data[idx++] = i;

    return allEqual( data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( Matrix3x3_index )
    Matrix3x3i o( cv_values );

    for( size_t i = 0; i < cv_size; ++i )
        o[i] = cv_valuesAlt[i];

    return allEqual( o.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( Matrix3x3_indexConst )
    int data[cv_size];
    const Matrix3x3i o( cv_values );

    for( size_t i = 0; i < cv_size; ++i )
        data[i] = o[i];

    return allEqual( data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( Matrix3x3_constructorValues )
    Matrix3x3i o(
        cv_values[0], cv_values[1], cv_values[2],
        cv_values[3], cv_values[4], cv_values[5],
        cv_values[6], cv_values[7], cv_values[8]
    );

    return allEqual( o.data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( Matrix3x3_setValues )
    Matrix3x3i o( cv_values );

    o.set(
        cv_valuesAlt[0], cv_valuesAlt[1], cv_valuesAlt[2],
        cv_valuesAlt[3], cv_valuesAlt[4], cv_valuesAlt[5],
        cv_valuesAlt[6], cv_valuesAlt[7], cv_valuesAlt[8]
    );

    return allEqual( o.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( Matrix3x3_getValues )
    Matrix3x3i o( cv_values );
    int data[cv_size];

    copyAll( cv_valuesAlt, data );

    o.get(
        data[0], data[1], data[2],
        data[3], data[4], data[5],
        data[6], data[7], data[8]
    );

    return allEqual( data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( Matrix3x3_constructorCopy )
    Matrix3x3f o1( cv_valuesAltF );

    Matrix3x3i o2( o1 );

    return allEqual( o2.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( Matrix3x3_isSquare )
    Matrix3x3i o( cv_values );

    return o.isSquare() == true;
UT_TEST_END()

UT_TEST_BEGIN( Matrix3x3_zero )
    Matrix3x3i o( cv_values );

    o.zero();

    return allEqual( o.data, cv_zero );
UT_TEST_END()

UT_TEST_BEGIN( Matrix3x3_isZero )
    Matrix3x3i o1( cv_zero );
    Matrix3x3i o2( cv_values );

    return o1.isZero() == true  &&
           o2.isZero() == false;
UT_TEST_END()

UT_TEST_BEGIN( Matrix3x3_getDeterminant )
    Matrix3x3f o( cv_invertF );

    return o.getDeterminant() == cv_determinantF;
UT_TEST_END()

UT_TEST_BEGIN( Matrix3x3_getTrace )
    Matrix3x3i o( cv_values );

    return o.getTrace() == cv_trace;
UT_TEST_END()

UT_TEST_BEGIN( Matrix3x3_setRow )
    Matrix3x3i o( cv_valuesAlt );

    o.setRow( 0, Vector3i( cv_values[0], cv_values[1], cv_values[2] ) );
    o.setRow( 1, Vector3i( cv_values[3], cv_values[4], cv_values[5] ) );
    o.setRow( 2, Vector3i( cv_values[6], cv_values[7], cv_values[8] ) );

    return allEqual( o.data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( Matrix3x3_setColumn )
    Matrix3x3i o( cv_valuesAlt );

    o.setColumn( 0, Vector3i( cv_values[0], cv_values[3], cv_values[6] ) );
    o.setColumn( 1, Vector3i( cv_values[1], cv_values[4], cv_values[7] ) );
    o.setColumn( 2, Vector3i( cv_values[2], cv_values[5], cv_values[8] ) );

    return allEqual( o.data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( Matrix3x3_getRow )
    Matrix3x3i o( cv_values );

    return o.getRow( 0 ) == Vector3i( cv_values[0], cv_values[1], cv_values[2] ) &&
           o.getRow( 1 ) == Vector3i( cv_values[3], cv_values[4], cv_values[5] ) &&
           o.getRow( 2 ) == Vector3i( cv_values[6], cv_values[7], cv_values[8] );
UT_TEST_END()

UT_TEST_BEGIN( Matrix3x3_getColumn )
    Matrix3x3i o( cv_values );

    return o.getColumn( 0 ) == Vector3i( cv_values[0], cv_values[3], cv_values[6] ) &&
           o.getColumn( 1 ) == Vector3i( cv_values[1], cv_values[4], cv_values[7] ) &&
           o.getColumn( 2 ) == Vector3i( cv_values[2], cv_values[5], cv_values[8] );
UT_TEST_END()

UT_TEST_BEGIN( Matrix3x3_setDiagonal )
    Matrix3x3i o( cv_values );

    o.setDiagonal( Vector3i( cv_diagonal ) );

    return allEqual( o.data, cv_valuesDiagonal );
UT_TEST_END()

UT_TEST_BEGIN( Matrix3x3_getDiagonal )
    Matrix3x3i o( cv_valuesDiagonal );

    Vector3i v = o.getDiagonal();

    return allEqual( v.data, cv_diagonal );
UT_TEST_END()

UT_TEST_BEGIN( Matrix3x3_isTriangular )
    Matrix3x3i o1( cv_lowerTriangular );
    Matrix3x3i o2( cv_upperTriangular );
    Matrix3x3i o3( cv_values          );

    return o1.isTriangular() == true &&
           o2.isTriangular() == true &&
           o3.isTriangular() == false;
UT_TEST_END()

UT_TEST_BEGIN( Matrix3x3_isLowerTriangular )
    Matrix3x3i o1( cv_lowerTriangular );
    Matrix3x3i o2( cv_values          );

    return o1.isLowerTriangular() == true &&
           o2.isLowerTriangular() == false;
UT_TEST_END()

UT_TEST_BEGIN( Matrix3x3_isUpperTriangular )
    Matrix3x3i o1( cv_upperTriangular );
    Matrix3x3i o2( cv_values          );

    return o1.isUpperTriangular() == true &&
           o2.isUpperTriangular() == false;
UT_TEST_END()

UT_TEST_BEGIN( Matrix3x3_isDiagonal )
    Matrix3x3i o1( cv_identity );
    Matrix3x3i o2( cv_values   );

    return o1.isDiagonal() == true &&
           o2.isDiagonal() == false;
UT_TEST_END()

UT_TEST_BEGIN( Matrix3x3_isInvertible )
    Matrix3x3f o1( cv_invertF );
    Matrix3x3f o2( cv_valuesF );

    return o1.isInvertible() == true &&
           o2.isInvertible() == false;
UT_TEST_END()

UT_TEST_BEGIN( Matrix3x3_transpose )
    Matrix3x3i o( cv_values );

    o.transpose();

    return allEqual( o.data, cv_transpose );
UT_TEST_END()

UT_TEST_BEGIN( Matrix3x3_invert )
    Matrix3x3f o( cv_invertF );

    o.invert();

    return allEqual( o.data, cv_inverseF );
UT_TEST_END()

UT_TEST_BEGIN( Matrix3x3_getRows )
    Matrix3x3i o( cv_values );

    return o.getRows() == cv_rows;
UT_TEST_END()

UT_TEST_BEGIN( Matrix3x3_getColumns )
    Matrix3x3i o( cv_values );

    return o.getColumns() == cv_cols;
UT_TEST_END()

UT_TEST_BEGIN( Matrix3x3_identity )
    Matrix3x3i o( cv_values );

    o.identity();

    return allEqual( o.data, cv_identity );
UT_TEST_END()

UT_TEST_BEGIN( Matrix3x3_isIdentity )
    Matrix3x3i o1( cv_identity );
    Matrix3x3i o2( cv_values );

    return o1.isIdentity() == true &&
           o2.isIdentity() == false;
UT_TEST_END()

UT_TEST_BEGIN( Matrix3x3_assignCopy )
    Matrix3x3f o1( cv_valuesAltF );
    Matrix3x3i o2( cv_values );

    o2 = o1;

    return allEqual( o2.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( Matrix3x3_equals )
    Matrix3x3i o1( cv_values );
    Matrix3x3i o2( cv_values );
    Matrix3x3i o3( cv_valuesAlt );

    return ( o1 == o2 ) == true   &&
           ( o1 == o3 ) == false;
UT_TEST_END()

UT_TEST_BEGIN( Matrix3x3_notEquals )
    Matrix3x3i o1( cv_values );
    Matrix3x3i o2( cv_values );
    Matrix3x3i o3( cv_valuesAlt );

    return ( o1 != o2 ) == false  &&
           ( o1 != o3 ) == true;
UT_TEST_END()

UT_TEST_BEGIN( Matrix3x3_output_int )
    Matrix3x3i o( cv_values );

    std::stringstream sout;
    sout << o;

    return sout.str() == cv_output;
UT_TEST_END()

UT_TEST_BEGIN( Matrix3x3_output_float )
    Matrix3x3f o( cv_valuesF );

    std::stringstream sout;
    sout << o;

    return sout.str() == cv_outputF;
UT_TEST_END()

UT_TEST_BEGIN( Matrix3x3_addAssign_matrix )
    Matrix3x3i o1( cv_arithmetic1 );
    Matrix3x3i o2( cv_arithmetic2 );

    o1 += o2;

    return allEqual( o1.data, cv_addResult );
UT_TEST_END()

UT_TEST_BEGIN( Matrix3x3_subAssign_matrix )
    Matrix3x3i o1( cv_arithmetic1 );
    Matrix3x3i o2( cv_arithmetic2 );

    o1 -= o2;

    return allEqual( o1.data, cv_subResult );
UT_TEST_END()

UT_TEST_BEGIN( Matrix3x3_mulAssign_matrix )
    Matrix3x3i o1( cv_arithmetic1 );
    Matrix3x3i o2( cv_arithmetic2 );

    o1 *= o2;

    return allEqual( o1.data, cv_mulResult );
UT_TEST_END()

UT_TEST_BEGIN( Matrix3x3_mulAssign_vector )
    Matrix3x3i o( cv_values );
    Vector3i   v( cv_valuesVector );

    v *= o;

    return allEqual( v.data, cv_mulVectorResult1 );
UT_TEST_END()

UT_TEST_BEGIN( Matrix3x3_addAssign_scalar )
    Matrix3x3i o( cv_arithmetic1 );

    o += cv_scalar;

    return allEqual( o.data, cv_addScalarResult );
UT_TEST_END()

UT_TEST_BEGIN( Matrix3x3_subAssign_scalar )
    Matrix3x3i o( cv_arithmetic1 );

    o -= cv_scalar;

    return allEqual( o.data, cv_subScalarResult );
UT_TEST_END()

UT_TEST_BEGIN( Matrix3x3_mulAssign_scalar )
    Matrix3x3i o( cv_arithmetic1 );

    o *= cv_scalar;

    return allEqual( o.data, cv_mulScalarResult );
UT_TEST_END()

UT_TEST_BEGIN( Matrix3x3_divAssign_scalar )
    Matrix3x3i o( cv_arithmetic1 );

    o /= cv_scalar;

    return allEqual( o.data, cv_divScalarResult );
UT_TEST_END()

UT_TEST_BEGIN( Matrix3x3_elem )
    Matrix3x3i o( cv_values );

    for( size_t r = 0; r < cv_rows; ++r )
        for( size_t c = 0; c < cv_cols; ++c )
            o( r, c ) = cv_valuesAlt[ r * cv_cols + c ];

    return allEqual( o.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( Matrix3x3_elemConst )
    const Matrix3x3i o( cv_values );

    for( size_t r = 0; r < cv_rows; ++r )
        for( size_t c = 0; c < cv_cols; ++c )
            if( o( r, c ) != cv_values[ r * cv_cols + c ] )
                return false;

    return true;
UT_TEST_END()

UT_TEST_BEGIN( Matrix3x3_unaryPlus )
    Matrix3x3i o1( cv_arithmetic1 );
    Matrix3x3i o2( cv_arithmetic2 );

    o2 = +o1;

    return allEqual( o2.data, cv_arithmetic1 );
UT_TEST_END()

UT_TEST_BEGIN( Matrix3x3_unaryMinus )
    Matrix3x3i o1( cv_arithmetic1 );
    Matrix3x3i o2( cv_arithmetic2 );

    o2 = -o1;

    return allEqual( o2.data, cv_negateResult );
UT_TEST_END()

UT_TEST_BEGIN( Matrix3x3_add_matrix )
    Matrix3x3i o1( cv_arithmetic1 );
    Matrix3x3i o2( cv_arithmetic2 );
    Matrix3x3i o3( cv_values );

    o3 = o1 + o2;

    return allEqual( o3.data, cv_addResult );
UT_TEST_END()

UT_TEST_BEGIN( Matrix3x3_sub_matrix )
    Matrix3x3i o1( cv_arithmetic1 );
    Matrix3x3i o2( cv_arithmetic2 );
    Matrix3x3i o3( cv_values );

    o3 = o1 - o2;

    return allEqual( o3.data, cv_subResult );
UT_TEST_END()

UT_TEST_BEGIN( Matrix3x3_mul_matrix )
    Matrix3x3i o1( cv_arithmetic1 );
    Matrix3x3i o2( cv_arithmetic2 );
    Matrix3x3i o3( cv_values );

    o3 = o1 * o2;

    return allEqual( o3.data, cv_mulResult );
UT_TEST_END()

UT_TEST_BEGIN( Matrix3x3_mul_vector_left )
    Matrix3x3i o( cv_values );
    Vector3i   v1( cv_valuesVector );

    Vector3i   v2 = v1 * o;

    return allEqual( v2.data, cv_mulVectorResult1 );
UT_TEST_END()

UT_TEST_BEGIN( Matrix3x3_mul_vector_right )
    Matrix3x3i o( cv_values );
    Vector3i   v1( cv_valuesVector );

    Vector3i   v2 = o * v1;

    return allEqual( v2.data, cv_mulVectorResult2 );
UT_TEST_END()

UT_TEST_BEGIN( Matrix3x3_add_scalar_left )
    Matrix3x3i o1( cv_arithmetic1 );
    Matrix3x3i o2( cv_values );

    o2 = cv_scalar + o1;

    return allEqual( o2.data, cv_addScalarResult );
UT_TEST_END()

UT_TEST_BEGIN( Matrix3x3_add_scalar_right )
    Matrix3x3i o1( cv_arithmetic1 );
    Matrix3x3i o2( cv_values );

    o2 = o1 + cv_scalar;

    return allEqual( o2.data, cv_addScalarResult );
UT_TEST_END()

UT_TEST_BEGIN( Matrix3x3_sub_scalar )
    Matrix3x3i o1( cv_arithmetic1 );
    Matrix3x3i o2( cv_values );

    o2 = o1 - cv_scalar;

    return allEqual( o2.data, cv_subScalarResult );
UT_TEST_END()

UT_TEST_BEGIN( Matrix3x3_mul_scalar_left )
    Matrix3x3i o1( cv_arithmetic1 );
    Matrix3x3i o2( cv_values );

    o2 = cv_scalar * o1;

    return allEqual( o2.data, cv_mulScalarResult );
UT_TEST_END()

UT_TEST_BEGIN( Matrix3x3_mul_scalar_right )
    Matrix3x3i o1( cv_arithmetic1 );
    Matrix3x3i o2( cv_values );

    o2 = o1 * cv_scalar;

    return allEqual( o2.data, cv_mulScalarResult );
UT_TEST_END()

UT_TEST_BEGIN( Matrix3x3_div_scalar )
    Matrix3x3i o1( cv_arithmetic1 );
    Matrix3x3i o2( cv_values );

    o2 = o1 / cv_scalar;

    return allEqual( o2.data, cv_divScalarResult );
UT_TEST_END()

UT_TEST_BEGIN( Matrix3x3_transpose_free )
    Matrix3x3i o1( cv_values );
    Matrix3x3i o2( cv_values );

    o2 = transpose( o1 );

    return allEqual( o2.data, cv_transpose );
UT_TEST_END()

UT_TEST_BEGIN( Matrix3x3_invert_free )
    Matrix3x3f o1( cv_invertF );

    Matrix3x3f o2 = Brimstone::invert( o1 );

    return allEqual( o2.data, cv_inverseF );
UT_TEST_END()




#ifdef BS_ZERO

UT_TEST_BEGIN( Matrix3x3_constructorZero )
    Matrix3x3i o;
    return allEqual( o.data, cv_identity );
UT_TEST_END()

#endif //BS_ZERO




#ifdef BS_CHECK_INDEX

UT_TEST_BEGIN( Matrix3x3_index_OOB )
    Matrix3x3i o;
    int i;

    try {
        i = o[(size_t)-1];
        return false;
    } catch( const BoundsException& ) {}

    try {
        i = o[25];
        return false;
    } catch( const BoundsException& ) {}

    return true;
UT_TEST_END()

UT_TEST_BEGIN( Matrix3x3_constIndex_OOB )
    const Matrix3x3i o;
    int i;

    try {
        i = o[(size_t)-1];
        return false;
    } catch( const BoundsException& ) {}

    try {
        i = o[25];
        return false;
    } catch( const BoundsException& ) {}

    return true;
UT_TEST_END()

UT_TEST_BEGIN( Matrix3x3_setRow_OOB )
    Matrix3x3i o;
    Vector3i v;

    try {
        o.setRow( (size_t)-1, v );
        return false;
    } catch( const BoundsException& ) {}

    try {
        o.setRow( 3, v );
        return false;
    } catch( const BoundsException& ) {}

    return true;
UT_TEST_END()

UT_TEST_BEGIN( Matrix3x3_setColumn_OOB )
    Matrix3x3i o;
    Vector3i v;

    try {
        o.setColumn( (size_t)-1, v );
        return false;
    } catch( const BoundsException& ) {}

    try {
        o.setColumn( 3, v );
        return false;
    } catch( const BoundsException& ) {}

    return true;
UT_TEST_END()

UT_TEST_BEGIN( Matrix3x3_elem_OOB )
    Matrix3x3i o;
    int i;

    try {
        i = o( (size_t)-1, 0 );
        return false;
    } catch( const BoundsException& ) {}

    try {
        i = o( 0, (size_t)-1 );
        return false;
    } catch( const BoundsException& ) {}

    try {
        i = o( 5, 0 );
        return false;
    } catch( const BoundsException& ) {}

    try {
        i = o( 0, 5 );
        return false;
    } catch( const BoundsException& ) {}

    return true;
UT_TEST_END()

UT_TEST_BEGIN( Matrix3x3_elemConst_OOB )
    const Matrix3x3i o;
    int i;

    try {
        i = o( (size_t)-1, 0 );
        return false;
    } catch( const BoundsException& ) {}

    try {
        i = o( 0, (size_t)-1 );
        return false;
    } catch( const BoundsException& ) {}

    try {
        i = o( 5, 0 );
        return false;
    } catch( const BoundsException& ) {}

    try {
        i = o( 0, 5 );
        return false;
    } catch( const BoundsException& ) {}

    return true;
UT_TEST_END()

#endif //BS_CHECK_INDEX

}