/*
test/MatrixNxN.cpp
-----------------------
Copyright (c) 2014, theJ89

Description:
    Unit tests for Matrix<T,N,N> specialization (square matrices)
*/




//Includes
#include "../Test.hpp"
#include "../utils.hpp"         //allEqual, allEqualTo

#include <brimstone/Matrix.hpp>




namespace {
    typedef ::Brimstone::Matrix< int, 5, 5 >   Matrix5x5i;
    typedef ::Brimstone::Matrix< float, 5, 5 > Matrix5x5f;
    typedef ::Brimstone::Vector< int, 5 >      Vector5i;
    using   ::Brimstone::BoundsException;

    const size_t cv_rows             = 5;
    const size_t cv_cols             = 5;
    const size_t cv_size             = cv_rows * cv_cols;
    const int    cv_zero[25] {
        0, 0, 0, 0, 0,
        0, 0, 0, 0, 0,
        0, 0, 0, 0, 0,
        0, 0, 0, 0, 0,
        0, 0, 0, 0, 0
    };
    const int    cv_identity[25] {
        1, 0, 0, 0, 0,
        0, 1, 0, 0, 0,
        0, 0, 1, 0, 0,
        0, 0, 0, 1, 0,
        0, 0, 0, 0, 1
    };
    const int    cv_values[25] {
          1,    2,     3,    4,    5,
          6,    7,     8,    9,   10,
         11,   12,    13,   14,   15,
         16,   17,    18,   19,   20,
         21,   22,    23,   24,   25
    };
    const int    cv_valuesAlt[25] {
         26,   27,    28,   29,   30,
         31,   32,    33,   34,   35,
         36,   37,    38,   39,   40,
         41,   42,    43,   44,   45,
         46,   47,    48,   49,   50
    };
    const int    cv_valuesDiagonal[25] {
         26,    2,     3,    4,    5,
          6,   32,     8,    9,   10,
         11,   12,    38,   14,   15,
         16,   17,    18,   44,   20,
         21,   22,    23,   24,   50
    };
    const int   cv_diagonal[5] { 26, 32, 38, 44, 50 };
    const int   cv_trace               = 65;
    const int   cv_valuesVector[5]     = { 26, 27, 28, 29, 30 };
    const int   cv_mulVectorResult1[5] = { 1590, 1730, 1870, 2010, 2150 };
    const int   cv_mulVectorResult2[5] = {  430, 1130, 1830, 2530, 3230 };
    const int   cv_lowerTriangular[25] {
          1,    0,     0,    0,    0,
          6,    7,     0,    0,    0,
         11,   12,    13,    0,    0,
         16,   17,    18,   19,    0,
         21,   22,    23,   24,   25
    };
    const int    cv_upperTriangular[25] {
          1,    2,     3,    4,    5,
          0,    7,     8,    9,   10,
          0,    0,    13,   14,   15,
          0,    0,     0,   19,   20,
          0,    0,     0,    0,   25
    };
    const int    cv_arithmetic1[25] {
          2,    8,    16,   32,   64,
          9,   13,    21,   25,   39,
        -15,  -10,     0,   10,   15,
         14,   52,   100,   50,   75,
         39,   66,    82,   18,   12
    };
    const int    cv_arithmetic2[25] {
          1,    2,    -2,    4,   16,
          9,  -13,     7,    5,   13,
         -5,    3,   100,  -10,    3,
          7,   16,    50,   10,   25,
         13,   33,   -41,    9,    3
    };
    const int    cv_addResult[25] {
          3,   10,    14,   36,   80,
         18,    0,    28,   30,   52,
        -20,   -7,   100,    0,   18,
         21,   68,   150,   60,  100,
         52,   99,    41,   27,   15
    };
    const int    cv_subResult[25] {
          1,    6,    18,   28,   48,
          0,   26,    14,   20,   26,
        -10,  -13,  -100,   20,   12,
          7,   36,    50,   40,   50,
         26,   33,   123,    9,    9
    };
    const int    cv_mulResult[25] {
        1050, 2572,   628,  784, 1176,
         703, 1599,  1824,  492, 1118,
         160,  755,  -155,  125,  -75,
        1307, 2927,  9761,  491, 2675,
         505,  150,  8992,  -46, 2214
    };
    const int    cv_scalar = 2;
    const int    cv_addScalarResult[25] {
          4,   10,    18,   34,   66,
         11,   15,    23,   27,   41,
        -13,   -8,     2,   12,   17,
         16,   54,   102,   52,   77,
         41,   68,    84,   20,   14
    };
    const int    cv_subScalarResult[25] {
          0,    6,    14,   30,   62,
          7,   11,    19,   23,   37,
        -17,  -12,    -2,    8,   13,
         12,   50,    98,   48,   73,
         37,   64,    80,   16,   10
    };
    const int    cv_mulScalarResult[25] {
          4,   16,    32,   64,  128,
         18,   26,    42,   50,   78,
        -30,  -20,     0,   20,   30,
         28,  104,   200,  100,  150,
         78,  132,   164,   36,   24
    };
    const int    cv_divScalarResult[25] {
          1,    4,     8,   16,   32,
          4,    6,    10,   12,   19,
         -7,   -5,     0,    5,    7,
          7,   26,    50,   25,   37,
         19,   33,    41,    9,    6
    };
    const int    cv_negateResult[25] {
         -2,   -8,   -16,  -32,  -64,
         -9,  -13,   -21,  -25,  -39,
         15,   10,     0,  -10,  -15,
        -14,  -52,  -100,  -50,  -75,
        -39,  -66,   -82,  -18,  -12
    };
    const int    cv_transpose[25] {
          1,    6,    11,   16,   21,
          2,    7,    12,   17,   22,
          3,    8,    13,   18,   23,
          4,    9,    14,   19,   24,
          5,   10,    15,   20,   25
    };

