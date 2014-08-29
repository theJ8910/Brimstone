/*
matrix/MatrixRxC.hpp
-----------------------
Copyright (c) 2014, theJ89

Description:
    Adds a generic matrix class, Matrix.
    You can specify what type of data the matrix uses (int, float, etc) as well as the number of rows and columns in the matrix.
    The code in this file handles rectangular matrices.
    Common specializations of Matrix are provided in Matrix2x2.hpp, Matrix3x3.hpp, Matrix4x4.hpp, and MatrixNxN.hpp.
*/
#ifndef BS_MATRIX_MATRIXRXC_HPP
#define BS_MATRIX_MATRIXRXC_HPP




//Includes
#include <iostream>                     //std::ostream
#include <iomanip>                      //std::setw, std::setprecision

#include <boost/format.hpp>             //boost::format

#include <brimstone/util/Array.hpp>     //BS_ARRAY_DECLARE_METHODS, BS_ARRAY_DEFINE_METHODS, etc
#include <brimstone/Vector.hpp>         //Vector
#include <brimstone/util/MinMax.hpp>    //electMax




//Macros
#define BS_MATRIX_DECLARE_METHODS( R, C )                                   \
    Matrix();                                                               \
                                                                            \
    template< typename T2 >                                                 \
    Matrix( const Matrix< T2, R, C >& toCopy );                             \
                                                                            \
    bool isSquare() const;                                                  \
    size_t getRows() const;                                                 \
    size_t getColumns() const;                                              \
                                                                            \
    void zero();                                                            \
    bool isZero() const;                                                    \
    void identity();                                                        \
    bool isIdentity() const;                                                \
                                                                            \
    void setRow( const size_t row, const Vector< T, C >& values );          \
    void setColumn( const size_t col, const Vector< T, R >& values );       \
                                                                            \
    Vector< T, C > getRow( const size_t row ) const;                        \
    Vector< T, R > getColumn( const size_t col ) const;                     \
                                                                            \
    template< typename T2 >                                                 \
    Matrix& operator =( const Matrix< T2, R, C >& right );                  \
    Matrix& operator +=( const Matrix& right );                             \
    Matrix& operator -=( const Matrix& right );                             \
    Matrix& operator *=( const Matrix< T, C, C >& right );                  \
    Matrix& operator +=( const T right );                                   \
    Matrix& operator -=( const T right );                                   \
    Matrix& operator *=( const T right );                                   \
    Matrix& operator /=( const T right );                                   \
    T&      operator ()( const size_t row, const size_t col );              \
    T       operator ()( const size_t row, const size_t col ) const;

#define BS_MATRIX_DEFINE_METHODS( R, C, tmpl, spec )                        \
    tmpl                                                                    \
    size_t Matrix spec::getRows() const {                                   \
        return R;                                                           \
    }                                                                       \
    tmpl                                                                    \
    size_t Matrix spec::getColumns() const {                                \
        return C;                                                           \
    }                                                                       \
    tmpl                                                                    \
    void Matrix spec::identity() {                                          \
        (*this) = m_identity;                                               \
    }                                                                       \
    tmpl                                                                    \
    bool Matrix spec::isIdentity() const {                                  \
        return (*this) == m_identity;                                       \
    }                                                                       \
    tmpl                                                                    \
    T& Matrix spec::operator()( const size_t row, const size_t col ) {      \
        BS_ASSERT_INDEX( row, R - 1 );                                      \
        BS_ASSERT_INDEX( col, C - 1 );                                      \
        return elem[row][col];                                              \
    }                                                                       \
    tmpl                                                                    \
    T Matrix spec::operator()( const size_t row, const size_t col ) const { \
        BS_ASSERT_INDEX( row, R - 1 );                                      \
        BS_ASSERT_INDEX( col, C - 1 );                                      \
        return elem[row][col];                                              \
    }

namespace Brimstone {

template< typename T, size_t R, size_t C >
class Matrix {
public:
//C4201: nonstandard extension used : nameless struct/union
//It's a non-standard feature, but VC++, G++, and LLVM support it so it shouldn't be too much of an issue
#pragma warning( push )
#pragma warning( disable: 4201 )

