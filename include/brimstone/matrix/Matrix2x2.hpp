/*
matrix/Matrix2x2.hpp
-----------------------
Copyright (c) 2014, theJ89

Description:
    Specialization of Matrix for 2x2 matrices.
    Adds the following typedefs for convenience:
        Matrix2x2< T >: Matrix<T,2,2>
        Matrix2x2i:     Matrix<int32,2,2>
        Matrix2x2f:     Matrix<float,2,2>
        Matrix2x2d:     Matrix<double,2,2>
*/
#ifndef BS_MATRIX_MATRIX2X2_HPP
#define BS_MATRIX_MATRIX2X2_HPP




//Includes
#include <utility>                          //std::swap

#include <brimstone/matrix/MatrixNxN.hpp>   //Matrix, BS_MATRIX_SQUARE_DECLARE_METHODS




namespace Brimstone {

template< typename T >
class Matrix< T, 2, 2 > {
public:
//C4201: nonstandard extension used : nameless struct/union
//It's a non-standard feature, but VC++, G++, and LLVM support it so it shouldn't be too much of an issue
#pragma warning( push )
#pragma warning( disable: 4201 )

    union {
        T              data[ 4 ];
        T              elem[2][2];
        struct {
            Vector< T, 2 > row[ 2 ];
        };
        struct {
            T _00, _01,
              _10, _11;
        };
    };

#pragma warning( pop )
public:
    BS_ARRAY_DECLARE_INHERITED_METHODS( Matrix, T )
    BS_ARRAY_DECLARE_METHODS( Matrix, T )
    BS_MATRIX_DECLARE_METHODS( 2, 2 )
    BS_MATRIX_SQUARE_DECLARE_METHODS( 2 )

    Matrix(
        const T _00, const T _01,
        const T _10, const T _11
    );

    void set(
        const T _00, const T _01,
        const T _10, const T _11
    );

