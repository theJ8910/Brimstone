/*
matrix/Matrix3x3.hpp
--------------------
Copyright (c) 2024, theJ89

Description:
    Specialization of Matrix for 3x3 matrices.
    Adds the following typedefs for convenience:
        Matrix3x3< T >: Matrix<T,3,3>
        Matrix3x3i:     Matrix<int32,3,3>
        Matrix3x3f:     Matrix<float,3,3>
        Matrix3x3d:     Matrix<double,3,3>
*/
#ifndef BS_MATRIX_MATRIX3X3_HPP
#define BS_MATRIX_MATRIX3X3_HPP




//Includes
#include <cstddef>                          //std::size_t
#include <utility>                          //std::swap

#include <brimstone/matrix/MatrixNxN.hpp>   //Matrix, BS_MATRIX_SQUARE_DECLARE_METHODS




namespace Brimstone {

template< typename T >
class Matrix< T, 3, 3 > {
public:
//C4201: nonstandard extension used : nameless struct/union
//It's a non-standard feature, but VC++, G++, and LLVM support it so it shouldn't be too much of an issue
#pragma warning( push )
#pragma warning( disable: 4201 )

    union {
        T              data[ 9 ];
        T              elem[3][3];
        struct {
            Vector< T, 3 > row[ 3 ];
        };
        struct {
            T _00, _01, _02,
              _10, _11, _12,
              _20, _21, _22;
        };
    };

#pragma warning( pop )
public:
    BS_ARRAY_DECLARE_INHERITED_METHODS( Matrix, T )
    BS_ARRAY_DECLARE_METHODS( Matrix, T )
    BS_MATRIX_DECLARE_METHODS( 3, 3 )
    BS_MATRIX_SQUARE_DECLARE_METHODS( 3 )

    Matrix(
        const T _00, const T _01, const T _02,
        const T _10, const T _11, const T _12,
        const T _20, const T _21, const T _22
    );

    void set(
        const T _00, const T _01, const T _02,
        const T _10, const T _11, const T _12,
        const T _20, const T _21, const T _22
    );