    union {
        T              data[ R * C ];
        T              elem[R][C];
        struct {
            Vector< T, C > row[ R ];
        };
    };

#pragma warning( pop )
public:
    BS_ARRAY_DECLARE_INHERITED_METHODS( Matrix, T )
    BS_ARRAY_DECLARE_METHODS( Matrix, T )
    BS_MATRIX_DECLARE_METHODS( R, C )
private:
    struct ConstructIdentity {};
    Matrix( const ConstructIdentity );
    static const Matrix m_identity;
};
BS_ARRAY_DEFINE_GENERIC_METHODS( Matrix, T, data, BS_TMPL_3( typename T, size_t R, size_t C ), BS_SPEC_3( T, R, C ) )
BS_ARRAY_DEFINE_METHODS( Matrix, T, data, BS_TMPL_3( typename T, size_t R, size_t C ), BS_SPEC_3( T, R, C ) )
BS_MATRIX_DEFINE_METHODS( R, C, BS_TMPL_3( typename T, size_t R, size_t C ), BS_SPEC_3( T, R, C ) )

//Note: We have a private constructor specifically for constructing
//m_identity. To differentiate it from other constructors,
//we pass an unused argument of type "ConstructIdentity",
//ConstructIdentity is a private inner class in Matrix, which means that only something
//belonging to Matrix can instantiate ConstructIdentity objects. Normally this
//means "a method in Matrix", but this also works with statements that initialize
//static members of Matrix as well.
template< typename T, size_t R, size_t C >
const Matrix< T, R, C > Matrix< T, R, C >::m_identity = Matrix< T, R, C >( ConstructIdentity() );

//We could just put this in the default constructor, but the advantage of this approach
//is that by copying a pre-built matrix, we can avoid using two loops and running a conditional
//on each element of every matrix that is default-constructed, and instead only do this once
//when constructing the identity matrix.
template< typename T, size_t R, size_t C >
Matrix< T, R, C >::Matrix( const ConstructIdentity ) {
    for( size_t r = 0; r < R; ++r )
        for( size_t c = 0; c < C; ++c )
            elem[r][c] = ( r == c ? static_cast< T >( 1 ) : static_cast< T >( 0 ) );
}

template< typename T, size_t R, size_t C >
Matrix< T, R, C >::Matrix()
#ifdef BS_ZERO
    : Matrix( m_identity )
#endif //BS_ZERO
{
}

template< typename T, size_t R, size_t C >
template< typename T2 >
Matrix< T, R, C >::Matrix( const Matrix< T2, R, C >& toCopy ) {
    (*this) = toCopy;
}

template< typename T, size_t R, size_t C >
bool Matrix< T, R, C >::isSquare() const {
    //This function always returns false because the generic case of Matrix handles rectangular matrices.
    return false;
}

template< typename T, size_t R, size_t C >
void Matrix< T, R, C >::zero() {
    T zero = static_cast< T >( 0 );
    for( size_t i = 0; i < R * C; ++i )
        data[i] = zero;
}

template< typename T, size_t R, size_t C >
bool Matrix< T, R, C >::isZero() const {
    T zero = static_cast< T >( 0 );
    for( size_t i = 0; i < R * C; ++i )
        if( data[i] != zero )
            return false;
    return true;
}

template< typename T, size_t R, size_t C >
void Matrix< T, R, C >::setRow( const size_t row, const Vector< T, C >& values ) {
    BS_ASSERT_INDEX( row, R - 1 );
    for( size_t c = 0; c < C; ++c )
        elem[row][c] = values.data[c];
}

template< typename T, size_t R, size_t C >
void Matrix< T, R, C >::setColumn( const size_t col, const Vector< T, R >& values ) {
    BS_ASSERT_INDEX( col, C - 1 );
    for( size_t r = 0; r < R; ++r )
        elem[r][col] = values.data[r];
}

template< typename T, size_t R, size_t C >
Vector< T, C > Matrix< T, R, C >::getRow( const size_t row ) const {
    BS_ASSERT_INDEX( row, R - 1 );
    Vector< T, C > out;

    for( size_t c = 0; c < C; ++c )
        out.data[c] = elem[row][c];

    return out;
}

template< typename T, size_t R, size_t C >
Vector< T, R > Matrix< T, R, C >::getColumn( const size_t col ) const {
    BS_ASSERT_INDEX( col, C - 1 );
    Vector< T, R > out;

    for( size_t r = 0; r < R; ++r )
        out.data[r] = elem[r][col];

    return out;
}

template< typename T, size_t R, size_t C >
template< typename T2 >
Matrix< T, R, C >& Matrix< T, R, C >::operator =( const Matrix< T2, R, C >& right ) {
    for( size_t i = 0; i < R * C; ++i )
        data[i] = static_cast<T>( right.data[i] );

    return (*this);
}

template< typename T, size_t R, size_t C >
Matrix< T, R, C >& Matrix< T, R, C >::operator +=( const Matrix& right ) {
    for( size_t i = 0; i < R * C; ++i )
        data[i] += right.data[i];

    return ( *this );
}

template< typename T, size_t R, size_t C >
Matrix< T, R, C >& Matrix< T, R, C >::operator -=( const Matrix& right ) {
    for( size_t i = 0; i < R * C; ++i )
        data[i] -= right.data[i];

    return ( *this );
}

namespace Private {

