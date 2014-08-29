/*
test/Matrix4x4.cpp
-----------------------
Copyright (c) 2014, theJ89

Description:
    Unit tests for Matrix<T,4,4> specialization
*/




//Includes
#include "../Test.hpp"
#include "../utils.hpp"         //allEqual, allEqualTo

#include <brimstone/Matrix.hpp>




namespace {
    using ::Brimstone::Matrix4x4i;
    using ::Brimstone::Matrix4x4f;
    using ::Brimstone::Vector4i;
    using ::Brimstone::BoundsException;

    const size_t cv_rows             = 4;
    const size_t cv_cols             = 4;
    const size_t cv_size             = cv_rows * cv_cols;
    const int    cv_zero[16] {
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0
    };
    const int    cv_identity[16] {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    };
    const int    cv_values[16] {
          1,    2,     3,    4,
          5,    6,     7,    8,
          9,   10,    11,   12,
         13,   14,    15,   16,
    };
    const int    cv_valuesAlt[16] {
         17,   18,    19,   20,
         21,   22,    23,   24,
         25,   26,    27,   28,
         29,   30,    31,   32
    };
    const int    cv_valuesDiagonal[16] {
         17,    2,     3,    4,
          5,   22,     7,    8,
          9,   10,    27,   12,
         13,   14,    15,   32
    };
    const int   cv_diagonal[4] { 17, 22, 27, 32 };
    const int   cv_trace               = 34;
    const int   cv_valuesVector[4]     = {   17,   18,   19,   20 };
    const int   cv_mulVectorResult1[4] = {  538,  612,  686,  760 };
    const int   cv_mulVectorResult2[4] = {  190,  486,  782, 1078 };
    const int   cv_lowerTriangular[16] {
          1,    0,     0,    0,
          5,    6,     0,    0,
          9,   10,    11,    0,
         13,   14,    15,   16
    };
    const int    cv_upperTriangular[16] {
          1,    2,     3,    4,
          0,    6,     7,    8,
          0,    0,    11,   12,
          0,    0,     0,   16
    };
    const int    cv_arithmetic1[16] {
          2,    8,    16,   32,
          9,   13,    21,   25,
        -15,  -10,     0,   10,
         14,   52,   100,   50
    };
    const int    cv_arithmetic2[16] {
          1,    2,    -2,    4,
          9,  -13,     7,    5,
         -5,    3,   100,  -10,
          7,   16,    50,   10
    };
    const int    cv_addResult[16] {
          3,   10,    14,   36,
         18,    0,    28,   30,
        -20,   -7,   100,    0,
         21,   68,   150,   60
    };
    const int    cv_subResult[16] {
          1,    6,    18,   28,
          0,   26,    14,   20,
        -10,  -13,  -100,   20,
          7,   36,    50,   40
    };
    const int    cv_mulResult[16] {
         218,  460,  3252,  208,
         196,  312,  3423,  141,
         -35,  260,   460,  -10,
         332,  452, 12836, -184
    };
    const int    cv_scalar = 2;
    const int    cv_addScalarResult[16] {
          4,   10,    18,   34,
         11,   15,    23,   27,
        -13,   -8,     2,   12,
         16,   54,   102,   52
    };
    const int    cv_subScalarResult[16] {
          0,    6,    14,   30,
          7,   11,    19,   23,
        -17,  -12,    -2,    8,
         12,   50,    98,   48
    };
    const int    cv_mulScalarResult[16] {
          4,   16,    32,   64,
         18,   26,    42,   50,
        -30,  -20,     0,   20,
         28,  104,   200,  100
    };
    const int    cv_divScalarResult[16] {
          1,    4,     8,   16,
          4,    6,    10,   12,
         -7,   -5,     0,    5,
          7,   26,    50,   25,
    };
    const int    cv_negateResult[16] {
         -2,   -8,   -16,  -32,
         -9,  -13,   -21,  -25,
         15,   10,     0,  -10,
        -14,  -52,  -100,  -50
    };
    const int    cv_transpose[16] {
          1,    5,     9,   13,
          2,    6,    10,   14,
          3,    7,    11,   15,
          4,    8,    12,   16
    };

    const char*  cv_output =
        " 1  2  3  4\n"
        " 5  6  7  8\n"
        " 9 10 11 12\n"
        "13 14 15 16\n";

