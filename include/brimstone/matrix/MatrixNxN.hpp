/*
matrix/MatrixNxN.hpp
-----------------------
Copyright (c) 2014, theJ89

Description:
    Specialization of Matrix for square matrices that aren't covered
    by existing specializations, such as the specialization defined in Matrix3x3.hpp.
*/
#ifndef BS_MATRIX_MATRIXNXN_HPP
#define BS_MATRIX_MATRIXNXN_HPP




//Includes
#include <brimstone/matrix/MatrixRxC.hpp>
#include <utility>                          //std::swap




#define BS_MATRIX_SQUARE_DECLARE_METHODS( N )                               \
    T    getDeterminant() const;                                            \
    bool isInvertible() const;                                              \
                                                                            \
    bool isTriangular() const;                                              \
    bool isLowerTriangular() const;                                         \
    bool isUpperTriangular() const;                                         \
    bool isDiagonal() const;                                                \
                                                                            \
    T    getTrace() const;                                                  \
                                                                            \
    void transpose();                                                       \
    void invert();                                                          \
                                                                            \
    void setDiagonal( const Vector< T, N >& values );                       \
    Vector< T, N > getDiagonal() const;

#define BS_MATRIX_SQUARE_DEFINE_METHODS( N, tmpl, spec )    \
    tmpl                                                    \
    bool Matrix spec::isInvertible() const {                \
        return getDeterminant() != 0;                       \
    }                                                       \
    tmpl                                                    \
    bool Matrix spec::isTriangular() const {                \
        return isLowerTriangular() || isUpperTriangular();  \
    }                                                       \
    tmpl                                                    \
    bool Matrix spec::isDiagonal() const {                  \
        return isLowerTriangular() && isUpperTriangular();  \
    }

namespace Brimstone {




template< typename T, size_t N >
class Matrix< T, N, N > {
public:
//C4201: nonstandard extension used : nameless struct/union
//It's a non-standard feature, but VC++, G++, and LLVM support it so it shouldn't be too much of an issue
#pragma warning( push )
#pragma warning( disable: 4201 )