    //Note: You can only multiply an IxJ matrix with an JxK matrix.
    //      The result is an IxK matrix.
    //      Since this function multiplies in-place (i.e. it implements leftInOut *= right ),
    //      the resulting matrix must be of size IxJ.
    //      The only way this is possible is if K = J;
    //      in other words, the given matrix must be a square JxJ matrix.
    template< typename T, size_t R, size_t C >
    void MatrixMultiplyInPlace( Matrix< T, R, C >& leftInOut, const Matrix< T, C, C >& right ) {
        T oldRow[ C - 1 ];
        T dot;
        for( size_t r = 0; r < R; ++r ) {
            //Store the old values for the current row we're working on
            //We only need to store the first C-1 values, because we need to know
            //their old values when setting their new values for all but the last
            //element in the current row
            for( size_t c = 0; c < C - 1; ++c )
                oldRow[c] = leftInOut.elem[r][c];

            //Update the values
            for( size_t c = 0; c < C; ++c ) {
                dot = static_cast< T >( 0 );

                //Partially calculate the dot product between the current row in the left matrix,
                //and the current column in the right matrix.
                for( size_t c2 = 0; c2 < C - 1; ++c2 )
                    dot += oldRow[ c2 ] * right.elem[c2][c];

                //Finalize the dot product and update the element in the left matrix.
                leftInOut.elem[r][c] = dot +
                                       leftInOut.elem[r][C-1] * right.elem[C-1][c];
            }
        }
    }
}

template< typename T, size_t R, size_t C >
Matrix< T, R, C >& Matrix< T, R, C >::operator *=( const Matrix< T, C, C >& right ) {
    Private::MatrixMultiplyInPlace( *this, right );
    return ( *this );
}

template< typename T, size_t R, size_t C >
Matrix< T, R, C >& Matrix< T, R, C >::operator +=( const T right ) {
    for( size_t i = 0; i < R * C; ++i )
        data[i] += right;

    return ( *this );
}

template< typename T, size_t R, size_t C >
Matrix< T, R, C >& Matrix< T, R, C >::operator -=( const T right ) {
    for( size_t i = 0; i < R * C; ++i )
        data[i] -= right;

    return ( *this );
}

template< typename T, size_t R, size_t C >
Matrix< T, R, C >& Matrix< T, R, C >::operator *=( const T right ) {
    for( size_t i = 0; i < R * C; ++i )
        data[i] *= right;

    return ( *this );
}

template< typename T, size_t R, size_t C >
Matrix< T, R, C >& Matrix< T, R, C >::operator /=( const T right ) {
    BS_ASSERT_NONZERO_DIVISOR( right )

    for( size_t i = 0; i < R * C; ++i )
        data[i] /= right;

    return ( *this );
}

template< typename T, size_t R, size_t C >
std::ostream& operator <<( std::ostream& left, const Matrix< T, R, C >& right ) {
    size_t maxWidth = 1;
    std::string str[ R * C ];
    for( size_t i = 0; i < R * C; ++i ) {
        str[ i ] = ( boost::format( "%|.5f|" ) % right.data[i] ).str();
        electMax( maxWidth, str[ i ].size() );
    }

    for( size_t r = 0; r < R; ++r ) {
        left << std::setw( maxWidth ) << str[r*C];
        for( size_t c = 1; c < C; ++c )
            left << " " << std::setw( maxWidth ) << str[r*C + c];
        left << std::endl;
    }
    return left;
}

template< typename T, size_t R, size_t C >
bool operator ==( const Matrix< T, R, C >& left, const Matrix< T, R, C >& right ) {
    for( size_t i = 0; i < R * C; ++i )
        if( left.data[i] != right.data[i] )
            return false;
    return true;
}

template< typename T, size_t R, size_t C >
bool operator !=( const Matrix< T, R, C >& left, const Matrix< T, R, C >& right ) {
    for( size_t i = 0; i < R * C; ++i )
        if( left.data[i] != right.data[i] )
            return true;
    return false;
}

template< typename T, size_t R, size_t C >
Matrix< T, R, C > operator +( const Matrix< T, R, C >& right ) {
    return right;
}

template< typename T, size_t R, size_t C >
Matrix< T, R, C > operator -( const Matrix< T, R, C >& right ) {
    Matrix< T, R, C > out;

    for( size_t i = 0; i < R * C; ++i )
        out.data[i] = -right.data[i];

    return out;
}

template< typename T, size_t R, size_t C >
Matrix< T, R, C > operator +( const Matrix< T, R, C >& left, const Matrix< T, R, C >& right ) {
    Matrix< T, R, C > out;

    for( size_t i = 0; i < R * C; ++i )
        out.data[i] = left.data[i] + right.data[i];

    return out;
}

template< typename T, size_t R, size_t C >
Matrix< T, R, C > operator -( const Matrix< T, R, C >& left, const Matrix< T, R, C >& right ) {
    Matrix< T, R, C > out;

    for( size_t i = 0; i < R * C; ++i )
        out.data[i] = left.data[i] - right.data[i];

    return out;
}

template< typename T, size_t R, size_t C, size_t C2 >
Matrix< T, R, C2 > operator *( const Matrix< T, R, C >& left, const Matrix< T, C, C2 >& right ) {
    Matrix< T, R, C2 > out;

    T dot;
    for( size_t r = 0; r < R; ++r ) {
        for( size_t c2 = 0; c2 < C2; ++c2 ) {
            dot = static_cast< T >( 0 );

            for( size_t c = 0; c < C; ++c )
                dot += left.elem[r][c] * right.elem[c][c2];

            out.elem[r][c2] = dot;
        }
    }

    return out;
}

//R-vector (as a 1xR matrix) * RxC matrix = C-vector (as a 1xC matrix)
template< typename T, size_t R, size_t C >
Vector< T, C > operator *( const Vector< T, R >& left, const Matrix< T, R, C >& right ) {
    Vector< T, C > out;

    T dot;
    for( size_t c = 0; c < C; ++c ) {
        dot = static_cast< T >( 0 );

        for( size_t r = 0; r < R; ++r )
            dot += left[r] * right.elem[r][c];

        out.data[c] = dot;
    }

    return out;
}

//RxC matrix * C-vector (as a Cx1 matrix) = R-vector (as a Rx1 matrix)
template< typename T, size_t R, size_t C >
Vector< T, R > operator *( const Matrix< T, R, C >& left, const Vector< T, C >& right ) {
    Vector< T, R > out;

    T dot;
    for( size_t r = 0; r < R; ++r ) {
        dot = static_cast< T >( 0 );

        for( size_t c = 0; c < C; ++c )
            dot += left.elem[r][c] * right[c];

        out.data[r] = dot;
    }

    return out;
}

template< typename T, size_t R, size_t C >
Matrix< T, R, C > operator +( const T left, const Matrix< T, R, C >& right ) {
    Matrix< T, R, C > out;

    for( size_t i = 0; i < R * C; ++i )
        out.data[i] = left + right.data[i];

    return out;
}

template< typename T, size_t R, size_t C >
Matrix< T, R, C > operator +( const Matrix< T, R, C >& left, const T right ) {
    Matrix< T, R, C > out;

    for( size_t i = 0; i < R * C; ++i )
        out.data[i] = left.data[i] + right;

    return out;
}

template< typename T, size_t R, size_t C >
Matrix< T, R, C > operator -( const Matrix< T, R, C >& left, const T right ) {
    Matrix< T, R, C > out;

    for( size_t i = 0; i < R * C; ++i )
        out.data[i] = left.data[i] - right;

    return out;
}

template< typename T, size_t R, size_t C >
Matrix< T, R, C > operator *( const T left, const Matrix< T, R, C >& right ) {
    Matrix< T, R, C > out;

    for( size_t i = 0; i < R * C; ++i )
        out.data[i] = left * right.data[i];

    return out;
}

template< typename T, size_t R, size_t C >
Matrix< T, R, C > operator *( const Matrix< T, R, C >& left, const T right ) {
    Matrix< T, R, C > out;

    for( size_t i = 0; i < R * C; ++i )
        out.data[i] = left.data[i] * right;

    return out;
}

template< typename T, size_t R, size_t C >
Matrix< T, R, C > operator /( const Matrix< T, R, C >& left, const T right ) {
    BS_ASSERT_NONZERO_DIVISOR( right );

    Matrix< T, R, C > out;

    for( size_t i = 0; i < R * C; ++i )
        out.data[i] = left.data[i] / right;

    return out;
}

template< typename T, size_t R, size_t C >
Matrix< T, C, R > transpose( const Matrix< T, R, C >& matrix ) {
    Matrix< T, C, R > out;

    for( size_t r = 0; r < R; ++r )
        for( size_t c = 0; c < C; ++c )
            out.elem[c][r] = matrix.elem[r][c];

    return out;
}

}




#endif //BS_MATRIX_MATRIXRXC_HPP