    const char*  cv_output =
        " 1  2  3  4  5\n"
        " 6  7  8  9 10\n"
        "11 12 13 14 15\n"
        "16 17 18 19 20\n"
        "21 22 23 24 25\n";

    const float  cv_valuesF[25] {
         1.0f,  2.0f,  3.0f,  4.0f,  5.0f,
         6.0f,  7.0f,  8.0f,  9.0f, 10.0f,
        11.0f, 12.0f, 13.0f, 14.0f, 15.0f,
        16.0f, 17.0f, 18.0f, 19.0f, 20.0f,
        21.0f, 22.0f, 23.0f, 24.0f, 25.0f
    };
    const float  cv_valuesAltF[25] {
        26.0f, 27.0f, 28.0f, 29.0f, 30.0f,
        31.0f, 32.0f, 33.0f, 34.0f, 35.0f,
        36.0f, 37.0f, 38.0f, 39.0f, 40.0f,
        41.0f, 42.0f, 43.0f, 44.0f, 45.0f,
        46.0f, 47.0f, 48.0f, 49.0f, 50.0f
    };
    const float  cv_invertF[25] {
         -1.0f,   2.0f,  13.0f,  -4.0f,  -5.0f,
         -6.0f,  17.0f,   8.0f,  -9.0f,  20.0f,
         11.0f, -22.0f,  -3.0f, -14.0f, -15.0f,
         16.0f,  -7.0f, -18.0f,  19.0f,  10.0f,
         21.0f, -12.0f, -23.0f, -24.0f,  25.0f
    };
    const float  cv_inverseF[25] {
         1628.0f / 4820.0f, -1474.0f / 4820.0f, -1342.0f / 4820.0f, -306.0f / 4820.0f,  822.0f / 4820.0f,
         1920.0f / 4820.0f, -2279.0f / 4820.0f, -2208.0f / 4820.0f, -789.0f / 4820.0f, 1198.0f / 4820.0f,
         -374.0f / 4820.0f,  1003.0f / 4820.0f,   888.0f / 4820.0f,  409.0f / 4820.0f, -508.0f / 4820.0f,
         -400.0f / 4820.0f,   525.0f / 4820.0f,   460.0f / 4820.0f,  315.0f / 4820.0f, -350.0f / 4820.0f,
        -1174.0f / 4820.0f,  1571.0f / 4820.0f,  1326.0f / 4820.0f,  557.0f / 4820.0f, -726.0f / 4820.0f
    };

