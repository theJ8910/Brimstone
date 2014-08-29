/*
vector/VectorN.hpp
-----------------------
Copyright (c) 2014, theJ89

Description:
    Adds a generic vector class, Vector.
    You can specify what type of data the vector uses (int, float, etc) as well as the dimension of the vector.
    2D, 3D and 4D specializations of Vector are provided in Vector2.hpp, Vector3.hpp, and Vector4.hpp, respectively.
*/
#ifndef BS_VECTOR_VECTORN_HPP
#define BS_VECTOR_VECTORN_HPP




//Includes
#include <initializer_list>             //std::initializer_list
#include <cstdlib>                      //abs

#include <brimstone/Point.hpp>          //BasePoint
#include <brimstone/util/Math.hpp>      //fastSqrt, fastInvSqrt
#include <brimstone/util/MinMax.hpp>    //electMax




//Defines
/*
NOTE: This define controls how much error isUnitVec() tolerates.

isUnitVec() returns true if the square length of the vector is within 1 +- 0.00100025.
The value is chosen with this in mind: For any vector, if you call .normalize() and then .isUnitVec(),
isUnitVec() should return true.

Ideally, a unit vector should only have a length of 1.0.
However, normalizing an arbitrary vector works by dividing its components by the length of the vector,
which involves calling fastSqrt(). The error for fastSqrt() is around 0.0005 (0.05%) of sqrt().
Even if we were using sqrt(), the precision of the floating point type is a factor as well, so unit vectors are unlikely to have a length of 1.0.

Because the ideal length of a unit vector is 1, the ideal squared length of a unit vector is also 1.
This means we can avoid calling fastSqrt() / sqrt() to check whether or not a vector is a unit vector.

However, unlike the length of a vector, the squared length minimum and maximum error amounts are different.
Though because they're so close to "1", they're nearly the same distance apart from 1 and differences between them can be ignored for our purposes.

We can calculate the minimum, ideal, and maximum bounds for the length and square lengths like so:
1 + 0.0005 = 1.0005, and 1.0005 * 1.0005 = 1.00100025
1 + 0      = 1,      and 1      * 1      = 1
1 - 0.0005 = 0.9995, and 0.9995 * 0.9995 = 0.99900025

From here we calculate the max error in the square length:
1.00100025 - 1 = 0.00100025
1 - 0.99900025 = 0.00099975

The upper bound is slightly higher than the lower bound, so we choose that for the max error in the square length of a unit vector.
*/
#define BS_VECTOR_UNIT_MAX_ERROR 0.00100025




//Macros
#define BS_VECTOR_DECLARE_METHODS( N )                                              \
    template< typename T2 >                                                         \
    Vector( const Point< T2, N >& to );                                             \
    Vector( const Point< T, N >& from, const Point< T, N >& to );                   \
                                                                                    \
    T getLength() const;                                                            \
    T getLengthSq() const;                                                          \
                                                                                    \
    void normalize();                                                               \
    bool isUnitVec() const;                                                         \
                                                                                    \
    Vector&    operator +=( const Vector& right );                                  \
    Vector&    operator -=( const Vector& right );                                  \
    Vector&    operator *=( const Vector& right );                                  \
    Vector&    operator /=( const Vector& right );                                  \
    Vector&    operator +=( const T right );                                        \
    Vector&    operator -=( const T right );                                        \
    Vector&    operator *=( const T right );                                        \
    Vector&    operator /=( const T right );                                        \
    explicit   operator Point< T, N >&();                                           \
    explicit   operator const Point< T, N >&() const;

#define BS_VECTOR_DEFINE_METHODS( N, tmpl )                                         \
    tmpl                                                                            \
    T Vector< T, N >::getLength() const {                                           \
        return (T)fastSqrt( (float)getLengthSq() );                                 \
    }                                                                               \
    tmpl                                                                            \
    void Vector< T, N >::normalize() {                                              \
        Private::normalize( *this );                                                \
    }                                                                               \
    tmpl                                                                            \
    bool Vector< T, N >::isUnitVec() const {                                        \
        return Private::isUnitVec( *this );                                         \
    }                                                                               \
    tmpl                                                                            \
    Vector< T, N >::operator Point< T, N >&() {                                     \
        return reinterpret_cast< Point< T, N >& >( *this );                         \
    }                                                                               \
    tmpl                                                                            \
    Vector< T, N >::operator const Point< T, N >&() const {                         \
        return reinterpret_cast< const Point< T, N >& >( *this );                   \
    }

