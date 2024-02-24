/*
matrix/Matrix4x4.hpp
--------------------
Copyright (c) 2024, theJ89

Description:
    Specialization of Matrix for 3x3 matrices.
    Adds the following typedefs for convenience:
        Matrix4x4< T >: Matrix<T,4,4>
        Matrix4x4i:     Matrix<int32,4,4>
        Matrix4x4f:     Matrix<float,4,4>
        Matrix4x4d:     Matrix<double,4,4>
*/
#ifndef BS_MATRIX_MATRIX4X4_HPP
#define BS_MATRIX_MATRIX4X4_HPP




//Includes
#include <utility>                          //std::swap

#include <brimstone/matrix/MatrixNxN.hpp>   //Matrix, BS_MATRIX_SQUARE_DECLARE_METHODS



namespace Brimstone {

template< typename T >
class Matrix< T, 4, 4 > {
public:
//C4201: nonstandard extension used : nameless struct/union
//It's a non-standard feature, but VC++, G++, and LLVM support it so it shouldn't be too much of an issue
#pragma warning( push )
#pragma warning( disable: 4201 )

    union {
        T              data[ 16 ];
        T              elem[4][4];
        struct {
            Vector< T, 4 > row[ 4 ];
        };
        struct {
            T _00, _01, _02, _03,
              _10, _11, _12, _13,
              _20, _21, _22, _23,
              _30, _31, _32, _33;
        };
    };

#pragma warning( pop )
public:
    BS_ARRAY_DECLARE_INHERITED_METHODS( Matrix, T )
    BS_ARRAY_DECLARE_METHODS( Matrix, T )
    BS_MATRIX_DECLARE_METHODS( 4, 4 )
    BS_MATRIX_SQUARE_DECLARE_METHODS( 4 )

    Matrix(
        const T _00, const T _01, const T _02, const T _03,
        const T _10, const T _11, const T _12, const T _13,
        const T _20, const T _21, const T _22, const T _23,
        const T _30, const T _31, const T _32, const T _33
    );

    void set(
        const T _00, const T _01, const T _02, const T _03,
        const T _10, const T _11, const T _12, const T _13,
        const T _20, const T _21, const T _22, const T _23,
        const T _30, const T _31, const T _32, const T _33
    );

