/*
test/Matrix2x2.cpp
-----------------------
Copyright (c) 2014, theJ89

Description:
    Unit tests for Matrix<T,2,2> specialization
*/




//Includes
#include "../Test.hpp"
#include "../utils.hpp"         //allEqual, allEqualTo

#include <brimstone/Matrix.hpp>




namespace {
    using ::Brimstone::Matrix2x2i;
    using ::Brimstone::Matrix2x2f;
    using ::Brimstone::Vector2i;
    using ::Brimstone::BoundsException;

    const size_t cv_rows             = 2;
    const size_t cv_cols             = 2;
    const size_t cv_size             = cv_rows * cv_cols;
    const int    cv_zero[4] {
         0,    0,
         0,    0
    };
    const int    cv_identity[4] {
         1,    0,
         0,    1
    };
    const int    cv_values[4] {
         1,    2,
         3,    4
    };
    const int    cv_valuesAlt[4] {
         5,    6,
         7,    8
    };
    const int    cv_valuesDiagonal[4] {
         5,    2,
         3,    8
    };
    const int   cv_diagonal[2] { 5, 8 };
    const int   cv_trace               = 5;
    const int   cv_valuesVector[2]     = {  5,  6 };
    const int   cv_mulVectorResult1[2] = { 23, 34 };
    const int   cv_mulVectorResult2[2] = { 17, 39 };
    const int   cv_lowerTriangular[4] {
         1,    0,
         3,    4
    };
    const int    cv_upperTriangular[4] {
         1,    2,
         0,    4
    };
    const int    cv_arithmetic1[4] {
         2,    8,
         9,   13
    };
    const int    cv_arithmetic2[4] {
         1,    2,
         9,  -13
    };
    const int    cv_addResult[4] {
         3,   10,
        18,    0
    };
    const int    cv_subResult[4] {
         1,    6,
         0,   26
    };
    const int    cv_mulResult[4] {
        74, -100,
       126, -151
    };
    const int    cv_scalar = 2;
    const int    cv_addScalarResult[4] {
         4,   10,
        11,   15
    };
    const int    cv_subScalarResult[4] {
         0,    6,
         7,   11
    };
    const int    cv_mulScalarResult[4] {
         4,   16,
        18,   26
    };
    const int    cv_divScalarResult[4] {
         1,    4,
         4,    6
    };
    const int    cv_negateResult[4] {
        -2,   -8,
        -9,  -13
    };
    const int    cv_transpose[4] {
         1,    3,
         2,    4
    };

    const char*  cv_output =
        "1 2\n"
        "3 4\n";
    const float  cv_zeroF[4] {
        0.0f, 0.0f,
        0.0f, 0.0f
    };
    const float  cv_valuesF[4] {
         1.0f,  2.0f,
         3.0f,  4.0f
    };
    const float  cv_valuesAltF[4] {
         5.0f,  6.0f,
         7.0f,  8.0f
    };
    const float  cv_invertF[4] {
         -1.0f,   2.0f,
         -6.0f,  17.0f
    };
    const float  cv_inverseF[4] {
          -17.0f / 5.0f,    2.0f / 5.0f,
           -6.0f / 5.0f,    1.0f / 5.0f
    };

    const float  cv_determinantF = -5.0f;
    const char*  cv_outputF =
        "1.00000 2.00000\n"
        "3.00000 4.00000\n";
}

namespace UnitTest {

UT_TEST_BEGIN( Matrix2x2_constructorFill )
    Matrix2x2i o( 1 );

    return allEqualTo( o.data, 1 );
UT_TEST_END()

UT_TEST_BEGIN( Matrix2x2_constructorCppRange )
    Matrix2x2i o( cv_values );