namespace Brimstone {

template< typename T, size_t N >
class Vector;

namespace Private {

template< typename T, size_t N >
void normalize( Vector< T, N >& vecInOut );

template< size_t N >
void normalize( Vector< int8, N >& vecInOut );

template< size_t N >
void normalize( Vector< uint8, N >& vecInOut );

template< size_t N >
void normalize( Vector< int16, N >& vecInOut );

template< size_t N >
void normalize( Vector< uint16, N >& vecInOut );

template< size_t N >
void normalize( Vector< int32, N >& vecInOut );

template< size_t N >
void normalize( Vector< uint32, N >& vecInOut );

template< size_t N >
void normalize( Vector< int64, N >& vecInOut );

template< size_t N >
void normalize( Vector< uint64, N >& vecInOut );

template< size_t N >
void normalize( Vector< float, N >& vecInOut );

template< size_t N >
void normalize( Vector< double, N >& vecInOut );

template< typename T, size_t N >
void intNormalize( Vector< T, N >& vecInOut );

template< typename T, size_t N >
void floatNormalize( Vector< T, N >& vecInOut );

template< typename T, size_t N >
bool isUnitVec( const Vector< T, N >& vec );

template< size_t N >
bool isUnitVec( const Vector< int8, N >& vec );

template< size_t N >
bool isUnitVec( const Vector< uint8, N >& vec );

template< size_t N >
bool isUnitVec( const Vector< int16, N >& vec );

template< size_t N >
bool isUnitVec( const Vector< uint16, N >& vec );

template< size_t N >
bool isUnitVec( const Vector< int32, N >& vec );

template< size_t N >
bool isUnitVec( const Vector< uint32, N >& vec );

template< size_t N >
bool isUnitVec( const Vector< int64, N >& vec );

template< size_t N >
bool isUnitVec( const Vector< uint64, N >& vec );

template< size_t N >
bool isUnitVec( const Vector< float, N >& vec );

template< size_t N >
bool isUnitVec( const Vector< double, N >& vec );

template< typename T, size_t N >
bool intIsUnitVec( const Vector< T, N >& vec );

template< typename T, size_t N >
bool floatIsUnitVec( const Vector< T, N >& vec );

}

template< typename T, size_t N >
class Vector : public Private::BasePoint< T, N > {
private:
    typedef Private::BasePoint< T, N > BaseClass;
public:
    BS_ARRAY_DECLARE_INHERITED_METHODS( Vector, T )
    BS_BASEPOINT_DECLARE_INHERITED_METHODS( Vector, N, BS_SPEC_2( T2, N ) )
    BS_VECTOR_DECLARE_METHODS( N )
};
BS_ARRAY_DEFINE_INHERITED_METHODS( Vector, T, BaseClass, BS_TMPL_2( typename T, size_t N ), BS_SPEC_2( T, N ) )
BS_BASEPOINT_DEFINE_INHERITED_METHODS( Vector, N, BS_TMPL_2( typename T, size_t N ), BS_SPEC_2( T, N ), BS_SPEC_2( T2, N ) );
BS_VECTOR_DEFINE_METHODS( N, BS_TMPL_2( typename T, size_t N ) )

template< typename T, size_t N >
template< typename T2 >
Vector< T, N >::Vector( const Point< T2, N >& to ) {
    for( size_t i = 0; i < N; ++i )
        data[i] = static_cast< T >( to.data[i] );
}

template< typename T, size_t N >
Vector< T, N >::Vector( const Point< T, N >& from, const Point< T, N >& to ) {
    for( int i = 0; i < N; ++i )
        data[i] = to.data[i] - from.data[i];
}

template< typename T, size_t N >
T Vector< T, N >::getLengthSq() const {
    T lengthSq = 0;

    for( int i = 0; i < N; ++i )
        lengthSq += data[i] * data[i];

    return lengthSq;
}

template< typename T, size_t N >
Vector< T, N >& Vector< T, N >::operator +=( const Vector& right ) {
    for( int i = 0; i < N; ++i )
        data[i] += right.data[i];

    return ( *this );
}

template< typename T, size_t N >
Vector< T, N >& Vector< T, N >::operator -=( const Vector& right ) {
    for( int i = 0; i < N; ++i )
        data[i] -= right.data[i];

    return ( *this );
}

template< typename T, size_t N >
Vector< T, N >& Vector< T, N >::operator *=( const Vector& right ) {
    for( int i = 0; i < N; ++i )
        data[i] *= right.data[i];

    return ( *this );
}

template< typename T, size_t N >
Vector< T, N >& Vector< T, N >::operator /=( const Vector& right ) {
    for( int i = 0; i < N; ++i ) {
        BS_ASSERT_NONZERO_DIVISOR( right.data[i] );
        data[i] /= right.data[i];
    }

    return ( *this );
}

template< typename T, size_t N >
Vector< T, N >& Vector< T, N >::operator +=( const T right ) {
    for( int i = 0; i < N; ++i )
        data[i] += right;

    return ( *this );
}

template< typename T, size_t N >
Vector< T, N >& Vector< T, N >::operator -=( const T right ) {
    for( int i = 0; i < N; ++i )
        data[i] -= right;

    return ( *this );
}

template< typename T, size_t N >
Vector< T, N >& Vector< T, N >::operator *=( const T right ) {
    for( int i = 0; i < N; ++i )
        data[i] *= right;

    return ( *this );
}

template< typename T, size_t N >
Vector< T, N >& Vector< T, N >::operator /=( const T right ) {
    BS_ASSERT_NONZERO_DIVISOR( right );

    for( int i = 0; i < N; ++i )
        data[i] /= right;

    return ( *this );
}

template< typename T, size_t N >
std::ostream& operator <<( std::ostream& left, const Vector< T, N >& right ) {
    left << "< " << ( boost::format( "%|.5f|" ) % right.data[0] ).str();
    for( int i = 1; i < N; ++i )
        left << ", " << ( boost::format( "%|.5f|" ) % right.data[i] ).str();
    left << " >";

    return left;
}

template< typename T, size_t N >
bool operator ==( const Vector< T, N >& left, const Vector< T, N >& right ) {
    for( size_t i = 0; i < N; ++i )
        if( left.data[i] != right.data[i] )
            return false;
    return true;
}

template< typename T, size_t N >
bool operator !=( const Vector< T, N >& left, const Vector< T, N >& right ) {
    for( size_t i = 0; i < N; ++i )
        if( left.data[i] != right.data[i] )
            return true;
    return false;
}

template< typename T, size_t N >
Vector< T, N > operator +( const Vector< T, N >& right ) {
    return right;
}

template< typename T, size_t N >
Vector< T, N > operator -( const Vector< T, N >& right ) {
    Vector< T, N > out;

    for( int i = 0; i < N; ++i )
        out.data[i] = -right.data[i];

    return out;
}

template< typename T, size_t N >
Vector< T, N > operator +( const Vector< T, N >& left, const Vector< T, N >& right ) {
    Vector< T, N > out;

    for( int i = 0; i < N; ++i )
        out.data[i] = left.data[i] + right.data[i];

    return out;
}

template< typename T, size_t N >
Vector< T, N > operator -( const Vector< T, N >& left, const Vector< T, N >& right ) {
    Vector< T, N > out;

    for( int i = 0; i < N; ++i )
        out.data[i] = left.data[i] - right.data[i];

    return out;
}

template< typename T, size_t N >
Vector< T, N > operator *( const Vector< T, N >& left, const Vector< T, N >& right ) {
    Vector< T, N > out;

    for( int i = 0; i < N; ++i )
        out.data[i] = left.data[i] * right.data[i];

    return out;
}

template< typename T, size_t N >
Vector< T, N > operator /( const Vector< T, N >& left, const Vector< T, N >& right ) {
    Vector< T, N > out;

    for( int i = 0; i < N; ++i ) {
        BS_ASSERT_NONZERO_DIVISOR( right.data[i] );
        out.data[i] = left.data[i] / right.data[i];
    }

    return out;
}

template< typename T, size_t N >
Vector< T, N > operator +( const T left, const Vector< T, N >& right ) {
    Vector< T, N > out;

    for( int i = 0; i < N; ++i )
        out[i] = left + right[i];

    return out;
}

template< typename T, size_t N >
Vector< T, N > operator +( const Vector< T, N >& left, const T right ) {
    Vector< T, N > out;

    for( int i = 0; i < N; ++i )
        out[i] = left[i] + right;

    return out;
}

template< typename T, size_t N >
Vector< T, N > operator -( const Vector< T, N >& left, const T right ) {
    Vector< T, N > out;

    for( int i = 0; i < N; ++i )
        out[i] = left[i] - right;

    return out;
}

template< typename T, size_t N >
Vector< T, N > operator *( const T left, const Vector< T, N >& right ) {
    Vector< T, N > out;

    for( int i = 0; i < N; ++i )
        out[i] = left * right[i];

    return out;
}

template< typename T, size_t N >
Vector< T, N > operator *( const Vector< T, N >& left, const T right ) {
    Vector< T, N > out;

    for( int i = 0; i < N; ++i )
        out[i] = left[i] * right;

    return out;
}

template< typename T, size_t N >
Vector< T, N > operator /( const Vector< T, N >& left, const T right ) {
    BS_ASSERT_NONZERO_DIVISOR( right );

    Vector< T, N > out;

    for( int i = 0; i < N; ++i )
        out[i] = left[i] / right;

    return out;
}

template< typename T, size_t N >
T dot( const Vector< T, N >& left, const Vector< T, N >& right ) {
    T dp = 0;

    for( int i = 0; i < N; ++i )
        dp += left.data[i] * right.data[i];

    return dp;
}

namespace Private {

template< typename T, size_t N >
void normalize( const Vector< T, N >& vec ) {
    //By default, do nothing for exotic types
    //Make an override if you want it to do something special
}

template< size_t N >
void normalize( Vector< int8,   N >& vecInOut ) { intNormalize( vecInOut );   }
template< size_t N >
void normalize( Vector< uint8,  N >& vecInOut ) { intNormalize( vecInOut );   }
template< size_t N >
void normalize( Vector< int16,  N >& vecInOut ) { intNormalize( vecInOut );   }
template< size_t N >
void normalize( Vector< uint16, N >& vecInOut ) { intNormalize( vecInOut );   }
template< size_t N >
void normalize( Vector< int32,  N >& vecInOut ) { intNormalize( vecInOut );   }
template< size_t N >
void normalize( Vector< uint32, N >& vecInOut ) { intNormalize( vecInOut );   }
template< size_t N >
void normalize( Vector< int64,  N >& vecInOut ) { intNormalize( vecInOut );   }
template< size_t N >
void normalize( Vector< uint64, N >& vecInOut ) { intNormalize( vecInOut );   }
template< size_t N >
void normalize( Vector< float,  N >& vecInOut ) { floatNormalize( vecInOut ); }
template< size_t N >
void normalize( Vector< double, N >& vecInOut ) { floatNormalize( vecInOut ); }

template< typename T, size_t N >
void intNormalize( Vector< T, N >& vecInOut ) {
    BS_ASSERT_CAN_NORMALIZE( vecInOut );

    T* maxp   = vecInOut.data;
    T  maxmag = abs( vecInOut.data[0] );
    T  mag;

    //There are only 2 * N possible "normalized" vectors for integer vectors,
    //i.e. vectors with a length of 1, because integers can't represent floating point numbers.
    //We can't use the floating point version of normalize() for integer vectors because for most vectors,
    //such as (1,1), it would normalize the vector to some non-normal vector, such as (0,0), due to integer truncation.
    //So instead, we approximate the normal by finding the coordinate with the largest magnitude,
    //setting its magnitude to 1, and setting all other coordinates to 0.
    for( T* p = vecInOut.data + 1; p != vecInOut.data + N; ++p ) {
        mag = abs( *p );
        if( mag > maxmag ) {
            //Winners replace the incumbent as the new max coordinate.
            //The old max coordinate is set to 0.
            maxmag = mag;
            *maxp = 0;
            maxp = p;
        } else {
            //Losers get set to 0
            *p = 0;
        }
    }

    //Divide the max magnitude coordinate by its magnitude to set it to +1 or -1, depending on its sign.
    *maxp /= maxmag;
}


template< typename T, size_t N >
void floatNormalize( Vector< T, N >& vecInOut ) {
    BS_ASSERT_CAN_NORMALIZE( vecInOut );

    T invLen = fastInvSqrt( vecInOut.getLengthSq() );
    for( int i = 0; i < N; ++i )
        vecInOut.data[i] *= invLen;
}

template< typename T, size_t N >
bool isUnitVector( const Vector< T, N >& vec ) {
    //By default, return false for exotic types
    //Make an override if you want it to do something special
    return false;
}

template< size_t N >
bool isUnitVec( const Vector< int8,   N >& vec ) { return intIsUnitVec( vec );   }
template< size_t N >
bool isUnitVec( const Vector< uint8,  N >& vec ) { return intIsUnitVec( vec );   }
template< size_t N >
bool isUnitVec( const Vector< int16,  N >& vec ) { return intIsUnitVec( vec );   }
template< size_t N >
bool isUnitVec( const Vector< uint16, N >& vec ) { return intIsUnitVec( vec );   }
template< size_t N >
bool isUnitVec( const Vector< int32,  N >& vec ) { return intIsUnitVec( vec );   }
template< size_t N >
bool isUnitVec( const Vector< uint32, N >& vec ) { return intIsUnitVec( vec );   }
template< size_t N >
bool isUnitVec( const Vector< int64,  N >& vec ) { return intIsUnitVec( vec );   }
template< size_t N >
bool isUnitVec( const Vector< uint64, N >& vec ) { return intIsUnitVec( vec );   }
template< size_t N >
bool isUnitVec( const Vector< float,  N >& vec ) { return floatIsUnitVec( vec ); }
template< size_t N >
bool isUnitVec( const Vector< double, N >& vec ) { return floatIsUnitVec( vec ); }

template< typename T, size_t N >
bool intIsUnitVec( const Vector< T, N >& vec ) {
    return vec.getLengthSq() == (T)1;
}

template< typename T, size_t N >
bool floatIsUnitVec( const Vector< T, N >& vec ) {
    return abs( (T)1.0 - vec.getLengthSq() ) < BS_VECTOR_UNIT_MAX_ERROR;
}

}

}




#endif //BS_VECTOR_VECTORN_HPP