    void get(
        T& _00Out, T& _01Out, T& _02Out, T& _03Out,
        T& _10Out, T& _11Out, T& _12Out, T& _13Out,
        T& _20Out, T& _21Out, T& _22Out, T& _23Out,
        T& _30Out, T& _31Out, T& _32Out, T& _33Out
    ) const;
private:
    static const Matrix m_identity;
};

BS_ARRAY_DEFINE_METHODS( Matrix, T, data, BS_TMPL_1( typename T ), BS_SPEC_3( T, 4, 4 ) )
BS_MATRIX_DEFINE_METHODS( 4, 4, BS_TMPL_1( typename T ), BS_SPEC_3( T, 4, 4 ) )
BS_MATRIX_SQUARE_DEFINE_METHODS( 4, BS_TMPL_1( typename T ), BS_SPEC_3( T, 4, 4 ) )




//Forward declarations
template< typename T >
Matrix< T, 4, 4 > invert( const Matrix< T, 4, 4 >& matrix );
template< typename T >
Matrix< T, 4, 4 > transpose( const Matrix< T, 4, 4 >& matrix );




template< typename T >
const Matrix< T, 4, 4 > Matrix< T, 4, 4 >::m_identity = Matrix< T, 4, 4 >(
    static_cast< T >( 1 ),  static_cast< T >( 0 ),  static_cast< T >( 0 ),  static_cast< T >( 0 ),
    static_cast< T >( 0 ),  static_cast< T >( 1 ),  static_cast< T >( 0 ),  static_cast< T >( 0 ),
    static_cast< T >( 0 ),  static_cast< T >( 0 ),  static_cast< T >( 1 ),  static_cast< T >( 0 ),
    static_cast< T >( 0 ),  static_cast< T >( 0 ),  static_cast< T >( 0 ),  static_cast< T >( 1 )
);

template< typename T >
inline Matrix< T, 4, 4 >::Matrix( const T& elem ) :
    _00( elem ),    _01( elem ),    _02( elem ),    _03( elem ),
    _10( elem ),    _11( elem ),    _12( elem ),    _13( elem ),
    _20( elem ),    _21( elem ),    _22( elem ),    _23( elem ),
    _30( elem ),    _31( elem ),    _32( elem ),    _33( elem ) {
}

template< typename T >
template< typename T2 >
inline Matrix< T, 4, 4 >::Matrix( const T2& cppRange ) :
    _00( cppRange[ 0] ),    _01( cppRange[ 1] ),    _02( cppRange[ 2] ),    _03( cppRange[ 3] ),
    _10( cppRange[ 4] ),    _11( cppRange[ 5] ),    _12( cppRange[ 6] ),    _13( cppRange[ 7] ),
    _20( cppRange[ 8] ),    _21( cppRange[ 9] ),    _22( cppRange[10] ),    _23( cppRange[11] ),
    _30( cppRange[12] ),    _31( cppRange[13] ),    _32( cppRange[14] ),    _33( cppRange[15] ) {
    BS_ASSERT_SIZE( rangeSize( cppRange ), 16 );
}

template< typename T >
inline Matrix< T, 4, 4 >::Matrix( std::initializer_list< T > il ) :
    _00( *( std::begin( il )      ) ),
    _01( *( std::begin( il ) +  1 ) ),
    _02( *( std::begin( il ) +  2 ) ),
    _03( *( std::begin( il ) +  3 ) ),
    _10( *( std::begin( il ) +  4 ) ),
    _11( *( std::begin( il ) +  5 ) ),
    _12( *( std::begin( il ) +  6 ) ),
    _13( *( std::begin( il ) +  7 ) ),
    _20( *( std::begin( il ) +  8 ) ),
    _21( *( std::begin( il ) +  9 ) ),
    _22( *( std::begin( il ) + 10 ) ),
    _23( *( std::begin( il ) + 11 ) ),
    _30( *( std::begin( il ) + 12 ) ),
    _31( *( std::begin( il ) + 13 ) ),
    _32( *( std::begin( il ) + 14 ) ),
    _33( *( std::begin( il ) + 15 ) ) {
    BS_ASSERT_SIZE( rangeSize( il ), 16 );
}

template< typename T >
template< typename T2 >
inline void Matrix< T, 4, 4 >::set( const T2& cppRange ) {
    BS_ASSERT_SIZE( rangeSize( cppRange ), 16 );

    _00 = cppRange[ 0];     _01 = cppRange[ 1];     _02 = cppRange[ 2];     _03 = cppRange[ 3];
    _10 = cppRange[ 4];     _11 = cppRange[ 5];     _12 = cppRange[ 6];     _13 = cppRange[ 7];
    _20 = cppRange[ 8];     _21 = cppRange[ 9];     _22 = cppRange[10];     _23 = cppRange[11];
    _30 = cppRange[12];     _31 = cppRange[13];     _32 = cppRange[14];     _33 = cppRange[15];
}
template< typename T >
inline void Matrix< T, 4, 4 >::set( std::initializer_list< T > il ) {
    BS_ASSERT_SIZE( rangeSize( il ), 16 );

    auto it = std::begin( il );
    _00 = *( it      );
    _01 = *( it +  1 );
    _02 = *( it +  2 );
    _03 = *( it +  3 );
    _10 = *( it +  4 );
    _11 = *( it +  5 );
    _12 = *( it +  6 );
    _13 = *( it +  7 );
    _20 = *( it +  8 );
    _21 = *( it +  9 );
    _22 = *( it + 10 );
    _23 = *( it + 11 );
    _30 = *( it + 12 );
    _31 = *( it + 13 );
    _32 = *( it + 14 );
    _33 = *( it + 15 );
}
template< typename T >
template< typename T2 >
inline void Matrix< T, 4, 4 >::get( T2& cppRangeOut ) const {
    BS_ASSERT_SIZE( rangeSize( cppRangeOut ), 16 );

    cppRangeOut[ 0] = _00;  cppRangeOut[ 1] = _01;  cppRangeOut[ 2] = _02;  cppRangeOut[ 3] = _03;
    cppRangeOut[ 4] = _10;  cppRangeOut[ 5] = _11;  cppRangeOut[ 6] = _12;  cppRangeOut[ 7] = _13;
    cppRangeOut[ 8] = _20;  cppRangeOut[ 9] = _21;  cppRangeOut[10] = _22;  cppRangeOut[11] = _23;
    cppRangeOut[12] = _30;  cppRangeOut[13] = _31;  cppRangeOut[14] = _32;  cppRangeOut[15] = _33;
}

template< typename T >
inline void Matrix< T, 4, 4 >::fill( const T& elem ) {
    _00 = elem;     _01 = elem;     _02 = elem;     _03 = elem;
    _10 = elem;     _11 = elem;     _12 = elem;     _13 = elem;
    _20 = elem;     _21 = elem;     _22 = elem;     _23 = elem;
    _30 = elem;     _31 = elem;     _32 = elem;     _33 = elem;
}

template< typename T >
Matrix< T, 4, 4 >::Matrix()
#ifdef BS_ZERO
    : Matrix( m_identity )
#endif //BS_ZERO
{
}

template< typename T >
template< typename T2 >
Matrix< T, 4, 4 >::Matrix( const Matrix< T2, 4, 4 >& toCopy ) :
    _00( static_cast< T >( toCopy._00 ) ),  _01( static_cast< T >( toCopy._01 ) ),  _02( static_cast< T >( toCopy._02 ) ),  _03( static_cast< T >( toCopy._03 ) ),
    _10( static_cast< T >( toCopy._10 ) ),  _11( static_cast< T >( toCopy._11 ) ),  _12( static_cast< T >( toCopy._12 ) ),  _13( static_cast< T >( toCopy._13 ) ),
    _20( static_cast< T >( toCopy._20 ) ),  _21( static_cast< T >( toCopy._21 ) ),  _22( static_cast< T >( toCopy._22 ) ),  _23( static_cast< T >( toCopy._23 ) ),
    _30( static_cast< T >( toCopy._30 ) ),  _31( static_cast< T >( toCopy._31 ) ),  _32( static_cast< T >( toCopy._32 ) ),  _33( static_cast< T >( toCopy._33 ) ) {
}

template< typename T >
bool Matrix< T, 4, 4 >::isSquare() const {
    return true;
}

template< typename T >
void Matrix< T, 4, 4 >::zero() {
    T zero = static_cast< T >( 0 );
    _00 = zero;     _01 = zero;     _02 = zero;     _03 = zero;
    _10 = zero;     _11 = zero;     _12 = zero;     _13 = zero;
    _20 = zero;     _21 = zero;     _22 = zero;     _23 = zero;
    _30 = zero;     _31 = zero;     _32 = zero;     _33 = zero;
}

template< typename T >
bool Matrix< T, 4, 4 >::isZero() const {
    T zero = static_cast< T >( 0 );
    return _00 == zero &&   _01 == zero &&  _02 == zero &&  _03 == zero &&
           _10 == zero &&   _11 == zero &&  _12 == zero &&  _13 == zero &&
           _20 == zero &&   _21 == zero &&  _22 == zero &&  _23 == zero &&
           _30 == zero &&   _31 == zero &&  _32 == zero &&  _33 == zero;
}

template< typename T >
void Matrix< T, 4, 4 >::setRow( const size_t row, const Vector< T, 4 >& values ) {
    BS_ASSERT_INDEX( row, 3 );

    elem[row][0] = values.data[0];
    elem[row][1] = values.data[1];
    elem[row][2] = values.data[2];
    elem[row][3] = values.data[3];
}

template< typename T >
void Matrix< T, 4, 4 >::setColumn( const size_t col, const Vector< T, 4 >& values ) {
    BS_ASSERT_INDEX( col, 3 );

    elem[0][col] = values.data[0];
    elem[1][col] = values.data[1];
    elem[2][col] = values.data[2];
    elem[3][col] = values.data[3];
}

template< typename T >
Vector< T, 4 > Matrix< T, 4, 4 >::getRow( const size_t row ) const {
    BS_ASSERT_INDEX( row, 3 );

    return Vector< T, 4 >(
        elem[row][0],
        elem[row][1],
        elem[row][2],
        elem[row][3]
    );
}

template< typename T >
Vector< T, 4 > Matrix< T, 4, 4 >::getColumn( const size_t col ) const {
    BS_ASSERT_INDEX( col, 3 );

    return Vector< T, 4 >(
        elem[0][col],
        elem[1][col],
        elem[2][col],
        elem[3][col]
    );
}

template< typename T >
void Matrix< T, 4, 4 >::setDiagonal( const Vector< T, 4 >& values ) {
    _00 = values.x;
    _11 = values.y;
    _22 = values.z;
    _33 = values.w;
}

template< typename T >
Vector< T, 4 > Matrix< T, 4, 4 >::getDiagonal() const {
    return Vector< T, 4 >( _00, _11, _22, _33 );
}

template< typename T >
T Matrix< T, 4, 4 >::getDeterminant() const {
    //Certain minors occur multiple times in the Laplace expansion for 4x4 matrices.
    //They're calculated here ahead of time.
    T m1 = _22 * _33 - _23 * _32;
    T m2 = _23 * _31 - _21 * _33;
    T m3 = _22 * _31 - _21 * _32;
    T m4 = _20 * _33 - _23 * _30;
    T m5 = _22 * _30 - _20 * _32;
    T m6 = _20 * _31 - _21 * _30;

    return _00 * ( _11 * m1 + _12 * m2 - _13 *  m3 ) +
           _01 * ( _12 * m4 + _13 * m5 - _10 *  m1 ) +
           _02 * ( _13 * m6 - _10 * m2 - _11 *  m4 ) +
           _03 * ( _10 * m3 - _12 * m6 - _11 *  m5 );
}

template< typename T >
bool Matrix< T, 4, 4 >::isLowerTriangular() const {
    T zero = static_cast< T >( 0 );

    return _01 == zero && _02 == zero && _03 == zero &&
                          _12 == zero && _13 == zero &&
                                         _23 == zero;
}

template< typename T >
bool Matrix< T, 4, 4 >::isUpperTriangular() const {
    T zero = static_cast< T >( 0 );

    return _10 == zero &&
           _20 == zero && _21 == zero &&
           _30 == zero && _31 == zero && _32 == zero;
}

template< typename T >
T Matrix< T, 4, 4 >::getTrace() const {
    return _00 + _11 + _22 + _33;
}

template< typename T >
void Matrix< T, 4, 4 >::transpose() {
    std::swap( _01, _10 );
    std::swap( _02, _20 );
    std::swap( _03, _30 );

    std::swap( _12, _21 );
    std::swap( _13, _31 );

    std::swap( _23, _32 );
}

template< typename T >
void Matrix< T, 4, 4 >::invert() {
    //Matrix of minors -> Matrix of Cofactors (* -1 on odd spaces) -> Transpose
    /*
    _00 _01 _02 _03  + - + -
    _10 _11 _12 _13  - + - +
    _20 _21 _22 _23  + - + -
    _30 _31 _32 _33  - + - +
    */

    //Certain minors occur multiple times in the Laplace expansion for 4x4 matrices.
    //The minors used for calculating the determinant are calculated here.
    //The rest of the minors are calculated after the determinant check below,
    //to prevent doing unnecessary work in the case that determinant is checked and det == 0.
    T m1 = _22 * _33 - _23 * _32;
    T m2 = _21 * _33 - _23 * _31;
    T m3 = _21 * _32 - _22 * _31;
    T m4 = _20 * _33 - _23 * _30;
    T m5 = _20 * _32 - _22 * _30;
    T m6 = _20 * _31 - _21 * _30;

    //The cofactors for the first row are used both to calculate the determinant
    //and the inverse itself, so they are calculated ahead of time here to be reused later
    /*
    _11 _12 _13 + - +
    _21 _22 _23 - + -
    _31 _32 _33 + - +
    */
    T c00 = _11 * m1 -
            _12 * m2 +
            _13 * m3;   //+

    /*
    _10 _12 _13 + - +
    _20 _22 _23 - + -
    _30 _32 _33 + - +
    */
    T c01 = _12 * m4 -
            _13 * m5 -
            _10 * m1;   //-

    /*
    _10 _11 _13 + - +
    _20 _21 _23 - + -
    _30 _31 _33 + - +
    */
    T c02 = _10 * m2 -
            _11 * m4 +
            _13 * m6;   //+

    /*
    _10 _11 _12 + - +
    _20 _21 _22 - + -
    _30 _31 _32 + - +
    */
    T c03 = _11 * m5 -
            _12 * m6 -
            _10 * m3;   //-


    T det = _00 * c00 +
            _01 * c01 +
            _02 * c02 +
            _03 * c03;

    BS_ASSERT_NONZERO_DIVISOR( det );

    T m7  = _12 * _33 - _13 * _32;
    T m8  = _11 * _33 - _13 * _31;
    T m9  = _11 * _32 - _12 * _31;
    T m10 = _10 * _33 - _13 * _30;
    T m11 = _10 * _32 - _12 * _30;
    T m12 = _10 * _31 - _11 * _30;
    T m13 = _12 * _23 - _13 * _22;
    T m14 = _11 * _23 - _13 * _21;
    T m15 = _11 * _22 - _12 * _21;
    T m16 = _10 * _23 - _13 * _20;
    T m17 = _10 * _22 - _12 * _20;
    T m18 = _10 * _21 - _11 * _20;

    T o01 = _01;
    T o02 = _02;
    T o03 = _03;

    /*
    _10 _12 _13  + - +
    _20 _22 _23  - + -
    _30 _32 _33  + - +
    */
    _01 = (
        o02 * m2 -
        o01 * m1 -
        o03 * m3
    ) / det;    //- _01

    /*
    _10 _11 _13  + - +
    _20 _21 _23  - + -
    _30 _31 _33  + - +
    */
    _02 = (
        o01 * m7 -
        o02 * m8 +
        o03 * m9
    ) / det;    //+ _02

    /*
    _10 _11 _12  + - +
    _20 _21 _22  - + -
    _30 _31 _32  + - +
    */
    _03 = (
        o02 * m14 -
        o01 * m13 -
        o03 * m15
    ) / det;    //- _03

    /*
    _00 _02 _03  + - +
    _20 _22 _23  - + -
    _30 _32 _33  + - +
    */
    _11 = (
        _00 * m1 -
        o02 * m4 +
        o03 * m5
    ) / det;    //+ _11

    /*
    _00 _01 _03  + - +
    _20 _21 _23  - + -
    _30 _31 _33  + - +
    */
    _12 = (
        o02 * m10 -
        _00 * m7  -
        o03 * m11
    ) / det;    //- _12

    /*
    _00 _01 _02  + - +
    _20 _21 _22  - + -
    _30 _31 _32  + - +
    */
    _13 = (
        _00 * m13 -
        o02 * m16 +
        o03 * m17
    ) / det;    //+ _13

    /*
    _00 _02 _03  + - +
    _10 _12 _13  - + -
    _30 _32 _33  + - +
    */
    _21 = (
        o01 * m4 -
        _00 * m2 -
        o03 * m6
    ) / det;    //- _21

    /*
    _00 _01 _03  + - +
    _10 _11 _13  - + -
    _30 _31 _33  + - +
    */
    _22 = (
        _00 * m8  -
        o01 * m10 +
        o03 * m12
    ) / det;    //+ _22

    /*
    _00 _01 _02  + - +
    _10 _11 _12  - + -
    _30 _31 _32  + - +
    */
    _23 = (
        o01 * m16 -
        _00 * m14 -
        o03 * m18
    ) / det;    //- _23

    /*
    _00 _02 _03  + - +
    _10 _12 _13  - + -
    _20 _22 _23  + - +
    */
    _31 = (
        _00 * m3 -
        o01 * m5 +
        o02 * m6
    ) / det;    //+ _31

    /*
    _00 _01 _03  + - +
    _10 _11 _13  - + -
    _20 _21 _23  + - +
    */
    _32 = (
        o01 * m11 -
        _00 * m9  -
        o02 * m12
    ) / det;    //- _32

    /*
    _00 _01 _02  + - +
    _10 _11 _12  - + -
    _20 _21 _22  + - +
    */
    _33 = (
        _00 * m15 -
        o01 * m17 +
        o02 * m18
    ) / det;    //+ _33

    /*
    _11 _12 _13  + - +
    _21 _22 _23  - + -
    _31 _32 _33  + - +
    */
    _00 = c00 / det;    //+ _00

    /*
    _01 _02 _03  + - +
    _21 _22 _23  - + -
    _31 _32 _33  + - +
    */
    _10 = c01 / det;    //- _10

    /*
    _01 _02 _03  + - +
    _11 _12 _13  - + -
    _31 _32 _33  + - +
    */
    _20 = c02 / det;    //+ _20

    /*
    _01 _02 _03  + - +
    _11 _12 _13  - + -
    _21 _22 _23  + - +
    */
    _30 = c03 / det;    //- _30
}

template< typename T >
template< typename T2 >
Matrix< T, 4, 4 >& Matrix< T, 4, 4 >::operator =( const Matrix< T2, 4, 4 >& right ) {
    _00 = static_cast<T>( right._00 );  _01 = static_cast<T>( right._01 );  _02 = static_cast<T>( right._02 );  _03 = static_cast<T>( right._03 );
    _10 = static_cast<T>( right._10 );  _11 = static_cast<T>( right._11 );  _12 = static_cast<T>( right._12 );  _13 = static_cast<T>( right._13 );
    _20 = static_cast<T>( right._20 );  _21 = static_cast<T>( right._21 );  _22 = static_cast<T>( right._22 );  _23 = static_cast<T>( right._23 );
    _30 = static_cast<T>( right._30 );  _31 = static_cast<T>( right._31 );  _32 = static_cast<T>( right._32 );  _33 = static_cast<T>( right._33 );

    return (*this);
}

template< typename T >
Matrix< T, 4, 4 >& Matrix< T, 4, 4 >::operator +=( const Matrix& right ) {
    _00 += right._00;   _01 += right._01;   _02 += right._02;   _03 += right._03;
    _10 += right._10;   _11 += right._11;   _12 += right._12;   _13 += right._13;
    _20 += right._20;   _21 += right._21;   _22 += right._22;   _23 += right._23;
    _30 += right._30;   _31 += right._31;   _32 += right._32;   _33 += right._33;

    return ( *this );
}

template< typename T >
Matrix< T, 4, 4 >& Matrix< T, 4, 4 >::operator -=( const Matrix& right ) {
    _00 -= right._00;   _01 -= right._01;   _02 -= right._02;   _03 -= right._03;
    _10 -= right._10;   _11 -= right._11;   _12 -= right._12;   _13 -= right._13;
    _20 -= right._20;   _21 -= right._21;   _22 -= right._22;   _23 -= right._23;
    _30 -= right._30;   _31 -= right._31;   _32 -= right._32;   _33 -= right._33;

    return ( *this );
}

template< typename T >
Matrix< T, 4, 4 >& Matrix< T, 4, 4 >::operator *=( const Matrix< T, 4, 4 >& right ) {
    /*
    When calculating the multiplied values for each cell on a row of the matrix,
    the original values from the previous cells on that row need to be preserved
    (because each calculation of a value on a row consists of a dot product involving the original row).
    After we've calculated all the values for a row, the previous row is never referenced again.
    Therefore an entirely seperate matrix isn't necessary; just one row.
    */
    T c0 = _00; T c1 = _01; T c2 = _02;

    _00 = c0 * right._00 + c1 * right._10 + c2 * right._20 + _03 * right._30;
    _01 = c0 * right._01 + c1 * right._11 + c2 * right._21 + _03 * right._31;
    _02 = c0 * right._02 + c1 * right._12 + c2 * right._22 + _03 * right._32;
    _03 = c0 * right._03 + c1 * right._13 + c2 * right._23 + _03 * right._33;


    c0 = _10; c1 = _11; c2 = _12;

    _10 = c0 * right._00 + c1 * right._10 + c2 * right._20 + _13 * right._30;
    _11 = c0 * right._01 + c1 * right._11 + c2 * right._21 + _13 * right._31;
    _12 = c0 * right._02 + c1 * right._12 + c2 * right._22 + _13 * right._32;
    _13 = c0 * right._03 + c1 * right._13 + c2 * right._23 + _13 * right._33;

    c0 = _20; c1 = _21; c2 = _22;

    _20 = c0 * right._00 + c1 * right._10 + c2 * right._20 + _23 * right._30;
    _21 = c0 * right._01 + c1 * right._11 + c2 * right._21 + _23 * right._31;
    _22 = c0 * right._02 + c1 * right._12 + c2 * right._22 + _23 * right._32;
    _23 = c0 * right._03 + c1 * right._13 + c2 * right._23 + _23 * right._33;

    c0 = _30; c1 = _31; c2 = _32;

    _30 = c0 * right._00 + c1 * right._10 + c2 * right._20 + _33 * right._30;
    _31 = c0 * right._01 + c1 * right._11 + c2 * right._21 + _33 * right._31;
    _32 = c0 * right._02 + c1 * right._12 + c2 * right._22 + _33 * right._32;
    _33 = c0 * right._03 + c1 * right._13 + c2 * right._23 + _33 * right._33;

    return ( *this );
}

template< typename T >
Matrix< T, 4, 4 >& Matrix< T, 4, 4 >::operator +=( const T right ) {
    _00 += right;   _01 += right;   _02 += right;   _03 += right;
    _10 += right;   _11 += right;   _12 += right;   _13 += right;
    _20 += right;   _21 += right;   _22 += right;   _23 += right;
    _30 += right;   _31 += right;   _32 += right;   _33 += right;

    return ( *this );
}

template< typename T >
Matrix< T, 4, 4 >& Matrix< T, 4, 4 >::operator -=( const T right ) {
    _00 -= right;   _01 -= right;   _02 -= right;   _03 -= right;
    _10 -= right;   _11 -= right;   _12 -= right;   _13 -= right;
    _20 -= right;   _21 -= right;   _22 -= right;   _23 -= right;
    _30 -= right;   _31 -= right;   _32 -= right;   _33 -= right;

    return ( *this );
}

template< typename T >
Matrix< T, 4, 4 >& Matrix< T, 4, 4 >::operator *=( const T right ) {
    _00 *= right;   _01 *= right;   _02 *= right;   _03 *= right;
    _10 *= right;   _11 *= right;   _12 *= right;   _13 *= right;
    _20 *= right;   _21 *= right;   _22 *= right;   _23 *= right;
    _30 *= right;   _31 *= right;   _32 *= right;   _33 *= right;

    return ( *this );
}

template< typename T >
Matrix< T, 4, 4 >& Matrix< T, 4, 4 >::operator /=( const T right ) {
    BS_ASSERT_NONZERO_DIVISOR( right );

    _00 /= right;   _01 /= right;   _02 /= right;   _03 /= right;
    _10 /= right;   _11 /= right;   _12 /= right;   _13 /= right;
    _20 /= right;   _21 /= right;   _22 /= right;   _23 /= right;
    _30 /= right;   _31 /= right;   _32 /= right;   _33 /= right;

    return ( *this );
}

template< typename T >
bool operator ==( const Matrix< T, 4, 4 >& left, const Matrix< T, 4, 4 >& right ) {
    return left._00 == right._00 &&     left._01 == right._01 &&    left._02 == right._02 &&    left._03 == right._03 &&
           left._10 == right._10 &&     left._11 == right._11 &&    left._12 == right._12 &&    left._13 == right._13 &&
           left._20 == right._20 &&     left._21 == right._21 &&    left._22 == right._22 &&    left._23 == right._23 &&
           left._30 == right._30 &&     left._31 == right._31 &&    left._32 == right._32 &&    left._33 == right._33;
}

template< typename T >
bool operator !=( const Matrix< T, 4, 4 >& left, const Matrix< T, 4, 4 >& right ) {
    return left._00 != right._00 ||     left._01 != right._01 ||    left._02 != right._02 ||    left._03 != right._03 ||
           left._10 != right._10 ||     left._11 != right._11 ||    left._12 != right._12 ||    left._13 != right._13 ||
           left._20 != right._20 ||     left._21 != right._21 ||    left._22 != right._22 ||    left._23 != right._23 ||
           left._30 != right._30 ||     left._31 != right._31 ||    left._32 != right._32 ||    left._33 != right._33;
}

template< typename T >
Matrix< T, 4, 4 > operator +( const Matrix< T, 4, 4 >& right ) {
    return right;
}

template< typename T >
Matrix< T, 4, 4 > operator -( const Matrix< T, 4, 4 >& right ) {
    return Matrix< T, 4, 4 >(
        -right._00,     -right._01,     -right._02,     -right._03,
        -right._10,     -right._11,     -right._12,     -right._13,
        -right._20,     -right._21,     -right._22,     -right._23,
        -right._30,     -right._31,     -right._32,     -right._33
    );
}

template< typename T >
Matrix< T, 4, 4 > operator +( const Matrix< T, 4, 4 >& left, const Matrix< T, 4, 4 >& right ) {
    return Matrix< T, 4, 4 >(
        left._00 + right._00,   left._01 + right._01,   left._02 + right._02,   left._03 + right._03,
        left._10 + right._10,   left._11 + right._11,   left._12 + right._12,   left._13 + right._13,
        left._20 + right._20,   left._21 + right._21,   left._22 + right._22,   left._23 + right._23,
        left._30 + right._30,   left._31 + right._31,   left._32 + right._32,   left._33 + right._33
    );
}

template< typename T >
Matrix< T, 4, 4 > operator -( const Matrix< T, 4, 4 >& left, const Matrix< T, 4, 4 >& right ) {
    return Matrix< T, 4, 4 >(
        left._00 - right._00,   left._01 - right._01,   left._02 - right._02,   left._03 - right._03,
        left._10 - right._10,   left._11 - right._11,   left._12 - right._12,   left._13 - right._13,
        left._20 - right._20,   left._21 - right._21,   left._22 - right._22,   left._23 - right._23,
        left._30 - right._30,   left._31 - right._31,   left._32 - right._32,   left._33 - right._33
    );
}

template< typename T >
Matrix< T, 4, 4 > operator *( const Matrix< T, 4, 4 >& left, const Matrix< T, 4, 4 >& right ) {
    return Matrix< T, 4, 4 >(
        //First row
        left._00 * right._00 + left._01 * right._10 + left._02 * right._20 + left._03 * right._30,
        left._00 * right._01 + left._01 * right._11 + left._02 * right._21 + left._03 * right._31,
        left._00 * right._02 + left._01 * right._12 + left._02 * right._22 + left._03 * right._32,
        left._00 * right._03 + left._01 * right._13 + left._02 * right._23 + left._03 * right._33,

        //Second row
        left._10 * right._00 + left._11 * right._10 + left._12 * right._20 + left._13 * right._30,
        left._10 * right._01 + left._11 * right._11 + left._12 * right._21 + left._13 * right._31,
        left._10 * right._02 + left._11 * right._12 + left._12 * right._22 + left._13 * right._32,
        left._10 * right._03 + left._11 * right._13 + left._12 * right._23 + left._13 * right._33,

        //Third row
        left._20 * right._00 + left._21 * right._10 + left._22 * right._20 + left._23 * right._30,
        left._20 * right._01 + left._21 * right._11 + left._22 * right._21 + left._23 * right._31,
        left._20 * right._02 + left._21 * right._12 + left._22 * right._22 + left._23 * right._32,
        left._20 * right._03 + left._21 * right._13 + left._22 * right._23 + left._23 * right._33,

        //Fourth row
        left._30 * right._00 + left._31 * right._10 + left._32 * right._20 + left._33 * right._30,
        left._30 * right._01 + left._31 * right._11 + left._32 * right._21 + left._33 * right._31,
        left._30 * right._02 + left._31 * right._12 + left._32 * right._22 + left._33 * right._32,
        left._30 * right._03 + left._31 * right._13 + left._32 * right._23 + left._33 * right._33
    );
}

template< typename T, size_t R2 >
Matrix< T, R2, 4 > operator *( const Matrix< T, R2, 4 >& left, const Matrix< T, 4, 4 >& right ) {
    Matrix< T, R2, 4 > out;

    for( size_t r2 = 0; r2 < R2; ++r2 ) {
        //Set 1st column of output matrix
        out.elem[r2][0] = left.elem[r2][0] * right._00 +
                          left.elem[r2][1] * right._10 +
                          left.elem[r2][2] * right._20 +
                          left.elem[r2][3] * right._30;

        //Set 2nd column of output matrix
        out.elem[r2][1] = left.elem[r2][0] * right._01 +
                          left.elem[r2][1] * right._11 +
                          left.elem[r2][2] * right._21 +
                          left.elem[r2][3] * right._31;

        //Set 3rd column of output matrix
        out.elem[r2][2] = left.elem[r2][0] * right._02 +
                          left.elem[r2][1] * right._12 +
                          left.elem[r2][2] * right._22 +
                          left.elem[r2][3] * right._32;

        //Set 4th column of output matrix
        out.elem[r2][3] = left.elem[r2][0] * right._03 +
                          left.elem[r2][1] * right._13 +
                          left.elem[r2][2] * right._23 +
                          left.elem[r2][3] * right._33;
    }

    return out;
}

template< typename T, size_t C2 >
Matrix< T, 4, C2 > operator *( const Matrix< T, 4, 4 >& left, const Matrix< T, 4, C2 >& right ) {
    Matrix< T, 4, C2 > out;

    //Set 1st row of output matrix
    for( size_t c2 = 0; c2 < C2; ++c2 ) {
        out.elem[0][c2] = left._00 * right.elem[0][c2] +
                          left._01 * right.elem[1][c2] +
                          left._02 * right.elem[2][c2] +
                          left._03 * right.elem[3][c2];
    }

    //Set 2nd row of output matrix
    for( size_t c2 = 0; c2 < C2; ++c2 ) {
        out.elem[1][c2] = left._10 * right.elem[0][c2] +
                          left._11 * right.elem[1][c2] +
                          left._12 * right.elem[2][c2] +
                          left._13 * right.elem[3][c2];
    }

    //Set 3rd row of output matrix
    for( size_t c2 = 0; c2 < C2; ++c2 ) {
        out.elem[2][c2] = left._20 * right.elem[0][c2] +
                          left._21 * right.elem[1][c2] +
                          left._22 * right.elem[2][c2] +
                          left._23 * right.elem[3][c2];
    }

    //Set 4th row of output matrix
    for( size_t c2 = 0; c2 < C2; ++c2 ) {
        out.elem[3][c2] = left._30 * right.elem[0][c2] +
                          left._31 * right.elem[1][c2] +
                          left._32 * right.elem[2][c2] +
                          left._33 * right.elem[3][c2];
    }

    return out;
}

//4D-vector (as a 1x4 matrix) * 4x4 matrix = 4D-vector (as a 1x4 matrix)
template< typename T >
Vector< T, 4 > operator *( const Vector< T, 4 >& left, const Matrix< T, 4, 4 >& right ) {
    return Vector< T, 4 >(
        left.x * right._00 + left.y * right._10 + left.z * right._20 + left.w * right._30,
        left.x * right._01 + left.y * right._11 + left.z * right._21 + left.w * right._31,
        left.x * right._02 + left.y * right._12 + left.z * right._22 + left.w * right._32,
        left.x * right._03 + left.y * right._13 + left.z * right._23 + left.w * right._33
    );
}

//4x4 matrix * 4D-vector (as a 4x1 matrix) = 4D-vector (as a 4x1 matrix)
template< typename T >
Vector< T, 4 > operator *( const Matrix< T, 4, 4 >& left, const Vector< T, 4 >& right ) {
    return Vector< T, 4 >(
        left._00 * right.x + left._01 * right.y + left._02 * right.z + left._03 * right.w,
        left._10 * right.x + left._11 * right.y + left._12 * right.z + left._13 * right.w,
        left._20 * right.x + left._21 * right.y + left._22 * right.z + left._23 * right.w,
        left._30 * right.x + left._31 * right.y + left._32 * right.z + left._33 * right.w
    );
}

//4D-vector (as a 1x4 matrix) * 4x4 matrix = 4D-vector (as a 1x4 matrix)
template< typename T >
Vector< T, 4 >& operator *=( Vector< T, 4 >& leftInOut, const Matrix< T, 4, 4 >& right ) {
    T cx = leftInOut.x;
    T cy = leftInOut.y;
    T cz = leftInOut.z;

    leftInOut.x = cx * right._00 + cy * right._10 + cz * right._20 + leftInOut.w * right._30;
    leftInOut.y = cx * right._01 + cy * right._11 + cz * right._21 + leftInOut.w * right._31;
    leftInOut.z = cx * right._02 + cy * right._12 + cz * right._22 + leftInOut.w * right._32;
    leftInOut.w = cx * right._03 + cy * right._13 + cz * right._23 + leftInOut.w * right._33;

    return leftInOut;
}

template< typename T >
Matrix< T, 4, 4 > operator +( const T left, const Matrix< T, 4, 4 >& right ) {
    return Matrix< T, 4, 4 >(
        left + right._00,   left + right._01,   left + right._02,   left + right._03,
        left + right._10,   left + right._11,   left + right._12,   left + right._13,
        left + right._20,   left + right._21,   left + right._22,   left + right._23,
        left + right._30,   left + right._31,   left + right._32,   left + right._33
    );
}

template< typename T >
Matrix< T, 4, 4 > operator +( const Matrix< T, 4, 4 >& left, const T right ) {
    return Matrix< T, 4, 4 >(
        left._00 + right,   left._01 + right,   left._02 + right,   left._03 + right,
        left._10 + right,   left._11 + right,   left._12 + right,   left._13 + right,
        left._20 + right,   left._21 + right,   left._22 + right,   left._23 + right,
        left._30 + right,   left._31 + right,   left._32 + right,   left._33 + right
    );
}

template< typename T >
Matrix< T, 4, 4 > operator -( const Matrix< T, 4, 4 >& left, const T right ) {
    return Matrix< T, 4, 4 >(
        left._00 - right,   left._01 - right,   left._02 - right,   left._03 - right,
        left._10 - right,   left._11 - right,   left._12 - right,   left._13 - right,
        left._20 - right,   left._21 - right,   left._22 - right,   left._23 - right,
        left._30 - right,   left._31 - right,   left._32 - right,   left._33 - right
    );
}

template< typename T >
Matrix< T, 4, 4 > operator *( const T left, const Matrix< T, 4, 4 >& right ) {
    return Matrix< T, 4, 4 >(
        left * right._00,   left * right._01,   left * right._02,   left * right._03,
        left * right._10,   left * right._11,   left * right._12,   left * right._13,
        left * right._20,   left * right._21,   left * right._22,   left * right._23,
        left * right._30,   left * right._31,   left * right._32,   left * right._33
    );
}

template< typename T >
Matrix< T, 4, 4 > operator *( const Matrix< T, 4, 4 >& left, const T right ) {
    return Matrix< T, 4, 4 >(
        left._00 * right,   left._01 * right,   left._02 * right,   left._03 * right,
        left._10 * right,   left._11 * right,   left._12 * right,   left._13 * right,
        left._20 * right,   left._21 * right,   left._22 * right,   left._23 * right,
        left._30 * right,   left._31 * right,   left._32 * right,   left._33 * right
    );
}

template< typename T >
Matrix< T, 4, 4 > operator /( const Matrix< T, 4, 4 >& left, const T right ) {
    BS_ASSERT_NONZERO_DIVISOR( right );

    return Matrix< T, 4, 4 >(
        left._00 / right,   left._01 / right,   left._02 / right,   left._03 / right,
        left._10 / right,   left._11 / right,   left._12 / right,   left._13 / right,
        left._20 / right,   left._21 / right,   left._22 / right,   left._23 / right,
        left._30 / right,   left._31 / right,   left._32 / right,   left._33 / right
    );
}

template< typename T >
Matrix< T, 4, 4 > transpose( const Matrix< T, 4, 4 >& matrix ) {
    return Matrix< T, 4, 4 >(
        matrix._00, matrix._10, matrix._20, matrix._30,
        matrix._01, matrix._11, matrix._21, matrix._31,
        matrix._02, matrix._12, matrix._22, matrix._32,
        matrix._03, matrix._13, matrix._23, matrix._33
    );
}

template< typename T >
Matrix< T, 4, 4 > invert( const Matrix< T, 4, 4 >& matrix ) {
    T m1 = matrix._22 * matrix._33 - matrix._23 * matrix._32;
    T m2 = matrix._21 * matrix._33 - matrix._23 * matrix._31;
    T m3 = matrix._21 * matrix._32 - matrix._22 * matrix._31;
    T m4 = matrix._20 * matrix._33 - matrix._23 * matrix._30;
    T m5 = matrix._20 * matrix._32 - matrix._22 * matrix._30;
    T m6 = matrix._20 * matrix._31 - matrix._21 * matrix._30;

    T c00 = matrix._11 * m1 - matrix._12 * m2 + matrix._13 * m3;
    T c01 = matrix._12 * m4 - matrix._13 * m5 - matrix._10 * m1;
    T c02 = matrix._10 * m2 - matrix._11 * m4 + matrix._13 * m6;
    T c03 = matrix._11 * m5 - matrix._12 * m6 - matrix._10 * m3;

    T det = matrix._00 * c00 +
            matrix._01 * c01 +
            matrix._02 * c02 +
            matrix._03 * c03;

    BS_ASSERT_NONZERO_DIVISOR( det );

    T m7  = matrix._12 * matrix._33 - matrix._13 * matrix._32;
    T m8  = matrix._11 * matrix._33 - matrix._13 * matrix._31;
    T m9  = matrix._11 * matrix._32 - matrix._12 * matrix._31;
    T m10 = matrix._10 * matrix._33 - matrix._13 * matrix._30;
    T m11 = matrix._10 * matrix._32 - matrix._12 * matrix._30;
    T m12 = matrix._10 * matrix._31 - matrix._11 * matrix._30;
    T m13 = matrix._12 * matrix._23 - matrix._13 * matrix._22;
    T m14 = matrix._11 * matrix._23 - matrix._13 * matrix._21;
    T m15 = matrix._11 * matrix._22 - matrix._12 * matrix._21;
    T m16 = matrix._10 * matrix._23 - matrix._13 * matrix._20;
    T m17 = matrix._10 * matrix._22 - matrix._12 * matrix._20;
    T m18 = matrix._10 * matrix._21 - matrix._11 * matrix._20;

    return Matrix< T, 4, 4 >(
        c00                                                        / det,
        ( matrix._02 * m2  - matrix._01 * m1  - matrix._03 * m3  ) / det,
        ( matrix._01 * m7  - matrix._02 * m8  + matrix._03 * m9  ) / det,
        ( matrix._02 * m14 - matrix._01 * m13 - matrix._03 * m15 ) / det,

        c01                                                        / det,
        ( matrix._00 * m1  - matrix._02 * m4  + matrix._03 * m5  ) / det,
        ( matrix._02 * m10 - matrix._00 * m7  - matrix._03 * m11 ) / det,
        ( matrix._00 * m13 - matrix._02 * m16 + matrix._03 * m17 ) / det,

        c02                                                        / det,
        ( matrix._01 * m4  - matrix._00 * m2  - matrix._03 * m6  ) / det,
        ( matrix._00 * m8  - matrix._01 * m10 + matrix._03 * m12 ) / det,
        ( matrix._01 * m16 - matrix._00 * m14 - matrix._03 * m18 ) / det,

        c03                                                        / det,
        ( matrix._00 * m3  - matrix._01 * m5  + matrix._02 * m6  ) / det,
        ( matrix._01 * m11 - matrix._00 * m9  - matrix._02 * m12 ) / det,
        ( matrix._00 * m15 - matrix._01 * m17 + matrix._02 * m18 ) / det
    );
}

template< typename T >
Matrix< T, 4, 4 >::Matrix(
    const T _00, const T _01, const T _02, const T _03,
    const T _10, const T _11, const T _12, const T _13,
    const T _20, const T _21, const T _22, const T _23,
    const T _30, const T _31, const T _32, const T _33
) :
    _00( _00 ),     _01( _01 ),     _02( _02 ),     _03( _03 ),
    _10( _10 ),     _11( _11 ),     _12( _12 ),     _13( _13 ),
    _20( _20 ),     _21( _21 ),     _22( _22 ),     _23( _23 ),
    _30( _30 ),     _31( _31 ),     _32( _32 ),     _33( _33 )
{
}

template< typename T >
void Matrix< T, 4, 4 >::set(
    const T _00, const T _01, const T _02, const T _03,
    const T _10, const T _11, const T _12, const T _13,
    const T _20, const T _21, const T _22, const T _23,
    const T _30, const T _31, const T _32, const T _33
) {
    Matrix::_00 = _00;  Matrix::_01 = _01;  Matrix::_02 = _02;  Matrix::_03 = _03;
    Matrix::_10 = _10;  Matrix::_11 = _11;  Matrix::_12 = _12;  Matrix::_13 = _13;
    Matrix::_20 = _20;  Matrix::_21 = _21;  Matrix::_22 = _22;  Matrix::_23 = _23;
    Matrix::_30 = _30;  Matrix::_31 = _31;  Matrix::_32 = _32;  Matrix::_33 = _33;
}

template< typename T >
void Matrix< T, 4, 4 >::get(
    T& _00Out, T& _01Out, T& _02Out, T& _03Out,
    T& _10Out, T& _11Out, T& _12Out, T& _13Out,
    T& _20Out, T& _21Out, T& _22Out, T& _23Out,
    T& _30Out, T& _31Out, T& _32Out, T& _33Out
) const {
    _00Out = _00;   _01Out = _01;   _02Out = _02;   _03Out = _03;
    _10Out = _10;   _11Out = _11;   _12Out = _12;   _13Out = _13;
    _20Out = _20;   _21Out = _21;   _22Out = _22;   _23Out = _23;
    _30Out = _30;   _31Out = _31;   _32Out = _32;   _33Out = _33;
}

//Typedefs
template< typename T >
using Matrix4x4 = Matrix< T, 4, 4 >;
typedef Matrix4x4< int32  > Matrix4x4i;
typedef Matrix4x4< float  > Matrix4x4f;
typedef Matrix4x4< double > Matrix4x4d;

}




#endif //BS_MATRIX_MATRIX4X4_HPP
