/*
test/MatrixRxC.cpp
------------------
Copyright (c) 2024, theJ89

Description:
    Unit tests for Matrix< T, R, C > (generic case; i.e. rectangular matrices)
*/




//Includes
#include "../Test.hpp"              //UT_TEST_BEGIN, UT_TEST_END
#include "../utils.hpp"             //UnitTest::allEqual, UnitTest::allEqualTo

#include <brimstone/Matrix.hpp>     //Brimstone::Matrix, Brimstone::Matrix4x4i
#include <brimstone/Vector.hpp>     //Brimstone::Vector, Brimstone::Vector4i
#include <brimstone/Exception.hpp>  //Brimstone::BoundsException

#include <cstddef>                  //std::size_t
#include <sstream>                  //std::ostringstream




namespace {




//Types
using Matrix5x4i = ::Brimstone::Matrix< int, 5, 4 >;
using Matrix5x4f = ::Brimstone::Matrix< float, 5, 4 >;
using Matrix4x5i = ::Brimstone::Matrix< int, 4, 5 >;
using              ::Brimstone::Matrix4x4i;
using              ::Brimstone::Vector4i;
using Vector5i   = ::Brimstone::Vector< int, 5 >;
using              ::Brimstone::BoundsException;




//Constants
const std::size_t cv_rows = 5;
const std::size_t cv_cols = 4;
const std::size_t cv_size = cv_rows * cv_cols;
const int         cv_zero[20] {
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0
};
const int         cv_identity[20] {
    1, 0, 0, 0,
    0, 1, 0, 0,
    0, 0, 1, 0,
    0, 0, 0, 1,
    0, 0, 0, 0
};
const int         cv_values[20] {
      1,    2,     3,    4,
      5,    6,     7,    8,
      9,   10,    11,   12,
     13,   14,    15,   16,
     17,   18,    19,   20
};
const int         cv_valuesAlt[20] {
     26,   27,    28,   29,
     30,   31,    32,   33,
     34,   35,    36,   37,
     38,   39,    40,   41,
     42,   43,    44,   45
};
const int         cv_values4x4[16] {
      1,    2,     3,    4,
      5,    6,     7,    8,
      9,   10,    11,   12,
     13,   14,    15,   16
};
const int         cv_valuesVector1[5]    = { 26, 27, 28, 29, 30 };
const int         cv_valuesVector2[4]    = { 26, 27, 28, 29 };
const int         cv_mulVectorResult1[4] = { 1300, 1440, 1580, 1720 };
const int         cv_mulVectorResult2[5] = { 280, 720, 1160, 1600, 2040 };
const int         cv_arithmetic1[20] {
      2,    8,    16,   32,
      9,   13,    21,   25,
    -15,  -10,     0,   10,
     14,   52,   100,   50,
     39,   66,    82,   18
};
const int         cv_arithmetic2[20] {
      1,    2,    -2,    4,
      9,  -13,     7,    5,
     -5,    3,   100,  -10,
      7,   16,    50,   10,
     13,   33,   -41,    9
};
const int         cv_addResult[20] {
      3,   10,    14,   36,
     18,    0,    28,   30,
    -20,   -7,   100,    0,
     21,   68,   150,   60,
     52,   99,    41,   27
};
const int         cv_subResult[20] {
      1,    6,    18,   28,
      0,   26,    14,   20,
    -10,  -13,  -100,   20,
      7,   36,    50,   40,
     26,   33,   123,    9
};
const int         cv_mulResult[20] {
     90,  100,   110,  120,
    202,  228,   254,  280,
    314,  356,   398,  440,
    426,  484,   542,  600,
    538,  612,   686,  760
};
const int         cv_scalar = 2;
const int         cv_addScalarResult[20] {
      4,   10,    18,   34,
     11,   15,    23,   27,
    -13,   -8,     2,   12,
     16,   54,   102,   52,
     41,   68,    84,   20
};
const int         cv_subScalarResult[20] {
      0,    6,    14,   30,
      7,   11,    19,   23,
    -17,  -12,    -2,    8,
     12,   50,    98,   48,
     37,   64,    80,   16
};
const int         cv_mulScalarResult[20] {
      4,   16,    32,   64,
     18,   26,    42,   50,
    -30,  -20,     0,   20,
     28,  104,   200,  100,
     78,  132,   164,   36
};
const int         cv_divScalarResult[20] {
      1,    4,     8,   16,
      4,    6,    10,   12,
     -7,   -5,     0,    5,
      7,   26,    50,   25,
     19,   33,    41,    9
};
const int         cv_negateResult[20] {
     -2,   -8,   -16,  -32,
     -9,  -13,   -21,  -25,
     15,   10,     0,  -10,
    -14,  -52,  -100,  -50,
    -39,  -66,   -82,  -18
};
const int         cv_transpose[20] {
      1,    5,     9,   13,  17,
      2,    6,    10,   14,  18,
      3,    7,    11,   15,  19,
      4,    8,    12,   16,  20
};

const char*       cv_output =
    " 1  2  3  4\n"
    " 5  6  7  8\n"
    " 9 10 11 12\n"
    "13 14 15 16\n"
    "17 18 19 20\n";

const float       cv_valuesF[20] {
     1.0f,  2.0f,   3.0f,  4.0f,
     5.0f,  6.0f,   7.0f,  8.0f,
     9.0f, 10.0f,  11.0f, 12.0f,
    13.0f, 14.0f,  15.0f, 16.0f,
    17.0f, 18.0f,  19.0f, 20.0f
};
const float       cv_valuesAltF[20] {
    26.0f, 27.0f, 28.0f, 29.0f,
    30.0f, 31.0f, 32.0f, 33.0f,
    34.0f, 35.0f, 36.0f, 37.0f,
    38.0f, 39.0f, 40.0f, 41.0f,
    42.0f, 43.0f, 44.0f, 45.0f
};
const char*       cv_outputF =
    " 1.00000  2.00000  3.00000  4.00000\n"
    " 5.00000  6.00000  7.00000  8.00000\n"
    " 9.00000 10.00000 11.00000 12.00000\n"
    "13.00000 14.00000 15.00000 16.00000\n"
    "17.00000 18.00000 19.00000 20.00000\n";




} //namespace