    const float  cv_valuesF[16] {
         1.0f,  2.0f,  3.0f,  4.0f,
         5.0f,  6.0f,  7.0f,  8.0f,
         9.0f, 10.0f, 11.0f, 12.0f,
        13.0f, 14.0f, 15.0f, 16.0f
    };
    const float  cv_valuesAltF[16] {
        17.0f, 18.0f, 19.0f, 20.0f,
        21.0f, 22.0f, 23.0f, 24.0f,
        25.0f, 26.0f, 27.0f, 28.0f,
        29.0f, 30.0f, 31.0f, 32.0f
    };
    const float  cv_invertF[16] {
         -1.0f,   2.0f,  13.0f,  -4.0f,
         -6.0f,  17.0f,   8.0f,  -9.0f,
         11.0f, -22.0f,  -3.0f, -14.0f,
         16.0f,  -7.0f, -18.0f,  19.0f
    };
    const float  cv_inverseF[16] {
          450.0f / 7260.0f,   459.0f / 7260.0f,   240.0f / 7260.0f,  489.0f / 7260.0f,
          -26.0f / 7260.0f,   472.0f / 7260.0f,   -30.0f / 7260.0f,  196.0f / 7260.0f,
          674.0f / 7260.0f,  -145.0f / 7260.0f,   -60.0f / 7260.0f,   29.0f / 7260.0f,
          250.0f / 7260.0f,  -350.0f / 7260.0f,  -270.0f / 7260.0f,   70.0f / 7260.0f,
    };

    const float  cv_determinantF = -72600.0f;
    const char*  cv_outputF =
        " 1.00000  2.00000  3.00000  4.00000\n"
        " 5.00000  6.00000  7.00000  8.00000\n"
        " 9.00000 10.00000 11.00000 12.00000\n"
        "13.00000 14.00000 15.00000 16.00000\n";
}

namespace UnitTest {

UT_TEST_BEGIN( Matrix4x4_constructorFill )
    Matrix4x4i o( 1 );

    return allEqualTo( o.data, 1 );
UT_TEST_END()

UT_TEST_BEGIN( Matrix4x4_constructorCppRange )
    Matrix4x4i o( cv_values );