    const float  cv_determinantF = 482000.0f;
    const char*  cv_outputF =
        " 1.00000  2.00000  3.00000  4.00000  5.00000\n"
        " 6.00000  7.00000  8.00000  9.00000 10.00000\n"
        "11.00000 12.00000 13.00000 14.00000 15.00000\n"
        "16.00000 17.00000 18.00000 19.00000 20.00000\n"
        "21.00000 22.00000 23.00000 24.00000 25.00000\n";
}

namespace UnitTest {

UT_TEST_BEGIN( MatrixNxN_constructorFill )
    Matrix5x5i o( 1 );

    return allEqualTo( o.data, 1 );
UT_TEST_END()

UT_TEST_BEGIN( MatrixNxN_constructorCppRange )
    Matrix5x5i o( cv_values );

    return allEqual( o.data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( MatrixNxN_constructorInitializerList )
    Matrix5x5i o( {
        cv_values[0],  cv_values[1],  cv_values[2],  cv_values[3],  cv_values[4],
        cv_values[5],  cv_values[6],  cv_values[7],  cv_values[8],  cv_values[9],
        cv_values[10], cv_values[11], cv_values[12], cv_values[13], cv_values[14],
        cv_values[15], cv_values[16], cv_values[17], cv_values[18], cv_values[19],
        cv_values[20], cv_values[21], cv_values[22], cv_values[23], cv_values[24]
    } );

    return allEqual( o.data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( MatrixNxN_setCppRange )
    Matrix5x5i o( cv_values );

    o.set( cv_valuesAlt );

    return allEqual( o.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( MatrixNxN_setInitializerList )
    Matrix5x5i o( cv_values );

    o.set( {
        cv_valuesAlt[0],  cv_valuesAlt[1],  cv_valuesAlt[2],  cv_valuesAlt[3],  cv_valuesAlt[4],
        cv_valuesAlt[5],  cv_valuesAlt[6],  cv_valuesAlt[7],  cv_valuesAlt[8],  cv_valuesAlt[9],
        cv_valuesAlt[10], cv_valuesAlt[11], cv_valuesAlt[12], cv_valuesAlt[13], cv_valuesAlt[14],
        cv_valuesAlt[15], cv_valuesAlt[16], cv_valuesAlt[17], cv_valuesAlt[18], cv_valuesAlt[19],
        cv_valuesAlt[20], cv_valuesAlt[21], cv_valuesAlt[22], cv_valuesAlt[23], cv_valuesAlt[24]
    } );

    return allEqual( o.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( MatrixNxN_getCppRange )
    int values[ cv_size ];
    copyAll( cv_values, values );
    Matrix5x5i o( cv_valuesAlt );

    o.get( values );

    return allEqual( o.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( MatrixNxN_fill )
    Matrix5x5i o( cv_values );

    o.fill( 1 );

    return allEqualTo( o.data, 1 );
UT_TEST_END()

UT_TEST_BEGIN( MatrixNxN_begin )
    Matrix5x5i o( cv_values );

    return o.begin() == std::begin( o.data );
UT_TEST_END()

UT_TEST_BEGIN( MatrixNxN_beginConst )
    const Matrix5x5i o( cv_values );

    return o.begin() == std::begin( o.data );
UT_TEST_END()

UT_TEST_BEGIN( MatrixNxN_cbegin )
    Matrix5x5i o( cv_values );

    return o.cbegin() == std::begin( o.data );
UT_TEST_END()

UT_TEST_BEGIN( MatrixNxN_end )
    Matrix5x5i o( cv_values );

    return o.end() == std::end( o.data );
UT_TEST_END()

UT_TEST_BEGIN( MatrixNxN_endConst )
    const Matrix5x5i o( cv_values );

    return o.end() == std::end( o.data );
UT_TEST_END()

UT_TEST_BEGIN( MatrixNxN_cend )
    Matrix5x5i o( cv_values );

    return o.cend() == std::end( o.data );
UT_TEST_END()

UT_TEST_BEGIN( MatrixNxN_rangedFor )
    Matrix5x5i o( cv_values );

    int idx = 0;
    for( int& i : o )
        i = cv_valuesAlt[idx++];

    return allEqual( o.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( MatrixNxN_rangedForConst )
    int data[cv_size];
    copyAll( cv_values, data );
    const Matrix5x5i o( cv_valuesAlt );

    int idx = 0;
    for( int i : o )
        data[idx++] = i;

    return allEqual( data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( MatrixNxN_index )
    Matrix5x5i o( cv_values );

    for( size_t i = 0; i < cv_size; ++i )
        o[i] = cv_valuesAlt[i];

    return allEqual( o.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( MatrixNxN_indexConst )
    int data[cv_size];
    const Matrix5x5i o( cv_values );

    for( size_t i = 0; i < cv_size; ++i )
        data[i] = o[i];

    return allEqual( data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( MatrixNxN_constructorCopy )
    Matrix5x5f o1( cv_valuesAltF );

    Matrix5x5i o2( o1 );

    return allEqual( o2.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( MatrixNxN_isSquare )
    Matrix5x5i o( cv_values );

    return o.isSquare() == true;
UT_TEST_END()

UT_TEST_BEGIN( MatrixNxN_zero )
    Matrix5x5i o( cv_values );

    o.zero();

    return allEqual( o.data, cv_zero );
UT_TEST_END()

UT_TEST_BEGIN( MatrixNxN_isZero )
    Matrix5x5i o1( cv_zero );
    Matrix5x5i o2( cv_values );

    return o1.isZero() == true  &&
           o2.isZero() == false;
UT_TEST_END()

UT_TEST_BEGIN( MatrixNxN_setRow )
    Matrix5x5i o( cv_valuesAlt );

    o.setRow( 0, Vector5i( { cv_values[0],  cv_values[1],  cv_values[2],  cv_values[3],  cv_values[4]  } ) );
    o.setRow( 1, Vector5i( { cv_values[5],  cv_values[6],  cv_values[7],  cv_values[8],  cv_values[9]  } ) );
    o.setRow( 2, Vector5i( { cv_values[10], cv_values[11], cv_values[12], cv_values[13], cv_values[14] } ) );
    o.setRow( 3, Vector5i( { cv_values[15], cv_values[16], cv_values[17], cv_values[18], cv_values[19] } ) );
    o.setRow( 4, Vector5i( { cv_values[20], cv_values[21], cv_values[22], cv_values[23], cv_values[24] } ) );

    return allEqual( o.data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( MatrixNxN_setColumn )
    Matrix5x5i o( cv_valuesAlt );

    o.setColumn( 0, Vector5i( { cv_values[0],  cv_values[5], cv_values[10], cv_values[15], cv_values[20] } ) );
    o.setColumn( 1, Vector5i( { cv_values[1],  cv_values[6], cv_values[11], cv_values[16], cv_values[21] } ) );
    o.setColumn( 2, Vector5i( { cv_values[2],  cv_values[7], cv_values[12], cv_values[17], cv_values[22] } ) );
    o.setColumn( 3, Vector5i( { cv_values[3],  cv_values[8], cv_values[13], cv_values[18], cv_values[23] } ) );
    o.setColumn( 4, Vector5i( { cv_values[4],  cv_values[9], cv_values[14], cv_values[19], cv_values[24] } ) );

    return allEqual( o.data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( MatrixNxN_getRow )
    Matrix5x5i o( cv_values );

    return o.getRow( 0 ) == Vector5i( { cv_values[0],  cv_values[1],  cv_values[2],  cv_values[3],  cv_values[4]  } ) &&
           o.getRow( 1 ) == Vector5i( { cv_values[5],  cv_values[6],  cv_values[7],  cv_values[8],  cv_values[9]  } ) &&
           o.getRow( 2 ) == Vector5i( { cv_values[10], cv_values[11], cv_values[12], cv_values[13], cv_values[14] } ) &&
           o.getRow( 3 ) == Vector5i( { cv_values[15], cv_values[16], cv_values[17], cv_values[18], cv_values[19] } ) &&
           o.getRow( 4 ) == Vector5i( { cv_values[20], cv_values[21], cv_values[22], cv_values[23], cv_values[24] } );
UT_TEST_END()

UT_TEST_BEGIN( MatrixNxN_getColumn )
    Matrix5x5i o( cv_values );

    return o.getColumn( 0 ) == Vector5i( { cv_values[0],  cv_values[5], cv_values[10], cv_values[15], cv_values[20] } ) &&
           o.getColumn( 1 ) == Vector5i( { cv_values[1],  cv_values[6], cv_values[11], cv_values[16], cv_values[21] } ) &&
           o.getColumn( 2 ) == Vector5i( { cv_values[2],  cv_values[7], cv_values[12], cv_values[17], cv_values[22] } ) &&
           o.getColumn( 3 ) == Vector5i( { cv_values[3],  cv_values[8], cv_values[13], cv_values[18], cv_values[23] } ) &&
           o.getColumn( 4 ) == Vector5i( { cv_values[4],  cv_values[9], cv_values[14], cv_values[19], cv_values[24] } );
UT_TEST_END()

UT_TEST_BEGIN( MatrixNxN_setDiagonal )
    Matrix5x5i o( cv_values );

    o.setDiagonal( Vector5i( cv_diagonal ) );

    return allEqual( o.data, cv_valuesDiagonal );
UT_TEST_END()

UT_TEST_BEGIN( MatrixNxN_getDiagonal )
    Matrix5x5i o( cv_valuesDiagonal );

    Vector5i v = o.getDiagonal();

    return allEqual( v.data, cv_diagonal );
UT_TEST_END()

UT_TEST_BEGIN( MatrixNxN_isTriangular )
    Matrix5x5i o1( cv_lowerTriangular );
    Matrix5x5i o2( cv_upperTriangular );
    Matrix5x5i o3( cv_values          );

    return o1.isTriangular() == true &&
           o2.isTriangular() == true &&
           o3.isTriangular() == false;
UT_TEST_END()

UT_TEST_BEGIN( MatrixNxN_isLowerTriangular )
    Matrix5x5i o1( cv_lowerTriangular );
    Matrix5x5i o2( cv_values          );

    return o1.isLowerTriangular() == true &&
           o2.isLowerTriangular() == false;
UT_TEST_END()

UT_TEST_BEGIN( MatrixNxN_isUpperTriangular )
    Matrix5x5i o1( cv_upperTriangular );
    Matrix5x5i o2( cv_values          );

    return o1.isUpperTriangular() == true &&
           o2.isUpperTriangular() == false;
UT_TEST_END()

UT_TEST_BEGIN( MatrixNxN_isDiagonal )
    Matrix5x5i o1( cv_identity );
    Matrix5x5i o2( cv_values   );

    return o1.isDiagonal() == true &&
           o2.isDiagonal() == false;
UT_TEST_END()

UT_TEST_BEGIN( MatrixNxN_isInvertible )
    Matrix5x5f o1( cv_invertF );
    Matrix5x5f o2( cv_valuesF );

    return o1.isInvertible() == true &&
           o2.isInvertible() == false;
UT_TEST_END()

UT_TEST_BEGIN( MatrixNxN_getDeterminant )
    Matrix5x5f o( cv_invertF );

    return o.getDeterminant() == cv_determinantF;
UT_TEST_END()

UT_TEST_BEGIN( MatrixNxN_getTrace )
    Matrix5x5i o( cv_values );

    return o.getTrace() == cv_trace;
UT_TEST_END()

UT_TEST_BEGIN( MatrixNxN_transpose )
    Matrix5x5i o( cv_values );

    o.transpose();

    return allEqual( o.data, cv_transpose );
UT_TEST_END()

UT_TEST_BEGIN( MatrixNxN_invert )
    Matrix5x5f o( cv_invertF );

    o.invert();

    return allEqual( o.data, cv_inverseF );
UT_TEST_END()

UT_TEST_BEGIN( MatrixNxN_getRows )
    Matrix5x5i o( cv_values );

    return o.getRows() == cv_rows;
UT_TEST_END()

UT_TEST_BEGIN( MatrixNxN_getColumns )
    Matrix5x5i o( cv_values );

    return o.getColumns() == cv_cols;
UT_TEST_END()

UT_TEST_BEGIN( MatrixNxN_identity )
    Matrix5x5i o( cv_values );

    o.identity();

    return allEqual( o.data, cv_identity );
UT_TEST_END()

UT_TEST_BEGIN( MatrixNxN_isIdentity )
    Matrix5x5i o1( cv_identity );
    Matrix5x5i o2( cv_values );

    return o1.isIdentity() == true &&
           o2.isIdentity() == false;
UT_TEST_END()

UT_TEST_BEGIN( MatrixNxN_assignCopy )
    Matrix5x5f o1( cv_valuesAltF );
    Matrix5x5i o2( cv_values );

    o2 = o1;

    return allEqual( o2.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( MatrixNxN_equals )
    Matrix5x5i o1( cv_values );
    Matrix5x5i o2( cv_values );
    Matrix5x5i o3( cv_valuesAlt );

    return ( o1 == o2 ) == true   &&
           ( o1 == o3 ) == false;
UT_TEST_END()

UT_TEST_BEGIN( MatrixNxN_notEquals )
    Matrix5x5i o1( cv_values );
    Matrix5x5i o2( cv_values );
    Matrix5x5i o3( cv_valuesAlt );

    return ( o1 != o2 ) == false  &&
           ( o1 != o3 ) == true;
UT_TEST_END()

UT_TEST_BEGIN( MatrixNxN_output_int )
    Matrix5x5i o( cv_values );

    std::stringstream sout;
    sout << o;

    return sout.str() == cv_output;
UT_TEST_END()

UT_TEST_BEGIN( MatrixNxN_output_float )
    Matrix5x5f o( cv_valuesF );

    std::stringstream sout;
    sout << o;

    return sout.str() == cv_outputF;
UT_TEST_END()

UT_TEST_BEGIN( MatrixNxN_addAssign_matrix )
    Matrix5x5i o1( cv_arithmetic1 );
    Matrix5x5i o2( cv_arithmetic2 );

    o1 += o2;

    return allEqual( o1.data, cv_addResult );
UT_TEST_END()

UT_TEST_BEGIN( MatrixNxN_subAssign_matrix )
    Matrix5x5i o1( cv_arithmetic1 );
    Matrix5x5i o2( cv_arithmetic2 );

    o1 -= o2;

    return allEqual( o1.data, cv_subResult );
UT_TEST_END()

UT_TEST_BEGIN( MatrixNxN_mulAssign_matrix )
    Matrix5x5i o1( cv_arithmetic1 );
    Matrix5x5i o2( cv_arithmetic2 );

    o1 *= o2;

    return allEqual( o1.data, cv_mulResult );
UT_TEST_END()

UT_TEST_BEGIN( MatrixNxN_mulAssign_vector )
    Matrix5x5i o( cv_values );
    Vector5i   v( cv_valuesVector );

    v *= o;

    return allEqual( v.data, cv_mulVectorResult1 );
UT_TEST_END()

UT_TEST_BEGIN( MatrixNxN_addAssign_scalar )
    Matrix5x5i o( cv_arithmetic1 );

    o += cv_scalar;

    return allEqual( o.data, cv_addScalarResult );
UT_TEST_END()

UT_TEST_BEGIN( MatrixNxN_subAssign_scalar )
    Matrix5x5i o( cv_arithmetic1 );

    o -= cv_scalar;

    return allEqual( o.data, cv_subScalarResult );
UT_TEST_END()

UT_TEST_BEGIN( MatrixNxN_mulAssign_scalar )
    Matrix5x5i o( cv_arithmetic1 );

    o *= cv_scalar;

    return allEqual( o.data, cv_mulScalarResult );
UT_TEST_END()

UT_TEST_BEGIN( MatrixNxN_divAssign_scalar )
    Matrix5x5i o( cv_arithmetic1 );

    o /= cv_scalar;

    return allEqual( o.data, cv_divScalarResult );
UT_TEST_END()

UT_TEST_BEGIN( MatrixNxN_elem )
    Matrix5x5i o( cv_values );

    for( size_t r = 0; r < cv_rows; ++r )
        for( size_t c = 0; c < cv_cols; ++c )
            o( r, c ) = cv_valuesAlt[ r * cv_cols + c ];

    return allEqual( o.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( MatrixNxN_elemConst )
    const Matrix5x5i o( cv_values );

    for( size_t r = 0; r < cv_rows; ++r )
        for( size_t c = 0; c < cv_cols; ++c )
            if( o( r, c ) != cv_values[ r * cv_cols + c ] )
                return false;

    return true;
UT_TEST_END()

UT_TEST_BEGIN( MatrixNxN_unaryPlus )
    Matrix5x5i o1( cv_arithmetic1 );
    Matrix5x5i o2( cv_arithmetic2 );

    o2 = +o1;

    return allEqual( o2.data, cv_arithmetic1 );
UT_TEST_END()

UT_TEST_BEGIN( MatrixNxN_unaryMinus )
    Matrix5x5i o1( cv_arithmetic1 );
    Matrix5x5i o2( cv_arithmetic2 );

    o2 = -o1;

    return allEqual( o2.data, cv_negateResult );
UT_TEST_END()

UT_TEST_BEGIN( MatrixNxN_add_matrix )
    Matrix5x5i o1( cv_arithmetic1 );
    Matrix5x5i o2( cv_arithmetic2 );
    Matrix5x5i o3( cv_values );

    o3 = o1 + o2;

    return allEqual( o3.data, cv_addResult );
UT_TEST_END()

UT_TEST_BEGIN( MatrixNxN_sub_matrix )
    Matrix5x5i o1( cv_arithmetic1 );
    Matrix5x5i o2( cv_arithmetic2 );
    Matrix5x5i o3( cv_values );

    o3 = o1 - o2;

    return allEqual( o3.data, cv_subResult );
UT_TEST_END()

UT_TEST_BEGIN( MatrixNxN_mul_matrix )
    Matrix5x5i o1( cv_arithmetic1 );
    Matrix5x5i o2( cv_arithmetic2 );
    Matrix5x5i o3( cv_values );

    o3 = o1 * o2;

    return allEqual( o3.data, cv_mulResult );
UT_TEST_END()

UT_TEST_BEGIN( MatrixNxN_mul_vector_left )
    Matrix5x5i o( cv_values );
    Vector5i   v1( cv_valuesVector );

    Vector5i   v2 = v1 * o;

    return allEqual( v2.data, cv_mulVectorResult1 );
UT_TEST_END()

UT_TEST_BEGIN( MatrixNxN_mul_vector_right )
    Matrix5x5i o( cv_values );
    Vector5i   v1( cv_valuesVector );

    Vector5i   v2 = o * v1;

    return allEqual( v2.data, cv_mulVectorResult2 );
UT_TEST_END()

UT_TEST_BEGIN( MatrixNxN_add_scalar_left )
    Matrix5x5i o1( cv_arithmetic1 );
    Matrix5x5i o2( cv_values );

    o2 = cv_scalar + o1;

    return allEqual( o2.data, cv_addScalarResult );
UT_TEST_END()

UT_TEST_BEGIN( MatrixNxN_add_scalar_right )
    Matrix5x5i o1( cv_arithmetic1 );
    Matrix5x5i o2( cv_values );

    o2 = o1 + cv_scalar;

    return allEqual( o2.data, cv_addScalarResult );
UT_TEST_END()

UT_TEST_BEGIN( MatrixNxN_sub_scalar )
    Matrix5x5i o1( cv_arithmetic1 );
    Matrix5x5i o2( cv_values );

    o2 = o1 - cv_scalar;

    return allEqual( o2.data, cv_subScalarResult );
UT_TEST_END()

UT_TEST_BEGIN( MatrixNxN_mul_scalar_left )
    Matrix5x5i o1( cv_arithmetic1 );
    Matrix5x5i o2( cv_values );

    o2 = cv_scalar * o1;

    return allEqual( o2.data, cv_mulScalarResult );
UT_TEST_END()

UT_TEST_BEGIN( MatrixNxN_mul_scalar_right )
    Matrix5x5i o1( cv_arithmetic1 );
    Matrix5x5i o2( cv_values );

    o2 = o1 * cv_scalar;

    return allEqual( o2.data, cv_mulScalarResult );
UT_TEST_END()

UT_TEST_BEGIN( MatrixNxN_div_scalar )
    Matrix5x5i o1( cv_arithmetic1 );
    Matrix5x5i o2( cv_values );

    o2 = o1 / cv_scalar;

    return allEqual( o2.data, cv_divScalarResult );
UT_TEST_END()

UT_TEST_BEGIN( MatrixNxN_transpose_free )
    Matrix5x5i o1( cv_values );
    Matrix5x5i o2( cv_values );

    o2 = transpose( o1 );

    return allEqual( o2.data, cv_transpose );
UT_TEST_END()

UT_TEST_BEGIN( MatrixNxN_invert_free )
    Matrix5x5f o1( cv_invertF );

    Matrix5x5f o2 = Brimstone::invert( o1 );

    return allEqual( o2.data, cv_inverseF );
UT_TEST_END()




#ifdef BS_ZERO

UT_TEST_BEGIN( MatrixNxN_constructorZero )
    Matrix5x5i o;
    return allEqual( o.data, cv_identity );
UT_TEST_END()

#endif //BS_ZERO




#ifdef BS_CHECK_INDEX

UT_TEST_BEGIN( MatrixNxN_index_OOB )
    Matrix5x5i o;
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

UT_TEST_BEGIN( MatrixNxN_constIndex_OOB )
    const Matrix5x5i o;
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

UT_TEST_BEGIN( MatrixNxN_setRow_OOB )
    Matrix5x5i o;
    Vector5i v;

    try {
        o.setRow( (size_t)-1, v );
        return false;
    } catch( const BoundsException& ) {}

    try {
        o.setRow( 5, v );
        return false;
    } catch( const BoundsException& ) {}

    return true;
UT_TEST_END()

UT_TEST_BEGIN( MatrixNxN_setColumn_OOB )
    Matrix5x5i o;
    Vector5i v;

    try {
        o.setColumn( (size_t)-1, v );
        return false;
    } catch( const BoundsException& ) {}

    try {
        o.setColumn( 5, v );
        return false;
    } catch( const BoundsException& ) {}

    return true;
UT_TEST_END()

UT_TEST_BEGIN( MatrixNxN_getRow_OOB )
    const Matrix5x5i o;
    Vector5i v;

    try {
        v = o.getRow( (size_t)-1 );
        return false;
    } catch( const BoundsException& ) {}

    try {
        v = o.getRow( 5 );
        return false;
    } catch( const BoundsException& ) {}

    return true;
UT_TEST_END()

UT_TEST_BEGIN( MatrixNxN_getColumn_OOB )
    const Matrix5x5i o;
    Vector5i v;

    try {
        v = o.getColumn( (size_t)-1 );
        return false;
    } catch( const BoundsException& ) {}

    try {
        v = o.getColumn( 5 );
        return false;
    } catch( const BoundsException& ) {}

    return true;
UT_TEST_END()

UT_TEST_BEGIN( MatrixNxN_elem_OOB )
    Matrix5x5i o;
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

UT_TEST_BEGIN( MatrixNxN_elemConst_OOB )
    const Matrix5x5i o;
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