namespace UnitTest {




UT_TEST_BEGIN( MatrixRxC_constructorFill )
    Matrix5x4i o( 1 );

    return allEqualTo( o.data, 1 );
UT_TEST_END()

UT_TEST_BEGIN( MatrixRxC_constructorCppRange )
    Matrix5x4i o( cv_values );

    return allEqual( o.data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( MatrixRxC_constructorInitializerList )
    Matrix5x4i o( {
        cv_values[0],  cv_values[1],  cv_values[2],  cv_values[3],
        cv_values[4],  cv_values[5],  cv_values[6],  cv_values[7],
        cv_values[8],  cv_values[9],  cv_values[10], cv_values[11],
        cv_values[12], cv_values[13], cv_values[14], cv_values[15],
        cv_values[16], cv_values[17], cv_values[18], cv_values[19]
    } );

    return allEqual( o.data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( MatrixRxC_setCppRange )
    Matrix5x4i o( cv_values );

    o.set( cv_valuesAlt );

    return allEqual( o.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( MatrixRxC_setInitializerList )
    Matrix5x4i o( cv_values );

    o.set( {
        cv_valuesAlt[0],  cv_valuesAlt[1],  cv_valuesAlt[2],  cv_valuesAlt[3],
        cv_valuesAlt[4],  cv_valuesAlt[5],  cv_valuesAlt[6],  cv_valuesAlt[7],
        cv_valuesAlt[8],  cv_valuesAlt[9],  cv_valuesAlt[10], cv_valuesAlt[11],
        cv_valuesAlt[12], cv_valuesAlt[13], cv_valuesAlt[14], cv_valuesAlt[15],
        cv_valuesAlt[16], cv_valuesAlt[17], cv_valuesAlt[18], cv_valuesAlt[19]
    } );

    return allEqual( o.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( MatrixRxC_getCppRange )
    int values[ cv_size ];
    copyAll( cv_values, values );
    Matrix5x4i o( cv_valuesAlt );

    o.get( values );

    return allEqual( o.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( MatrixRxC_fill )
    Matrix5x4i o( cv_values );

    o.fill( 1 );

    return allEqualTo( o.data, 1 );
UT_TEST_END()

UT_TEST_BEGIN( MatrixRxC_begin )
    Matrix5x4i o( cv_values );

    return o.begin() == std::begin( o.data );
UT_TEST_END()

UT_TEST_BEGIN( MatrixRxC_beginConst )
    const Matrix5x4i o( cv_values );

    return o.begin() == std::begin( o.data );
UT_TEST_END()

UT_TEST_BEGIN( MatrixRxC_cbegin )
    Matrix5x4i o( cv_values );

    return o.cbegin() == std::begin( o.data );
UT_TEST_END()

UT_TEST_BEGIN( MatrixRxC_end )
    Matrix5x4i o( cv_values );

    return o.end() == std::end( o.data );
UT_TEST_END()

UT_TEST_BEGIN( MatrixRxC_endConst )
    const Matrix5x4i o( cv_values );

    return o.end() == std::end( o.data );
UT_TEST_END()

UT_TEST_BEGIN( MatrixRxC_cend )
    Matrix5x4i o( cv_values );

    return o.cend() == std::end( o.data );
UT_TEST_END()

UT_TEST_BEGIN( MatrixRxC_rangedFor )
    Matrix5x4i o( cv_values );

    int idx = 0;
    for( int& i : o )
        i = cv_valuesAlt[idx++];

    return allEqual( o.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( MatrixRxC_rangedForConst )
    int data[cv_size];
    copyAll( cv_values, data );
    const Matrix5x4i o( cv_valuesAlt );

    int idx = 0;
    for( int i : o )
        data[idx++] = i;

    return allEqual( data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( MatrixRxC_index )
    Matrix5x4i o( cv_values );

    for( std::size_t i = 0; i < cv_size; ++i )
        o[i] = cv_valuesAlt[i];

    return allEqual( o.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( MatrixRxC_indexConst )
    int data[cv_size];
    const Matrix5x4i o( cv_values );

    for( std::size_t i = 0; i < cv_size; ++i )
        data[i] = o[i];

    return allEqual( data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( MatrixRxC_constructorCopy )
    Matrix5x4f o1( cv_valuesAltF );

    Matrix5x4i o2( o1 );

    return allEqual( o2.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( MatrixRxC_isSquare )
    Matrix5x4i o( cv_values );

    return o.isSquare() == false;
UT_TEST_END()

UT_TEST_BEGIN( MatrixRxC_zero )
    Matrix5x4i o( cv_values );

    o.zero();

    return allEqual( o.data, cv_zero );
UT_TEST_END()

UT_TEST_BEGIN( MatrixRxC_isZero )
    Matrix5x4i o1( cv_zero );
    Matrix5x4i o2( cv_values );

    return o1.isZero() == true  &&
           o2.isZero() == false;
UT_TEST_END()

UT_TEST_BEGIN( MatrixRxC_setRow )
    Matrix5x4i o( cv_valuesAlt );

    o.setRow( 0, Vector4i( cv_values[0],  cv_values[1],  cv_values[2],  cv_values[3]  ) );
    o.setRow( 1, Vector4i( cv_values[4],  cv_values[5],  cv_values[6],  cv_values[7]  ) );
    o.setRow( 2, Vector4i( cv_values[8],  cv_values[9],  cv_values[10], cv_values[11] ) );
    o.setRow( 3, Vector4i( cv_values[12], cv_values[13], cv_values[14], cv_values[15] ) );
    o.setRow( 4, Vector4i( cv_values[16], cv_values[17], cv_values[18], cv_values[19] ) );

    return allEqual( o.data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( MatrixRxC_setColumn )
    Matrix5x4i o( cv_valuesAlt );

    o.setColumn( 0, Vector5i( { cv_values[0],  cv_values[4], cv_values[8],  cv_values[12], cv_values[16] } ) );
    o.setColumn( 1, Vector5i( { cv_values[1],  cv_values[5], cv_values[9],  cv_values[13], cv_values[17] } ) );
    o.setColumn( 2, Vector5i( { cv_values[2],  cv_values[6], cv_values[10], cv_values[14], cv_values[18] } ) );
    o.setColumn( 3, Vector5i( { cv_values[3],  cv_values[7], cv_values[11], cv_values[15], cv_values[19] } ) );

    return allEqual( o.data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( MatrixRxC_getRow )
    Matrix5x4i o( cv_values );

    return o.getRow( 0 ) == Vector4i( cv_values[0],  cv_values[1],  cv_values[2],  cv_values[3]  ) &&
           o.getRow( 1 ) == Vector4i( cv_values[4],  cv_values[5],  cv_values[6],  cv_values[7]  ) &&
           o.getRow( 2 ) == Vector4i( cv_values[8],  cv_values[9],  cv_values[10], cv_values[11] ) &&
           o.getRow( 3 ) == Vector4i( cv_values[12], cv_values[13], cv_values[14], cv_values[15] ) &&
           o.getRow( 4 ) == Vector4i( cv_values[16], cv_values[17], cv_values[18], cv_values[19] );
UT_TEST_END()

UT_TEST_BEGIN( MatrixRxC_getColumn )
    Matrix5x4i o( cv_values );

    return o.getColumn( 0 ) == Vector5i( { cv_values[0],  cv_values[4], cv_values[8],  cv_values[12], cv_values[16] } ) &&
           o.getColumn( 1 ) == Vector5i( { cv_values[1],  cv_values[5], cv_values[9],  cv_values[13], cv_values[17] } ) &&
           o.getColumn( 2 ) == Vector5i( { cv_values[2],  cv_values[6], cv_values[10], cv_values[14], cv_values[18] } ) &&
           o.getColumn( 3 ) == Vector5i( { cv_values[3],  cv_values[7], cv_values[11], cv_values[15], cv_values[19] } );
UT_TEST_END()

UT_TEST_BEGIN( MatrixRxC_getRows )
    Matrix5x4i o( cv_values );

    return o.getRows() == cv_rows;
UT_TEST_END()

UT_TEST_BEGIN( MatrixRxC_getColumns )
    Matrix5x4i o( cv_values );

    return o.getColumns() == cv_cols;
UT_TEST_END()

UT_TEST_BEGIN( MatrixRxC_identity )
    Matrix5x4i o( cv_values );

    o.identity();

    return allEqual( o.data, cv_identity );
UT_TEST_END()

UT_TEST_BEGIN( MatrixRxC_isIdentity )
    Matrix5x4i o1( cv_identity );
    Matrix5x4i o2( cv_values );

    return o1.isIdentity() == true &&
           o2.isIdentity() == false;
UT_TEST_END()

UT_TEST_BEGIN( MatrixRxC_assignCopy )
    Matrix5x4f o1( cv_valuesAltF );
    Matrix5x4i o2( cv_values );

    o2 = o1;

    return allEqual( o2.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( MatrixRxC_equals )
    Matrix5x4i o1( cv_values );
    Matrix5x4i o2( cv_values );
    Matrix5x4i o3( cv_valuesAlt );

    return ( o1 == o2 ) == true   &&
           ( o1 == o3 ) == false;
UT_TEST_END()

UT_TEST_BEGIN( MatrixRxC_notEquals )
    Matrix5x4i o1( cv_values );
    Matrix5x4i o2( cv_values );
    Matrix5x4i o3( cv_valuesAlt );

    return ( o1 != o2 ) == false  &&
           ( o1 != o3 ) == true;
UT_TEST_END()

UT_TEST_BEGIN( MatrixRxC_output_int )
    Matrix5x4i o( cv_values );

    std::ostringstream sout;
    sout << o;

    return sout.str() == cv_output;
UT_TEST_END()

UT_TEST_BEGIN( MatrixRxC_output_float )
    Matrix5x4f o( cv_valuesF );

    std::ostringstream sout;
    sout << o;

    return sout.str() == cv_outputF;
UT_TEST_END()

UT_TEST_BEGIN( MatrixRxC_addAssign_matrix )
    Matrix5x4i o1( cv_arithmetic1 );
    Matrix5x4i o2( cv_arithmetic2 );

    o1 += o2;

    return allEqual( o1.data, cv_addResult );
UT_TEST_END()

UT_TEST_BEGIN( MatrixRxC_subAssign_matrix )
    Matrix5x4i o1( cv_arithmetic1 );
    Matrix5x4i o2( cv_arithmetic2 );

    o1 -= o2;

    return allEqual( o1.data, cv_subResult );
UT_TEST_END()

UT_TEST_BEGIN( MatrixRxC_mulAssign_matrix )
    Matrix5x4i o1( cv_values );
    Matrix4x4i o2( cv_values4x4 );

    o1 *= o2;

    return allEqual( o1.data, cv_mulResult );
UT_TEST_END()

UT_TEST_BEGIN( MatrixRxC_addAssign_scalar )
    Matrix5x4i o( cv_arithmetic1 );

    o += cv_scalar;

    return allEqual( o.data, cv_addScalarResult );
UT_TEST_END()

UT_TEST_BEGIN( MatrixRxC_subAssign_scalar )
    Matrix5x4i o( cv_arithmetic1 );

    o -= cv_scalar;

    return allEqual( o.data, cv_subScalarResult );
UT_TEST_END()

UT_TEST_BEGIN( MatrixRxC_mulAssign_scalar )
    Matrix5x4i o( cv_arithmetic1 );

    o *= cv_scalar;

    return allEqual( o.data, cv_mulScalarResult );
UT_TEST_END()

UT_TEST_BEGIN( MatrixRxC_divAssign_scalar )
    Matrix5x4i o( cv_arithmetic1 );

    o /= cv_scalar;

    return allEqual( o.data, cv_divScalarResult );
UT_TEST_END()

UT_TEST_BEGIN( MatrixRxC_elem )
    Matrix5x4i o( cv_values );

    for( std::size_t r = 0; r < cv_rows; ++r )
        for( std::size_t c = 0; c < cv_cols; ++c )
            o( r, c ) = cv_valuesAlt[ r * cv_cols + c ];

    return allEqual( o.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( MatrixRxC_elemConst )
    const Matrix5x4i o( cv_values );

    for( std::size_t r = 0; r < cv_rows; ++r )
        for( std::size_t c = 0; c < cv_cols; ++c )
            if( o( r, c ) != cv_values[ r * cv_cols + c ] )
                return false;

    return true;
UT_TEST_END()

UT_TEST_BEGIN( MatrixRxC_unaryPlus )
    Matrix5x4i o1( cv_arithmetic1 );
    Matrix5x4i o2( cv_arithmetic2 );

    o2 = +o1;

    return allEqual( o2.data, cv_arithmetic1 );
UT_TEST_END()

UT_TEST_BEGIN( MatrixRxC_unaryMinus )
    Matrix5x4i o1( cv_arithmetic1 );
    Matrix5x4i o2( cv_arithmetic2 );

    o2 = -o1;

    return allEqual( o2.data, cv_negateResult );
UT_TEST_END()

UT_TEST_BEGIN( MatrixRxC_add_matrix )
    Matrix5x4i o1( cv_arithmetic1 );
    Matrix5x4i o2( cv_arithmetic2 );
    Matrix5x4i o3( cv_values );

    o3 = o1 + o2;

    return allEqual( o3.data, cv_addResult );
UT_TEST_END()

UT_TEST_BEGIN( MatrixRxC_sub_matrix )
    Matrix5x4i o1( cv_arithmetic1 );
    Matrix5x4i o2( cv_arithmetic2 );
    Matrix5x4i o3( cv_values );

    o3 = o1 - o2;

    return allEqual( o3.data, cv_subResult );
UT_TEST_END()

UT_TEST_BEGIN( MatrixRxC_mul_matrix )
    Matrix5x4i o1( cv_values );
    Matrix4x4i o2( cv_values4x4 );
    Matrix5x4i o3( cv_values );

    o3 = o1 * o2;

    return allEqual( o3.data, cv_mulResult );
UT_TEST_END()

UT_TEST_BEGIN( MatrixRxC_mul_vector_left )
    Matrix5x4i o( cv_values );
    Vector5i   v1( cv_valuesVector1 );

    Vector4i   v2 = v1 * o;

    return allEqual( v2.data, cv_mulVectorResult1 );
UT_TEST_END()

UT_TEST_BEGIN( MatrixRxC_mul_vector_right )
    Matrix5x4i o( cv_values );
    Vector4i   v1( cv_valuesVector2 );

    Vector5i   v2 = o * v1;

    return allEqual( v2.data, cv_mulVectorResult2 );
UT_TEST_END()

UT_TEST_BEGIN( MatrixRxC_add_scalar_left )
    Matrix5x4i o1( cv_arithmetic1 );
    Matrix5x4i o2( cv_values );

    o2 = cv_scalar + o1;

    return allEqual( o2.data, cv_addScalarResult );
UT_TEST_END()

UT_TEST_BEGIN( MatrixRxC_add_scalar_right )
    Matrix5x4i o1( cv_arithmetic1 );
    Matrix5x4i o2( cv_values );

    o2 = o1 + cv_scalar;

    return allEqual( o2.data, cv_addScalarResult );
UT_TEST_END()

UT_TEST_BEGIN( MatrixRxC_sub_scalar )
    Matrix5x4i o1( cv_arithmetic1 );
    Matrix5x4i o2( cv_values );

    o2 = o1 - cv_scalar;

    return allEqual( o2.data, cv_subScalarResult );
UT_TEST_END()

UT_TEST_BEGIN( MatrixRxC_mul_scalar_left )
    Matrix5x4i o1( cv_arithmetic1 );
    Matrix5x4i o2( cv_values );

    o2 = cv_scalar * o1;

    return allEqual( o2.data, cv_mulScalarResult );
UT_TEST_END()

UT_TEST_BEGIN( MatrixRxC_mul_scalar_right )
    Matrix5x4i o1( cv_arithmetic1 );
    Matrix5x4i o2( cv_values );

    o2 = o1 * cv_scalar;

    return allEqual( o2.data, cv_mulScalarResult );
UT_TEST_END()

UT_TEST_BEGIN( MatrixRxC_div_scalar )
    Matrix5x4i o1( cv_arithmetic1 );
    Matrix5x4i o2( cv_values );

    o2 = o1 / cv_scalar;

    return allEqual( o2.data, cv_divScalarResult );
UT_TEST_END()

UT_TEST_BEGIN( MatrixRxC_transpose_free )
    Matrix5x4i o1( cv_values );
    Matrix4x5i o2( cv_values );

    o2 = transpose( o1 );

    return allEqual( o2.data, cv_transpose );
UT_TEST_END()




#ifdef BS_ZERO

UT_TEST_BEGIN( MatrixRxC_constructorZero )
    Matrix5x4i o;
    return allEqual( o.data, cv_identity );
UT_TEST_END()

#endif //BS_ZERO




#ifdef BS_CHECK_INDEX

UT_TEST_BEGIN( MatrixRxC_index_OOB )
    Matrix5x4i o;

    try {
        o[(std::size_t)-1];
        return false;
    } catch( const BoundsException& ) {}

    try {
        o[25];
        return false;
    } catch( const BoundsException& ) {}

    return true;
UT_TEST_END()

UT_TEST_BEGIN( MatrixRxC_constIndex_OOB )
    const Matrix5x4i o;

    try {
        o[(std::size_t)-1];
        return false;
    } catch( const BoundsException& ) {}

    try {
        o[25];
        return false;
    } catch( const BoundsException& ) {}

    return true;
UT_TEST_END()

UT_TEST_BEGIN( MatrixRxC_setRow_OOB )
    Matrix5x4i o;
    Vector4i v;

    try {
        o.setRow( (std::size_t)-1, v );
        return false;
    } catch( const BoundsException& ) {}

    try {
        o.setRow( 5, v );
        return false;
    } catch( const BoundsException& ) {}

    return true;
UT_TEST_END()

UT_TEST_BEGIN( MatrixRxC_setColumn_OOB )
    Matrix5x4i o;
    Vector5i v;

    try {
        o.setColumn( (std::size_t)-1, v );
        return false;
    } catch( const BoundsException& ) {}

    try {
        o.setColumn( 4, v );
        return false;
    } catch( const BoundsException& ) {}

    return true;
UT_TEST_END()

UT_TEST_BEGIN( MatrixRxC_getRow_OOB )
    const Matrix5x4i o;

    try {
        o.getRow( (std::size_t)-1 );
        return false;
    } catch( const BoundsException& ) {}

    try {
        o.getRow( 5 );
        return false;
    } catch( const BoundsException& ) {}

    return true;
UT_TEST_END()

UT_TEST_BEGIN( MatrixRxC_getColumn_OOB )
    const Matrix5x4i o;

    try {
        o.getColumn( (std::size_t)-1 );
        return false;
    } catch( const BoundsException& ) {}

    try {
        o.getColumn( 5 );
        return false;
    } catch( const BoundsException& ) {}

    return true;
UT_TEST_END()

UT_TEST_BEGIN( MatrixRxC_elem_OOB )
    Matrix5x4i o;

    try {
        o( (std::size_t)-1, 0 );
        return false;
    } catch( const BoundsException& ) {}

    try {
        o( 0, (std::size_t)-1 );
        return false;
    } catch( const BoundsException& ) {}

    try {
        o( 5, 0 );
        return false;
    } catch( const BoundsException& ) {}

    try {
        o( 0, 4 );
        return false;
    } catch( const BoundsException& ) {}

    return true;
UT_TEST_END()

UT_TEST_BEGIN( MatrixRxC_elemConst_OOB )
    const Matrix5x4i o;

    try {
        o( (std::size_t)-1, 0 );
        return false;
    } catch( const BoundsException& ) {}

    try {
        o( 0, (std::size_t)-1 );
        return false;
    } catch( const BoundsException& ) {}

    try {
        o( 5, 0 );
        return false;
    } catch( const BoundsException& ) {}

    try {
        o( 0, 4 );
        return false;
    } catch( const BoundsException& ) {}

    return true;
UT_TEST_END()

#endif //BS_CHECK_INDEX




} //namespace UnitTest