    return allEqual( o.data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( Matrix4x4_constructorInitializerList )
    Matrix4x4i o( {
        cv_values[0],  cv_values[1],  cv_values[2],  cv_values[3],
        cv_values[4],  cv_values[5],  cv_values[6],  cv_values[7],
        cv_values[8],  cv_values[9],  cv_values[10], cv_values[11],
        cv_values[12], cv_values[13], cv_values[14], cv_values[15]
    } );

    return allEqual( o.data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( Matrix4x4_setCppRange )
    Matrix4x4i o( cv_values );

    o.set( cv_valuesAlt );

    return allEqual( o.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( Matrix4x4_setInitializerList )
    Matrix4x4i o( cv_values );

    o.set( {
        cv_valuesAlt[0],  cv_valuesAlt[1],  cv_valuesAlt[2],  cv_valuesAlt[3],
        cv_valuesAlt[4],  cv_valuesAlt[5],  cv_valuesAlt[6],  cv_valuesAlt[7],
        cv_valuesAlt[8],  cv_valuesAlt[9],  cv_valuesAlt[10], cv_valuesAlt[11],
        cv_valuesAlt[12], cv_valuesAlt[13], cv_valuesAlt[14], cv_valuesAlt[15]
    } );

    return allEqual( o.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( Matrix4x4_getCppRange )
    int values[ cv_size ];
    copyAll( cv_values, values );
    Matrix4x4i o( cv_valuesAlt );

    o.get( values );

    return allEqual( o.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( Matrix4x4_fill )
    Matrix4x4i o( cv_values );

    o.fill( 1 );

    return allEqualTo( o.data, 1 );
UT_TEST_END()

UT_TEST_BEGIN( Matrix4x4_begin )
    Matrix4x4i o( cv_values );

    return o.begin() == std::begin( o.data );
UT_TEST_END()

UT_TEST_BEGIN( Matrix4x4_beginConst )
    const Matrix4x4i o( cv_values );

    return o.begin() == std::begin( o.data );
UT_TEST_END()

UT_TEST_BEGIN( Matrix4x4_cbegin )
    Matrix4x4i o( cv_values );

    return o.cbegin() == std::cbegin( o.data );
UT_TEST_END()

UT_TEST_BEGIN( Matrix4x4_end )
    Matrix4x4i o( cv_values );

    return o.end() == std::end( o.data );
UT_TEST_END()

UT_TEST_BEGIN( Matrix4x4_endConst )
    const Matrix4x4i o( cv_values );

    return o.end() == std::end( o.data );
UT_TEST_END()

UT_TEST_BEGIN( Matrix4x4_cend )
    Matrix4x4i o( cv_values );

    return o.cend() == std::cend( o.data );
UT_TEST_END()

UT_TEST_BEGIN( Matrix4x4_rangedFor )
    Matrix4x4i o( cv_values );

    int idx = 0;
    for( int& i : o )
        i = cv_valuesAlt[idx++];

    return allEqual( o.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( Matrix4x4_rangedForConst )
    int data[cv_size];
    copyAll( cv_values, data );
    const Matrix4x4i o( cv_valuesAlt );

    int idx = 0;
    for( int i : o )
        data[idx++] = i;

    return allEqual( data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( Matrix4x4_index )
    Matrix4x4i o( cv_values );

    for( size_t i = 0; i < cv_size; ++i )
        o[i] = cv_valuesAlt[i];

    return allEqual( o.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( Matrix4x4_indexConst )
    int data[cv_size];
    const Matrix4x4i o( cv_values );

    for( size_t i = 0; i < cv_size; ++i )
        data[i] = o[i];

    return allEqual( data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( Matrix4x4_constructorValues )
    Matrix4x4i o(
        cv_values[ 0], cv_values[ 1], cv_values[ 2], cv_values[ 3],
        cv_values[ 4], cv_values[ 5], cv_values[ 6], cv_values[ 7],
        cv_values[ 8], cv_values[ 9], cv_values[10], cv_values[11],
        cv_values[12], cv_values[13], cv_values[14], cv_values[15]
    );

    return allEqual( o.data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( Matrix4x4_setValues )
    Matrix4x4i o( cv_values );

    o.set(
        cv_valuesAlt[ 0], cv_valuesAlt[ 1], cv_valuesAlt[ 2], cv_valuesAlt[ 3],
        cv_valuesAlt[ 4], cv_valuesAlt[ 5], cv_valuesAlt[ 6], cv_valuesAlt[ 7],
        cv_valuesAlt[ 8], cv_valuesAlt[ 9], cv_valuesAlt[10], cv_valuesAlt[11],
        cv_valuesAlt[12], cv_valuesAlt[13], cv_valuesAlt[14], cv_valuesAlt[15]
    );

    return allEqual( o.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( Matrix4x4_getValues )
    Matrix4x4i o( cv_values );
    int data[cv_size];

    copyAll( cv_valuesAlt, data );

    o.get(
        data[ 0], data[ 1], data[ 2], data[ 3],
        data[ 4], data[ 5], data[ 6], data[ 7],
        data[ 8], data[ 9], data[10], data[11],
        data[12], data[13], data[14], data[15]
    );

    return allEqual( data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( Matrix4x4_constructorCopy )
    Matrix4x4f o1( cv_valuesAltF );

    Matrix4x4i o2( o1 );

    return allEqual( o2.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( Matrix4x4_isSquare )
    Matrix4x4i o( cv_values );

    return o.isSquare() == true;
UT_TEST_END()

UT_TEST_BEGIN( Matrix4x4_zero )
    Matrix4x4i o( cv_values );

    o.zero();

    return allEqual( o.data, cv_zero );
UT_TEST_END()

UT_TEST_BEGIN( Matrix4x4_isZero )
    Matrix4x4i o1( cv_zero );
    Matrix4x4i o2( cv_values );

    return o1.isZero() == true  &&
           o2.isZero() == false;
UT_TEST_END()

UT_TEST_BEGIN( Matrix4x4_getDeterminant )
    Matrix4x4f o( cv_invertF );

    return o.getDeterminant() == cv_determinantF;
UT_TEST_END()

UT_TEST_BEGIN( Matrix4x4_getTrace )
    Matrix4x4i o( cv_values );

    return o.getTrace() == cv_trace;
UT_TEST_END()

UT_TEST_BEGIN( Matrix4x4_setRow )
    Matrix4x4i o( cv_valuesAlt );

    o.setRow( 0, Vector4i( cv_values[0],  cv_values[1],  cv_values[2],  cv_values[3]  ) );
    o.setRow( 1, Vector4i( cv_values[4],  cv_values[5],  cv_values[6],  cv_values[7]  ) );
    o.setRow( 2, Vector4i( cv_values[8],  cv_values[9],  cv_values[10], cv_values[11] ) );
    o.setRow( 3, Vector4i( cv_values[12], cv_values[13], cv_values[14], cv_values[15] ) );

    return allEqual( o.data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( Matrix4x4_setColumn )
    Matrix4x4i o( cv_valuesAlt );

    o.setColumn( 0, Vector4i( cv_values[0],  cv_values[4], cv_values[8],  cv_values[12] ) );
    o.setColumn( 1, Vector4i( cv_values[1],  cv_values[5], cv_values[9],  cv_values[13] ) );
    o.setColumn( 2, Vector4i( cv_values[2],  cv_values[6], cv_values[10], cv_values[14] ) );
    o.setColumn( 3, Vector4i( cv_values[3],  cv_values[7], cv_values[11], cv_values[15] ) );

    return allEqual( o.data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( Matrix4x4_getRow )
    Matrix4x4i o( cv_values );

    return o.getRow( 0 ) == Vector4i( cv_values[0],  cv_values[1],  cv_values[2],  cv_values[3]  ) &&
           o.getRow( 1 ) == Vector4i( cv_values[4],  cv_values[5],  cv_values[6],  cv_values[7]  ) &&
           o.getRow( 2 ) == Vector4i( cv_values[8],  cv_values[9],  cv_values[10], cv_values[11] ) &&
           o.getRow( 3 ) == Vector4i( cv_values[12], cv_values[13], cv_values[14], cv_values[15] );
UT_TEST_END()

UT_TEST_BEGIN( Matrix4x4_getColumn )
    Matrix4x4i o( cv_values );

    return o.getColumn( 0 ) == Vector4i( cv_values[0], cv_values[4], cv_values[8],  cv_values[12] ) &&
           o.getColumn( 1 ) == Vector4i( cv_values[1], cv_values[5], cv_values[9],  cv_values[13] ) &&
           o.getColumn( 2 ) == Vector4i( cv_values[2], cv_values[6], cv_values[10], cv_values[14] ) &&
           o.getColumn( 3 ) == Vector4i( cv_values[3], cv_values[7], cv_values[11], cv_values[15] );
UT_TEST_END()

UT_TEST_BEGIN( Matrix4x4_setDiagonal )
    Matrix4x4i o( cv_values );

    o.setDiagonal( Vector4i( cv_diagonal ) );

    return allEqual( o.data, cv_valuesDiagonal );
UT_TEST_END()

UT_TEST_BEGIN( Matrix4x4_getDiagonal )
    Matrix4x4i o( cv_valuesDiagonal );

    Vector4i v = o.getDiagonal();

    return allEqual( v.data, cv_diagonal );
UT_TEST_END()

UT_TEST_BEGIN( Matrix4x4_isTriangular )
    Matrix4x4i o1( cv_lowerTriangular );
    Matrix4x4i o2( cv_upperTriangular );
    Matrix4x4i o3( cv_values          );

    return o1.isTriangular() == true &&
           o2.isTriangular() == true &&
           o3.isTriangular() == false;
UT_TEST_END()

UT_TEST_BEGIN( Matrix4x4_isLowerTriangular )
    Matrix4x4i o1( cv_lowerTriangular );
    Matrix4x4i o2( cv_values          );

    return o1.isLowerTriangular() == true &&
           o2.isLowerTriangular() == false;
UT_TEST_END()

UT_TEST_BEGIN( Matrix4x4_isUpperTriangular )
    Matrix4x4i o1( cv_upperTriangular );
    Matrix4x4i o2( cv_values          );

    return o1.isUpperTriangular() == true &&
           o2.isUpperTriangular() == false;
UT_TEST_END()

UT_TEST_BEGIN( Matrix4x4_isDiagonal )
    Matrix4x4i o1( cv_identity );
    Matrix4x4i o2( cv_values   );

    return o1.isDiagonal() == true &&
           o2.isDiagonal() == false;
UT_TEST_END()

UT_TEST_BEGIN( Matrix4x4_isInvertible )
    Matrix4x4f o1( cv_invertF );
    Matrix4x4f o2( cv_valuesF );

    return o1.isInvertible() == true &&
           o2.isInvertible() == false;
UT_TEST_END()

UT_TEST_BEGIN( Matrix4x4_transpose )
    Matrix4x4i o( cv_values );

    o.transpose();

    return allEqual( o.data, cv_transpose );
UT_TEST_END()

UT_TEST_BEGIN( Matrix4x4_invert )
    Matrix4x4f o( cv_invertF );

    o.invert();

    return allEqual( o.data, cv_inverseF );
UT_TEST_END()

UT_TEST_BEGIN( Matrix4x4_getRows )
    Matrix4x4i o( cv_values );

    return o.getRows() == cv_rows;
UT_TEST_END()

UT_TEST_BEGIN( Matrix4x4_getColumns )
    Matrix4x4i o( cv_values );

    return o.getColumns() == cv_cols;
UT_TEST_END()

UT_TEST_BEGIN( Matrix4x4_identity )
    Matrix4x4i o( cv_values );

    o.identity();

    return allEqual( o.data, cv_identity );
UT_TEST_END()

UT_TEST_BEGIN( Matrix4x4_isIdentity )
    Matrix4x4i o1( cv_identity );
    Matrix4x4i o2( cv_values );

    return o1.isIdentity() == true &&
           o2.isIdentity() == false;
UT_TEST_END()

UT_TEST_BEGIN( Matrix4x4_assignCopy )
    Matrix4x4f o1( cv_valuesAltF );
    Matrix4x4i o2( cv_values );

    o2 = o1;

    return allEqual( o2.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( Matrix4x4_equals )
    Matrix4x4i o1( cv_values );
    Matrix4x4i o2( cv_values );
    Matrix4x4i o3( cv_valuesAlt );

    return ( o1 == o2 ) == true   &&
           ( o1 == o3 ) == false;
UT_TEST_END()

UT_TEST_BEGIN( Matrix4x4_notEquals )
    Matrix4x4i o1( cv_values );
    Matrix4x4i o2( cv_values );
    Matrix4x4i o3( cv_valuesAlt );

    return ( o1 != o2 ) == false  &&
           ( o1 != o3 ) == true;
UT_TEST_END()

UT_TEST_BEGIN( Matrix4x4_output_int )
    Matrix4x4i o( cv_values );

    std::stringstream sout;
    sout << o;

    return sout.str() == cv_output;
UT_TEST_END()

UT_TEST_BEGIN( Matrix4x4_output_float )
    Matrix4x4f o( cv_valuesF );

    std::stringstream sout;
    sout << o;

    return sout.str() == cv_outputF;
UT_TEST_END()

UT_TEST_BEGIN( Matrix4x4_addAssign_matrix )
    Matrix4x4i o1( cv_arithmetic1 );
    Matrix4x4i o2( cv_arithmetic2 );

    o1 += o2;

    return allEqual( o1.data, cv_addResult );
UT_TEST_END()

UT_TEST_BEGIN( Matrix4x4_subAssign_matrix )
    Matrix4x4i o1( cv_arithmetic1 );
    Matrix4x4i o2( cv_arithmetic2 );

    o1 -= o2;

    return allEqual( o1.data, cv_subResult );
UT_TEST_END()

UT_TEST_BEGIN( Matrix4x4_mulAssign_matrix )
    Matrix4x4i o1( cv_arithmetic1 );
    Matrix4x4i o2( cv_arithmetic2 );

    o1 *= o2;

    return allEqual( o1.data, cv_mulResult );
UT_TEST_END()

UT_TEST_BEGIN( Matrix4x4_mulAssign_vector )
    Matrix4x4i o( cv_values );
    Vector4i   v( cv_valuesVector );

    v *= o;

    return allEqual( v.data, cv_mulVectorResult1 );
UT_TEST_END()

UT_TEST_BEGIN( Matrix4x4_addAssign_scalar )
    Matrix4x4i o( cv_arithmetic1 );

    o += cv_scalar;

    return allEqual( o.data, cv_addScalarResult );
UT_TEST_END()

UT_TEST_BEGIN( Matrix4x4_subAssign_scalar )
    Matrix4x4i o( cv_arithmetic1 );

    o -= cv_scalar;

    return allEqual( o.data, cv_subScalarResult );
UT_TEST_END()

UT_TEST_BEGIN( Matrix4x4_mulAssign_scalar )
    Matrix4x4i o( cv_arithmetic1 );

    o *= cv_scalar;

    return allEqual( o.data, cv_mulScalarResult );
UT_TEST_END()

UT_TEST_BEGIN( Matrix4x4_divAssign_scalar )
    Matrix4x4i o( cv_arithmetic1 );

    o /= cv_scalar;

    return allEqual( o.data, cv_divScalarResult );
UT_TEST_END()

UT_TEST_BEGIN( Matrix4x4_elem )
    Matrix4x4i o( cv_values );

    for( size_t r = 0; r < cv_rows; ++r )
        for( size_t c = 0; c < cv_cols; ++c )
            o( r, c ) = cv_valuesAlt[ r * cv_cols + c ];

    return allEqual( o.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( Matrix4x4_elemConst )
    const Matrix4x4i o( cv_values );

    for( size_t r = 0; r < cv_rows; ++r )
        for( size_t c = 0; c < cv_cols; ++c )
            if( o( r, c ) != cv_values[ r * cv_cols + c ] )
                return false;

    return true;
UT_TEST_END()

UT_TEST_BEGIN( Matrix4x4_unaryPlus )
    Matrix4x4i o1( cv_arithmetic1 );
    Matrix4x4i o2( cv_arithmetic2 );

    o2 = +o1;

    return allEqual( o2.data, cv_arithmetic1 );
UT_TEST_END()

UT_TEST_BEGIN( Matrix4x4_unaryMinus )
    Matrix4x4i o1( cv_arithmetic1 );
    Matrix4x4i o2( cv_arithmetic2 );

    o2 = -o1;

    return allEqual( o2.data, cv_negateResult );
UT_TEST_END()

UT_TEST_BEGIN( Matrix4x4_add_matrix )
    Matrix4x4i o1( cv_arithmetic1 );
    Matrix4x4i o2( cv_arithmetic2 );
    Matrix4x4i o3( cv_values );

    o3 = o1 + o2;

    return allEqual( o3.data, cv_addResult );
UT_TEST_END()

UT_TEST_BEGIN( Matrix4x4_sub_matrix )
    Matrix4x4i o1( cv_arithmetic1 );
    Matrix4x4i o2( cv_arithmetic2 );
    Matrix4x4i o3( cv_values );

    o3 = o1 - o2;

    return allEqual( o3.data, cv_subResult );
UT_TEST_END()

UT_TEST_BEGIN( Matrix4x4_mul_matrix )
    Matrix4x4i o1( cv_arithmetic1 );
    Matrix4x4i o2( cv_arithmetic2 );
    Matrix4x4i o3( cv_values );

    o3 = o1 * o2;

    return allEqual( o3.data, cv_mulResult );
UT_TEST_END()

UT_TEST_BEGIN( Matrix4x4_mul_vector_left )
    Matrix4x4i o( cv_values );
    Vector4i   v1( cv_valuesVector );

    Vector4i   v2 = v1 * o;

    return allEqual( v2.data, cv_mulVectorResult1 );
UT_TEST_END()

UT_TEST_BEGIN( Matrix4x4_mul_vector_right )
    Matrix4x4i o( cv_values );
    Vector4i   v1( cv_valuesVector );

    Vector4i   v2 = o * v1;

    return allEqual( v2.data, cv_mulVectorResult2 );
UT_TEST_END()

UT_TEST_BEGIN( Matrix4x4_add_scalar_left )
    Matrix4x4i o1( cv_arithmetic1 );
    Matrix4x4i o2( cv_values );

    o2 = cv_scalar + o1;

    return allEqual( o2.data, cv_addScalarResult );
UT_TEST_END()

UT_TEST_BEGIN( Matrix4x4_add_scalar_right )
    Matrix4x4i o1( cv_arithmetic1 );
    Matrix4x4i o2( cv_values );

    o2 = o1 + cv_scalar;

    return allEqual( o2.data, cv_addScalarResult );
UT_TEST_END()

UT_TEST_BEGIN( Matrix4x4_sub_scalar )
    Matrix4x4i o1( cv_arithmetic1 );
    Matrix4x4i o2( cv_values );

    o2 = o1 - cv_scalar;

    return allEqual( o2.data, cv_subScalarResult );
UT_TEST_END()

UT_TEST_BEGIN( Matrix4x4_mul_scalar_left )
    Matrix4x4i o1( cv_arithmetic1 );
    Matrix4x4i o2( cv_values );

    o2 = cv_scalar * o1;

    return allEqual( o2.data, cv_mulScalarResult );
UT_TEST_END()

UT_TEST_BEGIN( Matrix4x4_mul_scalar_right )
    Matrix4x4i o1( cv_arithmetic1 );
    Matrix4x4i o2( cv_values );

    o2 = o1 * cv_scalar;

    return allEqual( o2.data, cv_mulScalarResult );
UT_TEST_END()

UT_TEST_BEGIN( Matrix4x4_div_scalar )
    Matrix4x4i o1( cv_arithmetic1 );
    Matrix4x4i o2( cv_values );

    o2 = o1 / cv_scalar;

    return allEqual( o2.data, cv_divScalarResult );
UT_TEST_END()

UT_TEST_BEGIN( Matrix4x4_transpose_free )
    Matrix4x4i o1( cv_values );
    Matrix4x4i o2( cv_values );

    o2 = transpose( o1 );

    return allEqual( o2.data, cv_transpose );
UT_TEST_END()

UT_TEST_BEGIN( Matrix4x4_invert_free )
    Matrix4x4f o1( cv_invertF );

    Matrix4x4f o2 = Brimstone::invert( o1 );

    return allEqual( o2.data, cv_inverseF );
UT_TEST_END()




#ifdef BS_ZERO

UT_TEST_BEGIN( Matrix4x4_constructorZero )
    Matrix4x4i o;
    return allEqual( o.data, cv_identity );
UT_TEST_END()

#endif //BS_ZERO




#ifdef BS_CHECK_INDEX

UT_TEST_BEGIN( Matrix4x4_index_OOB )
    Matrix4x4i o;
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

UT_TEST_BEGIN( Matrix4x4_constIndex_OOB )
    const Matrix4x4i o;
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

UT_TEST_BEGIN( Matrix4x4_setRow_OOB )
    Matrix4x4i o;
    Vector4i v;

    try {
        o.setRow( (size_t)-1, v );
        return false;
    } catch( const BoundsException& ) {}

    try {
        o.setRow( 4, v );
        return false;
    } catch( const BoundsException& ) {}

    return true;
UT_TEST_END()

UT_TEST_BEGIN( Matrix4x4_setColumn_OOB )
    Matrix4x4i o;
    Vector4i v;

    try {
        o.setColumn( (size_t)-1, v );
        return false;
    } catch( const BoundsException& ) {}

    try {
        o.setColumn( 4, v );
        return false;
    } catch( const BoundsException& ) {}

    return true;
UT_TEST_END()

UT_TEST_BEGIN( Matrix4x4_getRow_OOB )
    const Matrix4x4i o;
    Vector4i v;

    try {
        v = o.getRow( (size_t)-1 );
        return false;
    } catch( const BoundsException& ) {}

    try {
        v = o.getRow( 4 );
        return false;
    } catch( const BoundsException& ) {}

    return true;
UT_TEST_END()

UT_TEST_BEGIN( Matrix4x4_getColumn_OOB )
    const Matrix4x4i o;
    Vector4i v;

    try {
        v = o.getColumn( (size_t)-1 );
        return false;
    } catch( const BoundsException& ) {}

    try {
        v = o.getColumn( 4 );
        return false;
    } catch( const BoundsException& ) {}

    return true;
UT_TEST_END()

UT_TEST_BEGIN( Matrix4x4_elem_OOB )
    Matrix4x4i o;
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

UT_TEST_BEGIN( Matrix4x4_elemConst_OOB )
    const Matrix4x4i o;
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