    union {
        T              data[ N * N ];
        T              elem[N][N];
        struct {
            Vector< T, N > row[ N ];
        };
    };

#pragma warning( pop )
public:
    BS_ARRAY_DECLARE_INHERITED_METHODS( Matrix, T )
    BS_ARRAY_DECLARE_METHODS( Matrix, T )
    BS_MATRIX_DECLARE_METHODS( N, N )
    BS_MATRIX_SQUARE_DECLARE_METHODS( N )

private:
    struct ConstructIdentity {};
    Matrix( const ConstructIdentity );
    static const Matrix m_identity;
};

BS_ARRAY_DEFINE_GENERIC_METHODS( Matrix, T, data, BS_TMPL_2( typename T, size_t N ), BS_SPEC_3( T, N, N ) )
BS_ARRAY_DEFINE_METHODS( Matrix, T, data, BS_TMPL_2( typename T, size_t N ), BS_SPEC_3( T, N, N ) )
BS_MATRIX_DEFINE_METHODS( N, N, BS_TMPL_2( typename T, size_t N ), BS_SPEC_3( T, N, N ) )
BS_MATRIX_SQUARE_DEFINE_METHODS( N, BS_TMPL_2( typename T, size_t N ), BS_SPEC_3( T, N, N ) )




//Forward declarations
template< typename T, size_t N >
Matrix< T, N, N > invert( const Matrix< T, N, N >& matrix );




template< typename T, size_t N >
const Matrix< T, N, N > Matrix< T, N, N >::m_identity = Matrix< T, N, N >( ConstructIdentity() );

template< typename T, size_t N >
Matrix< T, N, N >::Matrix( const ConstructIdentity ) {
    for( size_t r = 0; r < N; ++r )
        for( size_t c = 0; c < N; ++c )
            elem[r][c] = ( r == c ? static_cast< T >( 1 ) : static_cast< T >( 0 ) );
}

template< typename T, size_t N >
Matrix< T, N, N >::Matrix()
#ifdef BS_ZERO
    : Matrix( m_identity )
#endif //BS_ZERO
{
}

template< typename T, size_t N >
template< typename T2 >
Matrix< T, N, N >::Matrix( const Matrix< T2, N, N >& toCopy ) {
    (*this) = toCopy;
}

template< typename T, size_t N >
bool Matrix< T, N, N >::isSquare() const {
    //This function always returns true because the NxN specialization of Matrix handles square matrices.
    return true;
}

template< typename T, size_t N >
void Matrix< T, N, N >::zero() {
    T zero = static_cast< T >( 0 );
    for( size_t i = 0; i < N * N; ++i )
        data[i] = zero;
}

template< typename T, size_t N >
bool Matrix< T, N, N >::isZero() const {
    T zero = static_cast< T >( 0 );
    for( size_t i = 0; i < N * N; ++i )
        if( data[i] != zero )
            return false;
    return true;
}

template< typename T, size_t N >
void Matrix< T, N, N >::setRow( const size_t row, const Vector< T, N >& values ) {
    BS_ASSERT_INDEX( row, N - 1 );
    for( size_t c = 0; c < N; ++c )
        elem[row][c] = values.data[c];
}

template< typename T, size_t N >
void Matrix< T, N, N >::setColumn( const size_t col, const Vector< T, N >& values ) {
    BS_ASSERT_INDEX( col, N - 1 );
    for( size_t r = 0; r < N; ++r )
        elem[r][col] = values.data[r];
}

template< typename T, size_t N >
Vector< T, N > Matrix< T, N, N >::getRow( const size_t row ) const {
    BS_ASSERT_INDEX( row, N - 1 );
    Vector< T, N > out;

    for( size_t c = 0; c < N; ++c )
        out.data[c] = elem[row][c];

    return out;
}

template< typename T, size_t N >
Vector< T, N > Matrix< T, N, N >::getColumn( const size_t col ) const {
    BS_ASSERT_INDEX( col, N - 1 );
    Vector< T, N > out;

    for( size_t r = 0; r < N; ++r )
        out.data[r] = elem[r][col];

    return out;
}

template< typename T, size_t N >
void Matrix< T, N, N >::setDiagonal( const Vector< T, N >& values ) {
    for( size_t rc = 0; rc < N; ++rc )
        elem[rc][rc] = values.data[rc];
}

template< typename T, size_t N >
Vector< T, N > Matrix< T, N, N >::getDiagonal() const {
    Vector< T, N > out;

    for( size_t rc = 0; rc < N; ++rc )
        out.data[rc] = elem[rc][rc];

    return out;
}

template< typename T, size_t N >
T Matrix< T, N, N >::getDeterminant() const {
    //TODO:
    //Certain minors are reused multiple times when calculating the determinant.
    //You could calculate it more efficiently if you could somehow cache the minor.
    Matrix< T, N - 1, N - 1 > minorMatrix;

    //The sign of a cofactor, C_(r,c), is positive if r+c is an even number,
    //and negative if r+c is an odd number.
    T sign = ( ( N - 1 ) & 1 ) == 1 ? static_cast< T >( -1 ) : static_cast< T >( 1 );
    T det  = static_cast< T >( 0 );
    for( size_t rr = 0; rr < N; ++rr ) {
        //If we're multiplying against 0, we can skip a lot of work here
        //because we know ahead of time that the contribution to the determinant will be 0
        //Note: We calculate the minor using the last column of the matrix
        //This column was chosen because it tends to have a lot of zeros.
        if( elem[rr][N-1] == static_cast< T >( 0 ) )
            continue;

        //Get the matrix that would result from removing row rr and column N - 1
        for( size_t c = 0; c < N - 1; ++c ) {
            for( size_t r = 0; r < rr; ++r )
                minorMatrix.elem[r][c]   = elem[r][c];
            for( size_t r = rr+1; r < N; ++r )
                minorMatrix.elem[r-1][c] = elem[r][c];
        }

        //Compute the cofactor and add it to the determinant
        det += sign * elem[rr][N-1] * minorMatrix.getDeterminant();

        //Flip the sign
        sign = -sign;
    }

    return det;
}

template< typename T, size_t N >
bool Matrix< T, N, N >::isLowerTriangular() const {
    T zero = static_cast< T >( 0 );
    for( size_t r = 0; r < N - 1; ++r )
        for( size_t c = r+1; c < N; ++c )
            if( elem[r][c] != zero )
                return false;
    return true;
}

template< typename T, size_t N >
bool Matrix< T, N, N >::isUpperTriangular() const {
    T zero = static_cast< T >( 0 );
    for( size_t r = 1; r < N; ++r )
        for( size_t c = 0; c < r; ++c )
            if( elem[r][c] != zero )
                return false;
    return true;
}

template< typename T, size_t N >
T Matrix< T, N, N >::getTrace() const {
    T tr = elem[0][0];
    for( size_t rc = 1; rc < N; ++rc )
        tr += elem[rc][rc];
    return tr;
}

template< typename T, size_t N >
void Matrix< T, N, N >::transpose() {
    for( size_t r = 1; r < N; ++r )
        for( size_t c = 0; c < r; ++c )
            std::swap( elem[r][c], elem[c][r] );
}

template< typename T, size_t N >
void Matrix< T, N, N >::invert() {
    //This function makes an inverted copy of itself,
    //then assigns the inverted copy to itself.
    (*this) = Brimstone::invert( *this );
}

template< typename T, size_t N >
template< typename T2 >
Matrix< T, N, N >& Matrix< T, N, N >::operator =( const Matrix< T2, N, N >& right ) {
    for( size_t i = 0; i < N * N; ++i )
        data[i] = static_cast<T>( right.data[i] );

    return (*this);
}

template< typename T, size_t N >
Matrix< T, N, N >& Matrix< T, N, N >::operator +=( const Matrix& right ) {
    for( size_t i = 0; i < N * N; ++i )
        data[i] += right.data[i];

    return ( *this );
}

template< typename T, size_t N >
Matrix< T, N, N >& Matrix< T, N, N >::operator -=( const Matrix& right ) {
    for( size_t i = 0; i < N * N; ++i )
        data[i] -= right.data[i];

    return ( *this );
}

template< typename T, size_t N >
Matrix< T, N, N >& Matrix< T, N, N >::operator *=( const Matrix< T, N, N >& right ) {
    Private::MatrixMultiplyInPlace( *this, right );
    return ( *this );
}

//N-vector (as a 1xN matrix) * NxN matrix = N-vector (as a 1xN matrix)
template< typename T, size_t N >
Vector< T, N >& operator *=( Vector< T, N >& leftInOut, const Matrix< T, N, N >& right ) {
    //Store the old values for all but the last element in the input vector
    //The old values are used to calculate the new values. We need to do this
    //because the values in the vector get overwritten by new values as the algorithm progresses
    T leftOld[ N - 1 ];
    for( size_t c = 0; c < N - 1; ++c )
        leftOld[c] = leftInOut.data[c];

    T dot;
    for( size_t c = 0; c < N; ++c ) {
        dot = static_cast< T >( 0 );

        for( size_t r = 0; r < N - 1; ++r )
            dot += leftOld[r] * right.elem[r][c];

        leftInOut.data[c] = dot +
                            leftInOut[ N - 1 ] * right.elem[ N - 1 ][ c ];
    }

    return leftInOut;
}

template< typename T, size_t N >
Matrix< T, N, N >& Matrix< T, N, N >::operator +=( const T right ) {
    for( size_t i = 0; i < N * N; ++i )
        data[i] += right;

    return ( *this );
}

template< typename T, size_t N >
Matrix< T, N, N >& Matrix< T, N, N >::operator -=( const T right ) {
    for( size_t i = 0; i < N * N; ++i )
        data[i] -= right;

    return ( *this );
}

template< typename T, size_t N >
Matrix< T, N, N >& Matrix< T, N, N >::operator *=( const T right ) {
    for( size_t i = 0; i < N * N; ++i )
        data[i] *= right;

    return ( *this );
}

template< typename T, size_t N >
Matrix< T, N, N >& Matrix< T, N, N >::operator /=( const T right ) {
    BS_ASSERT_NONZERO_DIVISOR( right )

    for( size_t i = 0; i < N * N; ++i )
        data[i] /= right;

    return ( *this );
}

template< typename T, size_t N >
Matrix< T, N, N > invert( const Matrix< T, N, N >& matrix ) {
    //WARNING: The invert function for N > 4 matrices
    //is pretty unoptimized. Use at your own risk...
    T det = matrix.getDeterminant();

    BS_ASSERT_NONZERO_DIVISOR( det );

    Matrix< T, N, N > out;
    Matrix< T, N - 1, N - 1 > minorMatrix;

    //The sign of a cofactor, C_(r,c), is positive if r+c is an even number,
    //and negative if r+c is an odd number.
    T sign = static_cast< T >( 1 );

    for( size_t r = 0; r < N; ++r ) {
        for( size_t c = 0; c < N; ++c ) {

            //Get the matrix that results from removing row r, column c
            //from the given matrix so we can get its determinant
            for( size_t r2 = 0; r2 < r; ++r2 ) {
                for( size_t c2 = 0;   c2 < c; ++c2 )
                    minorMatrix.elem[r2][c2] = matrix.elem[r2][c2];
                for( size_t c2 = c+1; c2 < N; ++c2 )
                    minorMatrix.elem[r2][c2-1] = matrix.elem[r2][c2];
            }
            for( size_t r2 = r+1; r2 < N; ++r2 ) {
                for( size_t c2 = 0;   c2 < c; ++c2 )
                    minorMatrix.elem[r2-1][c2]   = matrix.elem[r2][c2];
                for( size_t c2 = c+1; c2 < N; ++c2 )
                    minorMatrix.elem[r2-1][c2-1] = matrix.elem[r2][c2];
            }

            //Calculate the cofactor (sign * the minor for matrix(r,c)),
            //translate it (i.e. in the output, place it in [c][r] instead of [r][c]),
            //then divide by the determinant for this.
            out.elem[c][r] = sign * minorMatrix.getDeterminant() / det;

            //Flip the sign
            sign = -sign;
        }
    }

    return out;
}

//Note: the other operations (mat+mat, mat*mat, etc) are implemented as free functions in MatrixRxC




}




#endif //BS_MATRIX_MATRIXNXN_HPP