    void get(
        T& _00Out, T& _01Out, T& _02Out,
        T& _10Out, T& _11Out, T& _12Out,
        T& _20Out, T& _21Out, T& _22Out
    ) const;
private:
    static const Matrix m_identity;
};

BS_ARRAY_DEFINE_METHODS( Matrix, T, data, BS_TMPL_1( typename T ), BS_SPEC_3( T, 3, 3 ) )
BS_MATRIX_DEFINE_METHODS( 3, 3, BS_TMPL_1( typename T ), BS_SPEC_3( T, 3, 3 ) )
BS_MATRIX_SQUARE_DEFINE_METHODS( 3, BS_TMPL_1( typename T ), BS_SPEC_3( T, 3, 3 ) )




//Forward declarations
template< typename T >
Matrix< T, 4, 4 > invert( const Matrix< T, 4, 4 >& matrix );
template< typename T >
Matrix< T, 4, 4 > transpose( const Matrix< T, 4, 4 >& matrix );




template< typename T >
const Matrix< T, 3, 3 > Matrix< T, 3, 3 >::m_identity = Matrix< T, 3, 3 >(
    static_cast< T >( 1 ),  static_cast< T >( 0 ),  static_cast< T >( 0 ),
    static_cast< T >( 0 ),  static_cast< T >( 1 ),  static_cast< T >( 0 ),
    static_cast< T >( 0 ),  static_cast< T >( 0 ),  static_cast< T >( 1 )
);

template< typename T >
inline Matrix< T, 3, 3 >::Matrix( const T& elem ) :
    _00( elem ),    _01( elem ),    _02( elem ),
    _10( elem ),    _11( elem ),    _12( elem ),
    _20( elem ),    _21( elem ),    _22( elem ) {
}

template< typename T >
template< typename T2 >
inline Matrix< T, 3, 3 >::Matrix( const T2& cppRange ) :
    _00( cppRange[0] ),     _01( cppRange[1] ),     _02( cppRange[2] ),
    _10( cppRange[3] ),     _11( cppRange[4] ),     _12( cppRange[5] ),
    _20( cppRange[6] ),     _21( cppRange[7] ),     _22( cppRange[8] ) {
    BS_ASSERT_SIZE( rangeSize( cppRange ), 9 );
}

template< typename T >
inline Matrix< T, 3, 3 >::Matrix( std::initializer_list< T > il ) :
    _00( *( std::begin( il )     ) ),
    _01( *( std::begin( il ) + 1 ) ),
    _02( *( std::begin( il ) + 2 ) ),
    _10( *( std::begin( il ) + 3 ) ),
    _11( *( std::begin( il ) + 4 ) ),
    _12( *( std::begin( il ) + 5 ) ),
    _20( *( std::begin( il ) + 6 ) ),
    _21( *( std::begin( il ) + 7 ) ),
    _22( *( std::begin( il ) + 8 ) ) {
    BS_ASSERT_SIZE( rangeSize( il ), 9 );
}

template< typename T >
template< typename T2 >
inline void Matrix< T, 3, 3 >::set( const T2& cppRange ) {
    BS_ASSERT_SIZE( rangeSize( cppRange ), 9 );

    _00 = cppRange[0];  _01 = cppRange[1];  _02 = cppRange[2];
    _10 = cppRange[3];  _11 = cppRange[4];  _12 = cppRange[5];
    _20 = cppRange[6];  _21 = cppRange[7];  _22 = cppRange[8];
}
template< typename T >
inline void Matrix< T, 3, 3 >::set( std::initializer_list< T > il ) {
    BS_ASSERT_SIZE( rangeSize( il ), 9 );

    auto it = std::begin( il );
    _00 = *( it      );
    _01 = *( it +  1 );
    _02 = *( it +  2 );
    _10 = *( it +  3 );
    _11 = *( it +  4 );
    _12 = *( it +  5 );
    _20 = *( it +  6 );
    _21 = *( it +  7 );
    _22 = *( it +  8 );
}
template< typename T >
template< typename T2 >
inline void Matrix< T, 3, 3 >::get( T2& cppRangeOut ) const {
    BS_ASSERT_SIZE( rangeSize( cppRangeOut ), 9 );

    cppRangeOut[0] = _00;   cppRangeOut[1] = _01;   cppRangeOut[2] = _02;
    cppRangeOut[3] = _10;   cppRangeOut[4] = _11;   cppRangeOut[5] = _12;
    cppRangeOut[6] = _20;   cppRangeOut[7] = _21;   cppRangeOut[8] = _22;
}

template< typename T >
inline void Matrix< T, 3, 3 >::fill( const T& elem ) {
    _00 = elem;     _01 = elem;     _02 = elem;
    _10 = elem;     _11 = elem;     _12 = elem;
    _20 = elem;     _21 = elem;     _22 = elem;
}

template< typename T >
Matrix< T, 3, 3 >::Matrix()
#ifdef BS_ZERO
    : Matrix( m_identity )
#endif //BS_ZERO
{
}

template< typename T >
template< typename T2 >
Matrix< T, 3, 3 >::Matrix( const Matrix< T2, 3, 3 >& toCopy ) :
    _00( static_cast< T >( toCopy._00 ) ),  _01( static_cast< T >( toCopy._01 ) ),  _02( static_cast< T >( toCopy._02 ) ),
    _10( static_cast< T >( toCopy._10 ) ),  _11( static_cast< T >( toCopy._11 ) ),  _12( static_cast< T >( toCopy._12 ) ),
    _20( static_cast< T >( toCopy._20 ) ),  _21( static_cast< T >( toCopy._21 ) ),  _22( static_cast< T >( toCopy._22 ) ) {
}

template< typename T >
bool Matrix< T, 3, 3 >::isSquare() const {
    return true;
}

template< typename T >
void Matrix< T, 3, 3 >::zero() {
    T zero = static_cast< T >( 0 );
    _00 = zero;     _01 = zero;     _02 = zero;
    _10 = zero;     _11 = zero;     _12 = zero;
    _20 = zero;     _21 = zero;     _22 = zero;
}

template< typename T >
bool Matrix< T, 3, 3 >::isZero() const {
    T zero = static_cast< T >( 0 );
    return _00 == zero &&   _01 == zero &&  _02 == zero &&
           _10 == zero &&   _11 == zero &&  _12 == zero &&
           _20 == zero &&   _21 == zero &&  _22 == zero;
}

template< typename T >
void Matrix< T, 3, 3 >::setRow( const std::size_t row, const Vector< T, 3 >& values ) {
    BS_ASSERT_INDEX( row, 2 );

    elem[row][0] = values.data[0];
    elem[row][1] = values.data[1];
    elem[row][2] = values.data[2];
}

template< typename T >
void Matrix< T, 3, 3 >::setColumn( const std::size_t col, const Vector< T, 3 >& values ) {
    BS_ASSERT_INDEX( col, 2 );

    elem[0][col] = values.data[0];
    elem[1][col] = values.data[1];
    elem[2][col] = values.data[2];
}

template< typename T >
Vector< T, 3 > Matrix< T, 3, 3 >::getRow( const std::size_t row ) const {
    BS_ASSERT_INDEX( row, 2 );

    return Vector< T, 3 >(
        elem[row][0],
        elem[row][1],
        elem[row][2]
    );
}

template< typename T >
Vector< T, 3 > Matrix< T, 3, 3 >::getColumn( const std::size_t col ) const {
    BS_ASSERT_INDEX( col, 2 );

    return Vector< T, 3 >(
        elem[0][col],
        elem[1][col],
        elem[2][col]
    );
}

template< typename T >
void Matrix< T, 3, 3 >::setDiagonal( const Vector< T, 3 >& values ) {
    _00 = values.x;
    _11 = values.y;
    _22 = values.z;
}

template< typename T >
Vector< T, 3 > Matrix< T, 3, 3 >::getDiagonal() const {
    return Vector< T, 3 >( _00, _11, _22 );
}

template< typename T >
T Matrix< T, 3, 3 >::getDeterminant() const {
    return _00 * ( _11 * _22 - _12 * _21 ) -
           _01 * ( _10 * _22 - _12 * _20 ) +
           _02 * ( _10 * _21 - _11 * _20 );
}

template< typename T >
bool Matrix< T, 3, 3 >::isLowerTriangular() const {
    T zero = static_cast< T >( 0 );

    return _01 == zero && _02 == zero &&
                          _12 == zero;
}

template< typename T >
bool Matrix< T, 3, 3 >::isUpperTriangular() const {
    T zero = static_cast< T >( 0 );

    return _10 == zero &&
           _20 == zero && _21 == zero;
}

template< typename T >
T Matrix< T, 3, 3 >::getTrace() const {
    return _00 + _11 + _22;
}

template< typename T >
void Matrix< T, 3, 3 >::transpose() {
    std::swap( _01, _10 );
    std::swap( _02, _20 );

    std::swap( _12, _21 );
}

template< typename T >
void Matrix< T, 3, 3 >::invert() {
    //The cofactors for the first row are used both to calculate the determinant
    //and the inverse itself, so they are calculated ahead of time here to be reused later
    T c00 = _11 * _22 - _12 * _21;
    T c01 = _12 * _20 - _10 * _22;
    T c02 = _10 * _21 - _11 * _20;

    T det = _00 * c00 +
            _01 * c01 +
            _02 * c02;

    BS_ASSERT_NONZERO_DIVISOR( det );

    //Old values for certain elements need to be stored here
    //because the old values are needed after the assign statements
    //for those values
    T o01 = _01; T o02 = _02;
    T o11 = _11;

    _01 = ( o02 * _21 - o01 * _22 ) / det;  //_01
    _02 = ( o01 * _12 - o02 * o11 ) / det;  //_02

    _11 = ( _00 * _22 - o02 * _20 ) / det;  //_11
    _12 = ( o02 * _10 - _00 * _12 ) / det;  //_12

    _21 = ( o01 * _20 - _00 * _21 ) / det;  //_21
    _22 = ( _00 * o11 - o01 * _10 ) / det;  //_22

    _00 = c00                       / det;  //_00
    _10 = c01                       / det;  //_10
    _20 = c02                       / det;  //_20
}

template< typename T >
template< typename T2 >
Matrix< T, 3, 3 >& Matrix< T, 3, 3 >::operator =( const Matrix< T2, 3, 3 >& right ) {
    _00 = static_cast<T>( right._00 );  _01 = static_cast<T>( right._01 );  _02 = static_cast<T>( right._02 );
    _10 = static_cast<T>( right._10 );  _11 = static_cast<T>( right._11 );  _12 = static_cast<T>( right._12 );
    _20 = static_cast<T>( right._20 );  _21 = static_cast<T>( right._21 );  _22 = static_cast<T>( right._22 );

    return (*this);
}

template< typename T >
Matrix< T, 3, 3 >& Matrix< T, 3, 3 >::operator +=( const Matrix& right ) {
    _00 += right._00;   _01 += right._01;   _02 += right._02;
    _10 += right._10;   _11 += right._11;   _12 += right._12;
    _20 += right._20;   _21 += right._21;   _22 += right._22;

    return ( *this );
}

template< typename T >
Matrix< T, 3, 3 >& Matrix< T, 3, 3 >::operator -=( const Matrix& right ) {
    _00 -= right._00;   _01 -= right._01;   _02 -= right._02;
    _10 -= right._10;   _11 -= right._11;   _12 -= right._12;
    _20 -= right._20;   _21 -= right._21;   _22 -= right._22;

    return ( *this );
}

template< typename T >
Matrix< T, 3, 3 >& Matrix< T, 3, 3 >::operator *=( const Matrix< T, 3, 3 >& right ) {
    /*
    When calculating the multiplied values for each cell on a row of the matrix,
    the original values from the previous cells on that row need to be preserved
    (because each calculation of a value on a row consists of a dot product involving the original row).
    After we've calculated all the values for a row, the previous row is never referenced again.
    Therefore an entirely seperate matrix isn't necessary; just one row.
    */
    T c0 = _00; T c1 = _01;

    _00 = c0 * right._00 + c1 * right._10 + _02 * right._20;
    _01 = c0 * right._01 + c1 * right._11 + _02 * right._21;
    _02 = c0 * right._02 + c1 * right._12 + _02 * right._22;


    c0 = _10; c1 = _11;

    _10 = c0 * right._00 + c1 * right._10 + _12 * right._20;
    _11 = c0 * right._01 + c1 * right._11 + _12 * right._21;
    _12 = c0 * right._02 + c1 * right._12 + _12 * right._22;

    c0 = _20; c1 = _21;

    _20 = c0 * right._00 + c1 * right._10 + _22 * right._20;
    _21 = c0 * right._01 + c1 * right._11 + _22 * right._21;
    _22 = c0 * right._02 + c1 * right._12 + _22 * right._22;

    return ( *this );
}

template< typename T >
Matrix< T, 3, 3 >& Matrix< T, 3, 3 >::operator +=( const T right ) {
    _00 += right;   _01 += right;   _02 += right;
    _10 += right;   _11 += right;   _12 += right;
    _20 += right;   _21 += right;   _22 += right;

    return ( *this );
}

template< typename T >
Matrix< T, 3, 3 >& Matrix< T, 3, 3 >::operator -=( const T right ) {
    _00 -= right;   _01 -= right;   _02 -= right;
    _10 -= right;   _11 -= right;   _12 -= right;
    _20 -= right;   _21 -= right;   _22 -= right;

    return ( *this );
}

template< typename T >
Matrix< T, 3, 3 >& Matrix< T, 3, 3 >::operator *=( const T right ) {
    _00 *= right;   _01 *= right;   _02 *= right;
    _10 *= right;   _11 *= right;   _12 *= right;
    _20 *= right;   _21 *= right;   _22 *= right;

    return ( *this );
}

template< typename T >
Matrix< T, 3, 3 >& Matrix< T, 3, 3 >::operator /=( const T right ) {
    BS_ASSERT_NONZERO_DIVISOR( right );

    _00 /= right;   _01 /= right;   _02 /= right;
    _10 /= right;   _11 /= right;   _12 /= right;
    _20 /= right;   _21 /= right;   _22 /= right;

    return ( *this );
}

template< typename T >
bool operator ==( const Matrix< T, 3, 3 >& left, const Matrix< T, 3, 3 >& right ) {
    return left._00 == right._00 &&     left._01 == right._01 &&    left._02 == right._02 &&
           left._10 == right._10 &&     left._11 == right._11 &&    left._12 == right._12 &&
           left._20 == right._20 &&     left._21 == right._21 &&    left._22 == right._22;
}

template< typename T >
bool operator !=( const Matrix< T, 3, 3 >& left, const Matrix< T, 3, 3 >& right ) {
    return left._00 != right._00 ||     left._01 != right._01 ||    left._02 != right._02 ||
           left._10 != right._10 ||     left._11 != right._11 ||    left._12 != right._12 ||
           left._20 != right._20 ||     left._21 != right._21 ||    left._22 != right._22;
}

template< typename T >
Matrix< T, 3, 3 > operator +( const Matrix< T, 3, 3 >& right ) {
    return right;
}

template< typename T >
Matrix< T, 3, 3 > operator -( const Matrix< T, 3, 3 >& right ) {
    return Matrix< T, 3, 3 >(
        -right._00,     -right._01,     -right._02,
        -right._10,     -right._11,     -right._12,
        -right._20,     -right._21,     -right._22
    );
}

template< typename T >
Matrix< T, 3, 3 > operator +( const Matrix< T, 3, 3 >& left, const Matrix< T, 3, 3 >& right ) {
    return Matrix< T, 3, 3 >(
        left._00 + right._00,   left._01 + right._01,   left._02 + right._02,
        left._10 + right._10,   left._11 + right._11,   left._12 + right._12,
        left._20 + right._20,   left._21 + right._21,   left._22 + right._22
    );
}

template< typename T >
Matrix< T, 3, 3 > operator -( const Matrix< T, 3, 3 >& left, const Matrix< T, 3, 3 >& right ) {
    return Matrix< T, 3, 3 >(
        left._00 - right._00,   left._01 - right._01,   left._02 - right._02,
        left._10 - right._10,   left._11 - right._11,   left._12 - right._12,
        left._20 - right._20,   left._21 - right._21,   left._22 - right._22
    );
}

template< typename T >
Matrix< T, 3, 3 > operator *( const Matrix< T, 3, 3 >& left, const Matrix< T, 3, 3 >& right ) {
    return Matrix< T, 3, 3 >(
        //First row
        left._00 * right._00 + left._01 * right._10 + left._02 * right._20,
        left._00 * right._01 + left._01 * right._11 + left._02 * right._21,
        left._00 * right._02 + left._01 * right._12 + left._02 * right._22,

        //Second row
        left._10 * right._00 + left._11 * right._10 + left._12 * right._20,
        left._10 * right._01 + left._11 * right._11 + left._12 * right._21,
        left._10 * right._02 + left._11 * right._12 + left._12 * right._22,

        //Third row
        left._20 * right._00 + left._21 * right._10 + left._22 * right._20,
        left._20 * right._01 + left._21 * right._11 + left._22 * right._21,
        left._20 * right._02 + left._21 * right._12 + left._22 * right._22
    );
}

template< typename T, std::size_t C2 >
Matrix< T, 3, C2 > operator *( const Matrix< T, 3, 3 >& left, const Matrix< T, 3, C2 >& right ) {
    Matrix< T, 3, C2 > out;

    //Set 1st row of output matrix
    for( std::size_t c2 = 0; c2 < C2; ++c2 ) {
        out.elem[0][c2] = left._00 * right.elem[0][c2] +
                          left._01 * right.elem[1][c2] +
                          left._02 * right.elem[2][c2];
    }

    //Set 2nd row of output matrix
    for( std::size_t c2 = 0; c2 < C2; ++c2 ) {
        out.elem[1][c2] = left._10 * right.elem[0][c2] +
                          left._11 * right.elem[1][c2] +
                          left._12 * right.elem[2][c2];
    }

    //Set 3rd row of output matrix
    for( std::size_t c2 = 0; c2 < C2; ++c2 ) {
        out.elem[2][c2] = left._20 * right.elem[0][c2] +
                          left._21 * right.elem[1][c2] +
                          left._22 * right.elem[2][c2];
    }

    return out;
}

template< typename T, std::size_t R2 >
Matrix< T, R2, 3 > operator *( const Matrix< T, R2, 3 >& left, const Matrix< T, 3, 3 >& right ) {
    Matrix< T, R2, 3 > out;

    for( std::size_t r2 = 0; r2 < R2; ++r2 ) {
        //Set 1st column of output matrix
        out.elem[r2][0] = left.elem[r2][0] * right._00 +
                          left.elem[r2][1] * right._10 +
                          left.elem[r2][2] * right._20;

        //Set 2nd column of output matrix
        out.elem[r2][1] = left.elem[r2][0] * right._01 +
                          left.elem[r2][1] * right._11 +
                          left.elem[r2][2] * right._21;

        //Set 3rd column of output matrix
        out.elem[r2][2] = left.elem[r2][0] * right._02 +
                          left.elem[r2][1] * right._12 +
                          left.elem[r2][2] * right._22;
    }

    return out;
}

//3D-vector (as a 1x3 matrix) * 3x3 matrix = 3D-vector (as a 1x3 matrix)
template< typename T >
Vector< T, 3 > operator *( const Vector< T, 3 >& left, const Matrix< T, 3, 3 >& right ) {
    return Vector< T, 3 >(
        left.x * right._00 + left.y * right._10 + left.z * right._20,
        left.x * right._01 + left.y * right._11 + left.z * right._21,
        left.x * right._02 + left.y * right._12 + left.z * right._22
    );
}

//3x3 matrix * 3D-vector (as a 3x1 matrix) = 3D-vector (as a 3x1 matrix)
template< typename T >
Vector< T, 3 > operator *( const Matrix< T, 3, 3 >& left, const Vector< T, 3 >& right ) {
    return Vector< T, 3 >(
        left._00 * right.x + left._01 * right.y + left._02 * right.z,
        left._10 * right.x + left._11 * right.y + left._12 * right.z,
        left._20 * right.x + left._21 * right.y + left._22 * right.z
    );
}

//3D-vector (as a 1x3 matrix) * 3x3 matrix = 3D-vector (as a 1x3 matrix)
template< typename T >
Vector< T, 3 >& operator *=( Vector< T, 3 >& leftInOut, const Matrix< T, 3, 3 >& right ) {
    T cx = leftInOut.x;
    T cy = leftInOut.y;

    leftInOut.x = cx * right._00 + cy * right._10 + leftInOut.z * right._20;
    leftInOut.y = cx * right._01 + cy * right._11 + leftInOut.z * right._21;
    leftInOut.z = cx * right._02 + cy * right._12 + leftInOut.z * right._22;

    return leftInOut;
}

template< typename T >
Matrix< T, 3, 3 > operator +( const T left, const Matrix< T, 3, 3 >& right ) {
    return Matrix< T, 3, 3 >(
        left + right._00,   left + right._01,   left + right._02,
        left + right._10,   left + right._11,   left + right._12,
        left + right._20,   left + right._21,   left + right._22
    );
}

template< typename T >
Matrix< T, 3, 3 > operator +( const Matrix< T, 3, 3 >& left, const T right ) {
    return Matrix< T, 3, 3 >(
        left._00 + right,   left._01 + right,   left._02 + right,
        left._10 + right,   left._11 + right,   left._12 + right,
        left._20 + right,   left._21 + right,   left._22 + right
    );
}

template< typename T >
Matrix< T, 3, 3 > operator -( const Matrix< T, 3, 3 >& left, const T right ) {
    return Matrix< T, 3, 3 >(
        left._00 - right,   left._01 - right,   left._02 - right,
        left._10 - right,   left._11 - right,   left._12 - right,
        left._20 - right,   left._21 - right,   left._22 - right
    );
}

template< typename T >
Matrix< T, 3, 3 > operator *( const T left, const Matrix< T, 3, 3 >& right ) {
    return Matrix< T, 3, 3 >(
        left * right._00,   left * right._01,   left * right._02,
        left * right._10,   left * right._11,   left * right._12,
        left * right._20,   left * right._21,   left * right._22
    );
}

template< typename T >
Matrix< T, 3, 3 > operator *( const Matrix< T, 3, 3 >& left, const T right ) {
    return Matrix< T, 3, 3 >(
        left._00 * right,   left._01 * right,   left._02 * right,
        left._10 * right,   left._11 * right,   left._12 * right,
        left._20 * right,   left._21 * right,   left._22 * right
    );
}

template< typename T >
Matrix< T, 3, 3 > operator /( const Matrix< T, 3, 3 >& left, const T right ) {
    BS_ASSERT_NONZERO_DIVISOR( right );

    return Matrix< T, 3, 3 >(
        left._00 / right,   left._01 / right,   left._02 / right,
        left._10 / right,   left._11 / right,   left._12 / right,
        left._20 / right,   left._21 / right,   left._22 / right
    );
}

template< typename T >
Matrix< T, 3, 3 > transpose( const Matrix< T, 3, 3 >& matrix ) {
    return Matrix< T, 3, 3 >(
        matrix._00, matrix._10, matrix._20,
        matrix._01, matrix._11, matrix._21,
        matrix._02, matrix._12, matrix._22
    );
}

template< typename T >
Matrix< T, 3, 3 > invert( const Matrix< T, 3, 3 >& matrix ) {
    T c00 = matrix._11 * matrix._22 - matrix._12 * matrix._21;
    T c01 = matrix._12 * matrix._20 - matrix._10 * matrix._22;
    T c02 = matrix._10 * matrix._21 - matrix._11 * matrix._20;

    T det = matrix._00 * c00 +
            matrix._01 * c01 +
            matrix._02 * c02;

    BS_ASSERT_NONZERO_DIVISOR( det );

    return Matrix< T, 3, 3 >(
        c00                                                   / det,
        ( matrix._02 * matrix._21 - matrix._01 * matrix._22 ) / det,
        ( matrix._01 * matrix._12 - matrix._02 * matrix._11 ) / det,

        c01                                                   / det,
        ( matrix._00 * matrix._22 - matrix._02 * matrix._20 ) / det,
        ( matrix._02 * matrix._10 - matrix._00 * matrix._12 ) / det,

        c02                                                   / det,
        ( matrix._01 * matrix._20 - matrix._00 * matrix._21 ) / det,
        ( matrix._00 * matrix._11 - matrix._01 * matrix._10 ) / det
    );
}

template< typename T >
Matrix< T, 3, 3 >::Matrix(
    const T _00, const T _01, const T _02,
    const T _10, const T _11, const T _12,
    const T _20, const T _21, const T _22
) :
    _00( _00 ),     _01( _01 ),     _02( _02 ),
    _10( _10 ),     _11( _11 ),     _12( _12 ),
    _20( _20 ),     _21( _21 ),     _22( _22 )
{
}

template< typename T >
void Matrix< T, 3, 3 >::set(
    const T _00, const T _01, const T _02,
    const T _10, const T _11, const T _12,
    const T _20, const T _21, const T _22
) {
    Matrix::_00 = _00;  Matrix::_01 = _01;  Matrix::_02 = _02;
    Matrix::_10 = _10;  Matrix::_11 = _11;  Matrix::_12 = _12;
    Matrix::_20 = _20;  Matrix::_21 = _21;  Matrix::_22 = _22;
}

template< typename T >
void Matrix< T, 3, 3 >::get(
    T& _00Out, T& _01Out, T& _02Out,
    T& _10Out, T& _11Out, T& _12Out,
    T& _20Out, T& _21Out, T& _22Out
) const {
    _00Out = _00;   _01Out = _01;   _02Out = _02;
    _10Out = _10;   _11Out = _11;   _12Out = _12;
    _20Out = _20;   _21Out = _21;   _22Out = _22;
}





//Typedefs
template< typename T >
using Matrix3x3 = Matrix< T, 3, 3 >;
typedef Matrix3x3< int32  > Matrix3x3i;
typedef Matrix3x3< float  > Matrix3x3f;
typedef Matrix3x3< double > Matrix3x3d;

}




#endif //BS_MATRIX_MATRIX3X3_HPP