    void get(
        T& _00Out, T& _01Out,
        T& _10Out, T& _11Out
    ) const;
private:
    static const Matrix m_identity;
};

BS_ARRAY_DEFINE_METHODS( Matrix, T, data, BS_TMPL_1( typename T ), BS_SPEC_3( T, 2, 2 ) )
BS_MATRIX_DEFINE_METHODS( 2, 2, BS_TMPL_1( typename T ), BS_SPEC_3( T, 2, 2 ) )
BS_MATRIX_SQUARE_DEFINE_METHODS( 2, BS_TMPL_1( typename T ), BS_SPEC_3( T, 2, 2 ) )




//Forward declarations
template< typename T >
Matrix< T, 4, 4 > invert( const Matrix< T, 4, 4 >& matrix );
template< typename T >
Matrix< T, 4, 4 > transpose( const Matrix< T, 4, 4 >& matrix );




template< typename T >
const Matrix< T, 2, 2 > Matrix< T, 2, 2 >::m_identity = Matrix< T, 2, 2 >(
    static_cast< T >( 1 ),  static_cast< T >( 0 ),
    static_cast< T >( 0 ),  static_cast< T >( 1 )
);

template< typename T >
inline Matrix< T, 2, 2 >::Matrix( const T& elem ) :
    _00( elem ),    _01( elem ),
    _10( elem ),    _11( elem ) {
}

template< typename T >
template< typename T2 >
inline Matrix< T, 2, 2 >::Matrix( const T2& cppRange ) :
    _00( cppRange[0] ),     _01( cppRange[1] ),
    _10( cppRange[2] ),     _11( cppRange[3] ) {
    BS_ASSERT_SIZE( rangeSize( cppRange ), 4 );
}

template< typename T >
inline Matrix< T, 2, 2 >::Matrix( std::initializer_list< T > il ) :
    _00( *( std::begin( il )     ) ),
    _01( *( std::begin( il ) + 1 ) ),
    _10( *( std::begin( il ) + 2 ) ),
    _11( *( std::begin( il ) + 3 ) ) {
    BS_ASSERT_SIZE( rangeSize( il ), 4 );
}

template< typename T >
template< typename T2 >
inline void Matrix< T, 2, 2 >::set( const T2& cppRange ) {
    BS_ASSERT_SIZE( rangeSize( cppRange ), 4 );

    _00 = cppRange[0];  _01 = cppRange[1];
    _10 = cppRange[2];  _11 = cppRange[3];
}
template< typename T >
inline void Matrix< T, 2, 2 >::set( std::initializer_list< T > il ) {
    BS_ASSERT_SIZE( rangeSize( il ), 4 );

    auto it = std::begin( il );
    _00 = *( it      );
    _01 = *( it +  1 );
    _10 = *( it +  2 );
    _11 = *( it +  3 );
}
template< typename T >
template< typename T2 >
inline void Matrix< T, 2, 2 >::get( T2& cppRangeOut ) const {
    BS_ASSERT_SIZE( rangeSize( cppRangeOut ), 4 );

    cppRangeOut[0] = _00;   cppRangeOut[1] = _01;
    cppRangeOut[2] = _10;   cppRangeOut[3] = _11;
}

template< typename T >
inline void Matrix< T, 2, 2 >::fill( const T& elem ) {
    _00 = elem; _01 = elem;
    _10 = elem; _11 = elem;
}

template< typename T >
Matrix< T, 2, 2 >::Matrix()
#ifdef BS_ZERO
    : Matrix( m_identity )
#endif //BS_ZERO
{
}

template< typename T >
template< typename T2 >
Matrix< T, 2, 2 >::Matrix( const Matrix< T2, 2, 2 >& toCopy ) :
    _00( static_cast< T >( toCopy._00 ) ), _01( static_cast< T >( toCopy._01 ) ),
    _10( static_cast< T >( toCopy._10 ) ), _11( static_cast< T >( toCopy._11 ) ) {
}

template< typename T >
bool Matrix< T, 2, 2 >::isSquare() const {
    return true;
}

template< typename T >
void Matrix< T, 2, 2 >::zero() {
    T zero = static_cast< T >( 0 );
    _00 = zero; _01 = zero;
    _10 = zero; _11 = zero;
}

template< typename T >
bool Matrix< T, 2, 2 >::isZero() const {
    T zero = static_cast< T >( 0 );
    return _00 == zero && _01 == zero &&
           _10 == zero && _11 == zero;
}

template< typename T >
void Matrix< T, 2, 2 >::setRow( const size_t row, const Vector< T, 2 >& values ) {
    BS_ASSERT_INDEX( row, 1 );

    elem[row][0] = values.data[0];
    elem[row][1] = values.data[1];
}

template< typename T >
void Matrix< T, 2, 2 >::setColumn( const size_t col, const Vector< T, 2 >& values ) {
    BS_ASSERT_INDEX( col, 1 );

    elem[0][col] = values.data[0];
    elem[1][col] = values.data[1];
}

template< typename T >
Vector< T, 2 > Matrix< T, 2, 2 >::getRow( const size_t row ) const {
    BS_ASSERT_INDEX( row, 1 );

    return Vector< T, 2 >(
        elem[row][0],
        elem[row][1]
    );
}

template< typename T >
Vector< T, 2 > Matrix< T, 2, 2 >::getColumn( const size_t col ) const {
    BS_ASSERT_INDEX( col, 1 );

    return Vector< T, 2 >(
        elem[0][col],
        elem[1][col]
    );
}

template< typename T >
void Matrix< T, 2, 2 >::setDiagonal( const Vector< T, 2 >& values ) {
    _00 = values.x;
    _11 = values.y;
}

template< typename T >
Vector< T, 2 > Matrix< T, 2, 2 >::getDiagonal() const {
    return Vector< T, 2 >( _00, _11 );
}

template< typename T >
T Matrix< T, 2, 2 >::getDeterminant() const {
    return _00 * _11 - _01 * _10;
}

template< typename T >
bool Matrix< T, 2, 2 >::isLowerTriangular() const {
    return _01 == static_cast< T >( 0 );
}

template< typename T >
bool Matrix< T, 2, 2 >::isUpperTriangular() const {
    return _10 == static_cast< T >( 0 );
}

template< typename T >
T Matrix< T, 2, 2 >::getTrace() const {
    return _00 + _11;
}

template< typename T >
void Matrix< T, 2, 2 >::transpose() {
    std::swap( _01, _10 );
}

template< typename T >
void Matrix< T, 2, 2 >::invert() {
    T det = getDeterminant();

    BS_ASSERT_NONZERO_DIVISOR( det );

    //WARNING: Due to rounding errors, inversions may not work properly
    //with non-floating point types!
    T swap = _00;
    _00 =  _11 / det;   _01 = -_01 / det;
    _10 = -_10 / det;   _11 = swap / det;
}

template< typename T >
template< typename T2 >
Matrix< T, 2, 2 >& Matrix< T, 2, 2 >::operator =( const Matrix< T2, 2, 2 >& right ) {
    _00 = static_cast<T>( right._00 );  _01 = static_cast<T>( right._01 );
    _10 = static_cast<T>( right._10 );  _11 = static_cast<T>( right._11 );

    return (*this);
}

template< typename T >
Matrix< T, 2, 2 >& Matrix< T, 2, 2 >::operator +=( const Matrix& right ) {
    _00 += right._00;   _01 += right._01;
    _10 += right._10;   _11 += right._11;

    return ( *this );
}

template< typename T >
Matrix< T, 2, 2 >& Matrix< T, 2, 2 >::operator -=( const Matrix& right ) {
    _00 -= right._00;   _01 -= right._01;
    _10 -= right._10;   _11 -= right._11;

    return ( *this );
}

template< typename T >
Matrix< T, 2, 2 >& Matrix< T, 2, 2 >::operator *=( const Matrix< T, 2, 2 >& right ) {
    /*
    When calculating the multiplied values for each cell on a row of the matrix,
    the original values from the previous cells on that row need to be preserved
    (because each calculation of a value on a row consists of a dot product involving the original row).
    After we've calculated all the values for a row, the previous row is never referenced again.
    Therefore an entirely seperate matrix isn't necessary; just one row.
    */
    T c0 = _00;

    _00 = c0 * right._00 + _01 * right._10;
    _01 = c0 * right._01 + _01 * right._11;


    c0 = _10;

    _10 = c0 * right._00 + _11 * right._10;
    _11 = c0 * right._01 + _11 * right._11;

    return ( *this );
}

template< typename T >
Matrix< T, 2, 2 >& Matrix< T, 2, 2 >::operator +=( const T right ) {
    _00 += right;   _01 += right;
    _10 += right;   _11 += right;

    return ( *this );
}

template< typename T >
Matrix< T, 2, 2 >& Matrix< T, 2, 2 >::operator -=( const T right ) {
    _00 -= right;   _01 -= right;
    _10 -= right;   _11 -= right;

    return ( *this );
}

template< typename T >
Matrix< T, 2, 2 >& Matrix< T, 2, 2 >::operator *=( const T right ) {
    _00 *= right;   _01 *= right;
    _10 *= right;   _11 *= right;

    return ( *this );
}

template< typename T >
Matrix< T, 2, 2 >& Matrix< T, 2, 2 >::operator /=( const T right ) {
    BS_ASSERT_NONZERO_DIVISOR( right );

    _00 /= right;   _01 /= right;
    _10 /= right;   _11 /= right;

    return ( *this );
}

template< typename T >
bool operator ==( const Matrix< T, 2, 2 >& left, const Matrix< T, 2, 2 >& right ) {
    return left._00 == right._00 &&     left._01 == right._01 &&
           left._10 == right._10 &&     left._11 == right._11;
}

template< typename T >
bool operator !=( const Matrix< T, 2, 2 >& left, const Matrix< T, 2, 2 >& right ) {
    return left._00 != right._00 ||     left._01 != right._01 ||
           left._10 != right._10 ||     left._11 != right._11;
}

template< typename T >
Matrix< T, 2, 2 > operator +( const Matrix< T, 2, 2 >& right ) {
    return right;
}

template< typename T >
Matrix< T, 2, 2 > operator -( const Matrix< T, 2, 2 >& right ) {
    return Matrix< T, 2, 2 >(
        -right._00,     -right._01,
        -right._10,     -right._11
    );
}

template< typename T >
Matrix< T, 2, 2 > operator +( const Matrix< T, 2, 2 >& left, const Matrix< T, 2, 2 >& right ) {
    return Matrix< T, 2, 2 >(
        left._00 + right._00,   left._01 + right._01,
        left._10 + right._10,   left._11 + right._11
    );
}

template< typename T >
Matrix< T, 2, 2 > operator -( const Matrix< T, 2, 2 >& left, const Matrix< T, 2, 2 >& right ) {
    return Matrix< T, 2, 2 >(
        left._00 - right._00,   left._01 - right._01,
        left._10 - right._10,   left._11 - right._11
    );
}

template< typename T >
Matrix< T, 2, 2 > operator *( const Matrix< T, 2, 2 >& left, const Matrix< T, 2, 2 >& right ) {
    return Matrix< T, 2, 2 >(
        //First row
        left._00 * right._00 + left._01 * right._10,
        left._00 * right._01 + left._01 * right._11,

        //Second row
        left._10 * right._00 + left._11 * right._10,
        left._10 * right._01 + left._11 * right._11
    );
}

template< typename T, size_t C2 >
Matrix< T, 2, C2 > operator *( const Matrix< T, 2, 2 >& left, const Matrix< T, 2, C2 >& right ) {
    Matrix< T, 2, C2 > out;

    //Set 1st row of output matrix
    for( size_t c2 = 0; c2 < C2; ++c2 ) {
        out.elem[0][c2] = left._00 * right.elem[0][c2] +
                          left._01 * right.elem[1][c2];
    }

    //Set 2nd row of output matrix
    for( size_t c2 = 0; c2 < C2; ++c2 ) {
        out.elem[1][c2] = left._10 * right.elem[0][c2] +
                          left._11 * right.elem[1][c2];
    }

    return out;
}

template< typename T, size_t R2 >
Matrix< T, R2, 2 > operator *( const Matrix< T, R2, 2 >& left, const Matrix< T, 2, 2 >& right ) {
    Matrix< T, R2, 2 > out;

    for( size_t r2 = 0; r2 < R2; ++r2 ) {
        //Set 1st column of output matrix
        out.elem[r2][0] = left.elem[r2][0] * right._00 +
                          left.elem[r2][1] * right._10;

        //Set 2nd column of output matrix
        out.elem[r2][1] = left.elem[r2][0] * right._01 +
                          left.elem[r2][1] * right._11;
    }

    return out;
}

//2D-vector (as a 1x2 matrix) * 2x2 matrix = 2D-vector (as a 1x2 matrix)
template< typename T >
Vector< T, 2 > operator *( const Vector< T, 2 >& left, const Matrix< T, 2, 2 >& right ) {
    return Vector< T, 2 >(
        left.x * right._00 + left.y * right._10,
        left.x * right._01 + left.y * right._11
    );
}

//2x2 matrix * 2D-vector (as a 2x1 matrix) = 2D-vector (as a 2x1 matrix)
template< typename T >
Vector< T, 2 > operator *( const Matrix< T, 2, 2 >& left, const Vector< T, 2 >& right ) {
    return Vector< T, 2 >(
        left._00 * right.x + left._01 * right.y,
        left._10 * right.x + left._11 * right.y
    );
}

//2D-vector (as a 1x2 matrix) * 2x2 matrix = 2D-vector (as a 1x2 matrix)
template< typename T >
Vector< T, 2 >& operator *=( Vector< T, 2 >& leftInOut, const Matrix< T, 2, 2 >& right ) {
    T cx = leftInOut.x;

    leftInOut.x = cx * right._00 + leftInOut.y * right._10;
    leftInOut.y = cx * right._01 + leftInOut.y * right._11;

    return leftInOut;
}

template< typename T >
Matrix< T, 2, 2 > operator +( const T left, const Matrix< T, 2, 2 >& right ) {
    return Matrix< T, 2, 2 >(
        left + right._00,   left + right._01,
        left + right._10,   left + right._11
    );
}

template< typename T >
Matrix< T, 2, 2 > operator +( const Matrix< T, 2, 2 >& left, const T right ) {
    return Matrix< T, 2, 2 >(
        left._00 + right,   left._01 + right,
        left._10 + right,   left._11 + right
    );
}

template< typename T >
Matrix< T, 2, 2 > operator -( const Matrix< T, 2, 2 >& left, const T right ) {
    return Matrix< T, 2, 2 >(
        left._00 - right,   left._01 - right,
        left._10 - right,   left._11 - right
    );
}

template< typename T >
Matrix< T, 2, 2 > operator *( const T left, const Matrix< T, 2, 2 >& right ) {
    return Matrix< T, 2, 2 >(
        left * right._00,   left * right._01,
        left * right._10,   left * right._11
    );
}

template< typename T >
Matrix< T, 2, 2 > operator *( const Matrix< T, 2, 2 >& left, const T right ) {
    return Matrix< T, 2, 2 >(
        left._00 * right,   left._01 * right,
        left._10 * right,   left._11 * right
    );
}

template< typename T >
Matrix< T, 2, 2 > operator /( const Matrix< T, 2, 2 >& left, const T right ) {
    BS_ASSERT_NONZERO_DIVISOR( right );

    return Matrix< T, 2, 2 >(
        left._00 / right,   left._01 / right,
        left._10 / right,   left._11 / right
    );
}

template< typename T >
Matrix< T, 2, 2 > invert( const Matrix< T, 2, 2 >& matrix ) {
    T det = matrix.getDeterminant();

    BS_ASSERT_NONZERO_DIVISOR( det );

    return Matrix< T, 2, 2 >(
         matrix._11 / det,  -matrix._01 / det,
        -matrix._10 / det,   matrix._00 / det
    );
}

template< typename T >
Matrix< T, 2, 2 > transpose( const Matrix< T, 2, 2 >& matrix ) {
    return Matrix< T, 2, 2 >(
        matrix._00,     matrix._10,
        matrix._01,     matrix._11
    );
}

template< typename T >
Matrix< T, 2, 2 >::Matrix(
    const T _00, const T _01,
    const T _10, const T _11
) :
    _00( _00 ),     _01( _01 ),
    _10( _10 ),     _11( _11 )
{
}

template< typename T >
void Matrix< T, 2, 2 >::set(
    const T _00, const T _01,
    const T _10, const T _11
) {
    Matrix::_00 = _00;  Matrix::_01 = _01;
    Matrix::_10 = _10;  Matrix::_11 = _11;
}

template< typename T >
void Matrix< T, 2, 2 >::get(
    T& _00Out, T& _01Out,
    T& _10Out, T& _11Out
) const {
    _00Out = _00;   _01Out = _01;
    _10Out = _10;   _11Out = _11;
}

//Typedefs
template< typename T >
using Matrix2x2 = Matrix< T, 2, 2 >;
typedef Matrix2x2< int32  > Matrix2x2i;
typedef Matrix2x2< float  > Matrix2x2f;
typedef Matrix2x2< double > Matrix2x2d;

}




#endif //BS_MATRIX_MATRIX2X2_HPP