    return allEqual( o.data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( Matrix2x2_constructorInitializerList )
    Matrix2x2i o( {
        cv_values[0],  cv_values[1],
        cv_values[2],  cv_values[3]
    } );

    return allEqual( o.data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( Matrix2x2_setCppRange )
    Matrix2x2i o( cv_values );

    o.set( cv_valuesAlt );

    return allEqual( o.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( Matrix2x2_setInitializerList )
    Matrix2x2i o( cv_values );

    o.set( {
        cv_valuesAlt[0],  cv_valuesAlt[1],
        cv_valuesAlt[2],  cv_valuesAlt[3]
    } );

    return allEqual( o.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( Matrix2x2_getCppRange )
    int values[ cv_size ];
    copyAll( cv_values, values );
    Matrix2x2i o( cv_valuesAlt );

    o.get( values );

    return allEqual( o.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( Matrix2x2_fill )
    Matrix2x2i o( cv_values );

    o.fill( 1 );

    return allEqualTo( o.data, 1 );
UT_TEST_END()

UT_TEST_BEGIN( Matrix2x2_begin )
    Matrix2x2i o( cv_values );

    return o.begin() == std::begin( o.data );
UT_TEST_END()

UT_TEST_BEGIN( Matrix2x2_beginConst )
    const Matrix2x2i o( cv_values );

    return o.begin() == std::begin( o.data );
UT_TEST_END()

UT_TEST_BEGIN( Matrix2x2_cbegin )
    Matrix2x2i o( cv_values );

    return o.cbegin() == std::cbegin( o.data );
UT_TEST_END()

UT_TEST_BEGIN( Matrix2x2_end )
    Matrix2x2i o( cv_values );

    return o.end() == std::end( o.data );
UT_TEST_END()

UT_TEST_BEGIN( Matrix2x2_endConst )
    const Matrix2x2i o( cv_values );

    return o.end() == std::end( o.data );
UT_TEST_END()

UT_TEST_BEGIN( Matrix2x2_cend )
    Matrix2x2i o( cv_values );

    return o.cend() == std::cend( o.data );
UT_TEST_END()

UT_TEST_BEGIN( Matrix2x2_rangedFor )
    Matrix2x2i o( cv_values );

    int idx = 0;
    for( int& i : o )
        i = cv_valuesAlt[idx++];

    return allEqual( o.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( Matrix2x2_rangedForConst )
    int data[cv_size];
    copyAll( cv_values, data );
    const Matrix2x2i o( cv_valuesAlt );

    int idx = 0;
    for( int i : o )
        data[idx++] = i;

    return allEqual( data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( Matrix2x2_index )
    Matrix2x2i o( cv_values );

    for( size_t i = 0; i < cv_size; ++i )
        o[i] = cv_valuesAlt[i];

    return allEqual( o.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( Matrix2x2_indexConst )
    int data[cv_size];
    const Matrix2x2i o( cv_values );

    for( size_t i = 0; i < cv_size; ++i )
        data[i] = o[i];

    return allEqual( data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( Matrix2x2_constructorValues )
    Matrix2x2i o(
        cv_values[0], cv_values[1],
        cv_values[2], cv_values[3]
    );

    return allEqual( o.data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( Matrix2x2_setValues )
    Matrix2x2i o( cv_values );

    o.set(
        cv_valuesAlt[0], cv_valuesAlt[1],
        cv_valuesAlt[2], cv_valuesAlt[3]
    );

    return allEqual( o.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( Matrix2x2_getValues )
    Matrix2x2i o( cv_values );
    int data[cv_size];

    copyAll( cv_valuesAlt, data );

    o.get(
        data[0], data[1],
        data[2], data[3]
    );

    return allEqual( data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( Matrix2x2_constructorCopy )
    Matrix2x2f o1( cv_valuesAltF );

    Matrix2x2i o2( o1 );

    return allEqual( o2.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( Matrix2x2_isSquare )
    Matrix2x2i o( cv_values );

    return o.isSquare() == true;
UT_TEST_END()

UT_TEST_BEGIN( Matrix2x2_zero )
    Matrix2x2i o( cv_values );

    o.zero();

    return allEqual( o.data, cv_zero );
UT_TEST_END()

UT_TEST_BEGIN( Matrix2x2_isZero )
    Matrix2x2i o1( cv_zero );
    Matrix2x2i o2( cv_values );

    return o1.isZero() == true  &&
           o2.isZero() == false;
UT_TEST_END()

UT_TEST_BEGIN( Matrix2x2_getDeterminant )
    Matrix2x2f o( cv_invertF );

    return o.getDeterminant() == cv_determinantF;
UT_TEST_END()

UT_TEST_BEGIN( Matrix2x2_getTrace )
    Matrix2x2i o( cv_values );

    return o.getTrace() == cv_trace;
UT_TEST_END()

UT_TEST_BEGIN( Matrix2x2_setRow )
    Matrix2x2i o( cv_valuesAlt );

    o.setRow( 0, Vector2i( cv_values[0], cv_values[1] ) );
    o.setRow( 1, Vector2i( cv_values[2], cv_values[3] ) );

    return allEqual( o.data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( Matrix2x2_setColumn )
    Matrix2x2i o( cv_valuesAlt );

    o.setColumn( 0, Vector2i( cv_values[0], cv_values[2] ) );
    o.setColumn( 1, Vector2i( cv_values[1], cv_values[3] ) );

    return allEqual( o.data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( Matrix2x2_getRow )
    Matrix2x2i o( cv_values );

    return o.getRow( 0 ) == Vector2i( cv_values[0], cv_values[1] ) &&
           o.getRow( 1 ) == Vector2i( cv_values[2], cv_values[3] );
UT_TEST_END()

UT_TEST_BEGIN( Matrix2x2_getColumn )
    Matrix2x2i o( cv_values );

    return o.getColumn( 0 ) == Vector2i( cv_values[0], cv_values[2] ) &&
           o.getColumn( 1 ) == Vector2i( cv_values[1], cv_values[3] );
UT_TEST_END()

UT_TEST_BEGIN( Matrix2x2_setDiagonal )
    Matrix2x2i o( cv_values );

    o.setDiagonal( Vector2i( cv_diagonal ) );

    return allEqual( o.data, cv_valuesDiagonal );
UT_TEST_END()

UT_TEST_BEGIN( Matrix2x2_getDiagonal )
    Matrix2x2i o( cv_valuesDiagonal );

    Vector2i v = o.getDiagonal();

    return allEqual( v.data, cv_diagonal );
UT_TEST_END()

UT_TEST_BEGIN( Matrix2x2_isTriangular )
    Matrix2x2i o1( cv_lowerTriangular );
    Matrix2x2i o2( cv_upperTriangular );
    Matrix2x2i o3( cv_values          );

    return o1.isTriangular() == true &&
           o2.isTriangular() == true &&
           o3.isTriangular() == false;
UT_TEST_END()

UT_TEST_BEGIN( Matrix2x2_isLowerTriangular )
    Matrix2x2i o1( cv_lowerTriangular );
    Matrix2x2i o2( cv_values          );

    return o1.isLowerTriangular() == true &&
           o2.isLowerTriangular() == false;
UT_TEST_END()

UT_TEST_BEGIN( Matrix2x2_isUpperTriangular )
    Matrix2x2i o1( cv_upperTriangular );
    Matrix2x2i o2( cv_values          );

    return o1.isUpperTriangular() == true &&
           o2.isUpperTriangular() == false;
UT_TEST_END()

UT_TEST_BEGIN( Matrix2x2_isDiagonal )
    Matrix2x2i o1( cv_identity );
    Matrix2x2i o2( cv_values   );

    return o1.isDiagonal() == true &&
           o2.isDiagonal() == false;
UT_TEST_END()

UT_TEST_BEGIN( Matrix2x2_isInvertible )
    Matrix2x2f o1( cv_invertF );
    Matrix2x2f o2( cv_zeroF   );

    return o1.isInvertible() == true &&
           o2.isInvertible() == false;
UT_TEST_END()

UT_TEST_BEGIN( Matrix2x2_transpose )
    Matrix2x2i o( cv_values );

    o.transpose();

    return allEqual( o.data, cv_transpose );
UT_TEST_END()

UT_TEST_BEGIN( Matrix2x2_invert )
    Matrix2x2f o( cv_invertF );

    o.invert();

    return allEqual( o.data, cv_inverseF );
UT_TEST_END()

UT_TEST_BEGIN( Matrix2x2_getRows )
    Matrix2x2i o( cv_values );

    return o.getRows() == cv_rows;
UT_TEST_END()

UT_TEST_BEGIN( Matrix2x2_getColumns )
    Matrix2x2i o( cv_values );

    return o.getColumns() == cv_cols;
UT_TEST_END()

UT_TEST_BEGIN( Matrix2x2_identity )
    Matrix2x2i o( cv_values );

    o.identity();

    return allEqual( o.data, cv_identity );
UT_TEST_END()

UT_TEST_BEGIN( Matrix2x2_isIdentity )
    Matrix2x2i o1( cv_identity );
    Matrix2x2i o2( cv_values );

    return o1.isIdentity() == true &&
           o2.isIdentity() == false;
UT_TEST_END()

UT_TEST_BEGIN( Matrix2x2_assignCopy )
    Matrix2x2f o1( cv_valuesAltF );
    Matrix2x2i o2( cv_values );

    o2 = o1;

    return allEqual( o2.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( Matrix2x2_equals )
    Matrix2x2i o1( cv_values );
    Matrix2x2i o2( cv_values );
    Matrix2x2i o3( cv_valuesAlt );

    return ( o1 == o2 ) == true   &&
           ( o1 == o3 ) == false;
UT_TEST_END()

UT_TEST_BEGIN( Matrix2x2_notEquals )
    Matrix2x2i o1( cv_values );
    Matrix2x2i o2( cv_values );
    Matrix2x2i o3( cv_valuesAlt );

    return ( o1 != o2 ) == false  &&
           ( o1 != o3 ) == true;
UT_TEST_END()

UT_TEST_BEGIN( Matrix2x2_output_int )
    Matrix2x2i o( cv_values );

    std::stringstream sout;
    sout << o;

    return sout.str() == cv_output;
UT_TEST_END()

UT_TEST_BEGIN( Matrix2x2_output_float )
    Matrix2x2f o( cv_valuesF );

    std::stringstream sout;
    sout << o;

    return sout.str() == cv_outputF;
UT_TEST_END()

UT_TEST_BEGIN( Matrix2x2_addAssign_matrix )
    Matrix2x2i o1( cv_arithmetic1 );
    Matrix2x2i o2( cv_arithmetic2 );

    o1 += o2;

    return allEqual( o1.data, cv_addResult );
UT_TEST_END()

UT_TEST_BEGIN( Matrix2x2_subAssign_matrix )
    Matrix2x2i o1( cv_arithmetic1 );
    Matrix2x2i o2( cv_arithmetic2 );

    o1 -= o2;

    return allEqual( o1.data, cv_subResult );
UT_TEST_END()

UT_TEST_BEGIN( Matrix2x2_mulAssign_matrix )
    Matrix2x2i o1( cv_arithmetic1 );
    Matrix2x2i o2( cv_arithmetic2 );

    o1 *= o2;

    return allEqual( o1.data, cv_mulResult );
UT_TEST_END()

UT_TEST_BEGIN( Matrix2x2_mulAssign_vector )
    Matrix2x2i o( cv_values );
    Vector2i   v( cv_valuesVector );

    v *= o;

    return allEqual( v.data, cv_mulVectorResult1 );
UT_TEST_END()

UT_TEST_BEGIN( Matrix2x2_addAssign_scalar )
    Matrix2x2i o( cv_arithmetic1 );

    o += cv_scalar;

    return allEqual( o.data, cv_addScalarResult );
UT_TEST_END()

UT_TEST_BEGIN( Matrix2x2_subAssign_scalar )
    Matrix2x2i o( cv_arithmetic1 );

    o -= cv_scalar;

    return allEqual( o.data, cv_subScalarResult );
UT_TEST_END()

UT_TEST_BEGIN( Matrix2x2_mulAssign_scalar )
    Matrix2x2i o( cv_arithmetic1 );

    o *= cv_scalar;

    return allEqual( o.data, cv_mulScalarResult );
UT_TEST_END()

UT_TEST_BEGIN( Matrix2x2_divAssign_scalar )
    Matrix2x2i o( cv_arithmetic1 );

    o /= cv_scalar;

    return allEqual( o.data, cv_divScalarResult );
UT_TEST_END()

UT_TEST_BEGIN( Matrix2x2_elem )
    Matrix2x2i o( cv_values );

    for( size_t r = 0; r < cv_rows; ++r )
        for( size_t c = 0; c < cv_cols; ++c )
            o( r, c ) = cv_valuesAlt[ r * cv_cols + c ];

    return allEqual( o.data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( Matrix2x2_elemConst )
    const Matrix2x2i o( cv_values );

    for( size_t r = 0; r < cv_rows; ++r )
        for( size_t c = 0; c < cv_cols; ++c )
            if( o( r, c ) != cv_values[ r * cv_cols + c ] )
                return false;

    return true;
UT_TEST_END()

UT_TEST_BEGIN( Matrix2x2_unaryPlus )
    Matrix2x2i o1( cv_arithmetic1 );
    Matrix2x2i o2( cv_arithmetic2 );

    o2 = +o1;

    return allEqual( o2.data, cv_arithmetic1 );
UT_TEST_END()

UT_TEST_BEGIN( Matrix2x2_unaryMinus )
    Matrix2x2i o1( cv_arithmetic1 );
    Matrix2x2i o2( cv_arithmetic2 );

    o2 = -o1;

    return allEqual( o2.data, cv_negateResult );
UT_TEST_END()

UT_TEST_BEGIN( Matrix2x2_add_matrix )
    Matrix2x2i o1( cv_arithmetic1 );
    Matrix2x2i o2( cv_arithmetic2 );
    Matrix2x2i o3( cv_values );

    o3 = o1 + o2;

    return allEqual( o3.data, cv_addResult );
UT_TEST_END()

UT_TEST_BEGIN( Matrix2x2_sub_matrix )
    Matrix2x2i o1( cv_arithmetic1 );
    Matrix2x2i o2( cv_arithmetic2 );
    Matrix2x2i o3( cv_values );

    o3 = o1 - o2;

    return allEqual( o3.data, cv_subResult );
UT_TEST_END()

UT_TEST_BEGIN( Matrix2x2_mul_matrix )
    Matrix2x2i o1( cv_arithmetic1 );
    Matrix2x2i o2( cv_arithmetic2 );
    Matrix2x2i o3( cv_values );

    o3 = o1 * o2;

    return allEqual( o3.data, cv_mulResult );
UT_TEST_END()

UT_TEST_BEGIN( Matrix2x2_mul_vector_left )
    Matrix2x2i o( cv_values );
    Vector2i   v1( cv_valuesVector );

    Vector2i   v2 = v1 * o;

    return allEqual( v2.data, cv_mulVectorResult1 );
UT_TEST_END()

UT_TEST_BEGIN( Matrix2x2_mul_vector_right )
    Matrix2x2i o( cv_values );
    Vector2i   v1( cv_valuesVector );

    Vector2i   v2 = o * v1;

    return allEqual( v2.data, cv_mulVectorResult2 );
UT_TEST_END()

UT_TEST_BEGIN( Matrix2x2_add_scalar_left )
    Matrix2x2i o1( cv_arithmetic1 );
    Matrix2x2i o2( cv_values );

    o2 = cv_scalar + o1;

    return allEqual( o2.data, cv_addScalarResult );
UT_TEST_END()

UT_TEST_BEGIN( Matrix2x2_add_scalar_right )
    Matrix2x2i o1( cv_arithmetic1 );
    Matrix2x2i o2( cv_values );

    o2 = o1 + cv_scalar;

    return allEqual( o2.data, cv_addScalarResult );
UT_TEST_END()

UT_TEST_BEGIN( Matrix2x2_sub_scalar )
    Matrix2x2i o1( cv_arithmetic1 );
    Matrix2x2i o2( cv_values );

    o2 = o1 - cv_scalar;

    return allEqual( o2.data, cv_subScalarResult );
UT_TEST_END()

UT_TEST_BEGIN( Matrix2x2_mul_scalar_left )
    Matrix2x2i o1( cv_arithmetic1 );
    Matrix2x2i o2( cv_values );

    o2 = cv_scalar * o1;

    return allEqual( o2.data, cv_mulScalarResult );
UT_TEST_END()

UT_TEST_BEGIN( Matrix2x2_mul_scalar_right )
    Matrix2x2i o1( cv_arithmetic1 );
    Matrix2x2i o2( cv_values );

    o2 = o1 * cv_scalar;

    return allEqual( o2.data, cv_mulScalarResult );
UT_TEST_END()

UT_TEST_BEGIN( Matrix2x2_div_scalar )
    Matrix2x2i o1( cv_arithmetic1 );
    Matrix2x2i o2( cv_values );

    o2 = o1 / cv_scalar;

    return allEqual( o2.data, cv_divScalarResult );
UT_TEST_END()

UT_TEST_BEGIN( Matrix2x2_transpose_free )
    Matrix2x2i o1( cv_values );
    Matrix2x2i o2( cv_values );

    o2 = transpose( o1 );

    return allEqual( o2.data, cv_transpose );
UT_TEST_END()

UT_TEST_BEGIN( Matrix2x2_invert_free )
    Matrix2x2f o1( cv_invertF );

    Matrix2x2f o2 = Brimstone::invert( o1 );

    return allEqual( o2.data, cv_inverseF );
UT_TEST_END()




#ifdef BS_ZERO

UT_TEST_BEGIN( Matrix2x2_constructorZero )
    Matrix2x2i o;
    return allEqual( o.data, cv_identity );
UT_TEST_END()

#endif //BS_ZERO




#ifdef BS_CHECK_INDEX

UT_TEST_BEGIN( Matrix2x2_index_OOB )
    Matrix2x2i o;
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

UT_TEST_BEGIN( Matrix2x2_constIndex_OOB )
    const Matrix2x2i o;
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

UT_TEST_BEGIN( Matrix2x2_setRow_OOB )
    Matrix2x2i o;
    Vector2i v;

    try {
        o.setRow( (size_t)-1, v );
        return false;
    } catch( const BoundsException& ) {}

    try {
        o.setRow( 2, v );
        return false;
    } catch( const BoundsException& ) {}

    return true;
UT_TEST_END()

UT_TEST_BEGIN( Matrix2x2_setColumn_OOB )
    Matrix2x2i o;
    Vector2i v;

    try {
        o.setColumn( (size_t)-1, v );
        return false;
    } catch( const BoundsException& ) {}

    try {
        o.setColumn( 2, v );
        return false;
    } catch( const BoundsException& ) {}

    return true;
UT_TEST_END()

UT_TEST_BEGIN( Matrix2x2_getRow_OOB )
    const Matrix2x2i o;
    Vector2i v;

    try {
        v = o.getRow( (size_t)-1 );
        return false;
    } catch( const BoundsException& ) {}

    try {
        v = o.getRow( 2 );
        return false;
    } catch( const BoundsException& ) {}

    return true;
UT_TEST_END()

UT_TEST_BEGIN( Matrix2x2_getColumn_OOB )
    const Matrix2x2i o;
    Vector2i v;

    try {
        v = o.getColumn( (size_t)-1 );
        return false;
    } catch( const BoundsException& ) {}

    try {
        v = o.getColumn( 2 );
        return false;
    } catch( const BoundsException& ) {}

    return true;
UT_TEST_END()

UT_TEST_BEGIN( Matrix2x2_elem_OOB )
    Matrix2x2i o;
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

UT_TEST_BEGIN( Matrix2x2_elemConst_OOB )
    const Matrix2x2i o;
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