/*
VectorN.hpp
-----------------------
Copyright (c) 2014, theJ89

Description:
    Adds a generic vector class, VectorN.
    You can specify what type of data the vector uses (int, float, etc) as well as the dimension of the vector.
    2D and 3D specializations of VectorN are provided in Vector2.hpp and Vector3.hpp.
*/
#ifndef BS_VECTOR_VECTORN_HPP
#define BS_VECTOR_VECTORN_HPP




//Includes
#include <initializer_list>             //std::initializer_list
#include <cstdlib>                      //abs

#include <brimstone/Point.hpp>          //BasePointN
#include <brimstone/util/Math.hpp>      //fastSqrt, fastInvSqrt




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
    VectorN( const PointN< T2, N >& to );                                           \
    VectorN( const PointN< T, N >& from, const PointN< T, N >& to );                \
                                                                                    \
    T getLength() const;                                                            \
    T getLengthSq() const;                                                          \
                                                                                    \
    void normalize();                                                               \
    bool isUnitVec() const;                                                         \
                                                                                    \
    VectorN&    operator +=( const VectorN& right );                                \
    VectorN&    operator -=( const VectorN& right );                                \
    VectorN&    operator *=( const VectorN& right );                                \
    VectorN&    operator /=( const VectorN& right );                                \
    VectorN&    operator +=( const T right );                                       \
    VectorN&    operator -=( const T right );                                       \
    VectorN&    operator *=( const T right );                                       \
    VectorN&    operator /=( const T right );

#define BS_VECTOR_DEFINE_METHODS( N, tmpl )                                         \
    tmpl                                                                            \
    T VectorN< T, N >::getLength() const {                                          \
        return (T)fastSqrt( (float)getLengthSq() );                                 \
    }                                                                               \
    tmpl                                                                            \
    void VectorN< T, N >::normalize() {                                             \
        Private::normalize( *this );                                                \
    }                                                                               \
    tmpl                                                                            \
    bool VectorN< T, N >::isUnitVec() const {                                       \
        return Private::isUnitVec( *this );                                         \
    }                                                                               \

namespace Brimstone {

template< typename T, size_t N >
class VectorN;

namespace Private {

template< typename T, size_t N >
void normalize( VectorN< T, N >& vecInOut );

template< size_t N >
void normalize( VectorN< int8, N >& vecInOut );

template< size_t N >
void normalize( VectorN< uint8, N >& vecInOut );

template< size_t N >
void normalize( VectorN< int16, N >& vecInOut );

template< size_t N >
void normalize( VectorN< uint16, N >& vecInOut );

template< size_t N >
void normalize( VectorN< int32, N >& vecInOut );

template< size_t N >
void normalize( VectorN< uint32, N >& vecInOut );

template< size_t N >
void normalize( VectorN< int64, N >& vecInOut );

template< size_t N >
void normalize( VectorN< uint64, N >& vecInOut );

template< size_t N >
void normalize( VectorN< float, N >& vecInOut );

template< size_t N >
void normalize( VectorN< double, N >& vecInOut );

template< typename T, size_t N >
void intNormalize( VectorN< T, N >& vecInOut );

template< typename T, size_t N >
void floatNormalize( VectorN< T, N >& vecInOut );

template< typename T, size_t N >
bool isUnitVec( const VectorN< T, N >& vec );

template< size_t N >
bool isUnitVec( const VectorN< int8, N >& vec );

template< size_t N >
bool isUnitVec( const VectorN< uint8, N >& vec );

template< size_t N >
bool isUnitVec( const VectorN< int16, N >& vec );

template< size_t N >
bool isUnitVec( const VectorN< uint16, N >& vec );

template< size_t N >
bool isUnitVec( const VectorN< int32, N >& vec );

template< size_t N >
bool isUnitVec( const VectorN< uint32, N >& vec );

template< size_t N >
bool isUnitVec( const VectorN< int64, N >& vec );

template< size_t N >
bool isUnitVec( const VectorN< uint64, N >& vec );

template< size_t N >
bool isUnitVec( const VectorN< float, N >& vec );

template< size_t N >
bool isUnitVec( const VectorN< double, N >& vec );

template< typename T, size_t N >
bool intIsUnitVec( const VectorN< T, N >& vec );

template< typename T, size_t N >
bool floatIsUnitVec( const VectorN< T, N >& vec );

}

template< typename T, size_t N >
class VectorN : public Private::BasePointN< T, N > {
public:
    BS_POINT_DECLARE_INHERITED_METHODS( VectorN, N, BS_SPEC_2( T2, N ) )
    BS_VECTOR_DECLARE_METHODS( N )
};
BS_POINT_DEFINE_INHERITED_METHODS( VectorN, N, BS_TMPL_2( typename T, size_t N ), BS_SPEC_2( T, N ), BS_SPEC_2( T2, N ) );
BS_VECTOR_DEFINE_METHODS( N, BS_TMPL_2( typename T, size_t N ) )

template< typename T, size_t N >
template< typename T2 >
VectorN< T, N >::VectorN( const PointN< T2, N >& to ) {
    for( size_t i = 0; i < N; ++i )
        data[i] = static_cast< T >( to.data[i] );
}

template< typename T, size_t N >
VectorN< T, N >::VectorN( const PointN< T, N >& from, const PointN< T, N >& to ) {
    for( int i = 0; i < N; ++i )
        data[i] = to.data[i] - from.data[i];
}

template< typename T, size_t N >
T VectorN< T, N >::getLengthSq() const {
    T lengthSq = 0;

    for( int i = 0; i < N; ++i )
        lengthSq += data[i] * data[i];

    return lengthSq;
}

template< typename T, size_t N >
VectorN< T, N >& VectorN< T, N >::operator +=( const VectorN& right ) {
    for( int i = 0; i < N; ++i )
        data[i] += right.data[i];

    return ( *this );
}

template< typename T, size_t N >
VectorN< T, N >& VectorN< T, N >::operator -=( const VectorN& right ) {
    for( int i = 0; i < N; ++i )
        data[i] -= right.data[i];

    return ( *this );
}

template< typename T, size_t N >
VectorN< T, N >& VectorN< T, N >::operator *=( const VectorN& right ) {
    for( int i = 0; i < N; ++i )
        data[i] *= right.data[i];

    return ( *this );
}

template< typename T, size_t N >
VectorN< T, N >& VectorN< T, N >::operator /=( const VectorN& right ) {
    for( int i = 0; i < N; ++i ) {
        BS_ASSERT_NONZERO_DIVISOR( right.data[i] );
        data[i] /= right.data[i];
    }

    return ( *this );
}

template< typename T, size_t N >
VectorN< T, N >& VectorN< T, N >::operator +=( const T right ) {
    for( int i = 0; i < N; ++i )
        data[i] += right;

    return ( *this );
}

template< typename T, size_t N >
VectorN< T, N >& VectorN< T, N >::operator -=( const T right ) {
    for( int i = 0; i < N; ++i )
        data[i] -= right;

    return ( *this );
}

template< typename T, size_t N >
VectorN< T, N >& VectorN< T, N >::operator *=( const T right ) {
    for( int i = 0; i < N; ++i )
        data[i] *= right;

    return ( *this );
}

template< typename T, size_t N >
VectorN< T, N >& VectorN< T, N >::operator /=( const T right ) {
    BS_ASSERT_NONZERO_DIVISOR( right );

    for( int i = 0; i < N; ++i )
        data[i] /= right;

    return ( *this );
}

template< typename T, size_t N >
std::ostream& operator <<( std::ostream& left, const VectorN< T, N >& right ) {
    left << "< ";

    left << right.data[0];
    for( int i = 1; i < N; ++i )
        left << ", " << right.data[i];

    left << " >";

    return left;
}

template< typename T, size_t N >
bool operator ==( const VectorN< T, N >& left, const VectorN< T, N >& right ) {
    for( size_t i = 0; i < N; ++i )
        if( left.data[i] != right.data[i] )
            return false;
    return true;
}

template< typename T, size_t N >
bool operator !=( const VectorN< T, N >& left, const VectorN< T, N >& right ) {
    for( size_t i = 0; i < N; ++i )
        if( left.data[i] != right.data[i] )
            return true;
    return false;
}

template< typename T, size_t N >
VectorN< T, N > operator +( const VectorN< T, N >& right ) {
    return right;
}

template< typename T, size_t N >
VectorN< T, N > operator -( const VectorN< T, N >& right ) {
    VectorN< T, N > out;

    for( int i = 0; i < N; ++i )
        out.data[i] = -right.data[i];

    return out;
}

template< typename T, size_t N >
VectorN< T, N > operator +( const VectorN< T, N >& left, const VectorN< T, N >& right ) {
    VectorN< T, N > out;

    for( int i = 0; i < N; ++i )
        out.data[i] = left.data[i] + right.data[i];

    return out;
}

template< typename T, size_t N >
VectorN< T, N > operator -( const VectorN< T, N >& left, const VectorN< T, N >& right ) {
    VectorN< T, N > out;

    for( int i = 0; i < N; ++i )
        out.data[i] = left.data[i] - right.data[i];

    return out;
}

template< typename T, size_t N >
VectorN< T, N > operator *( const VectorN< T, N >& left, const VectorN< T, N >& right ) {
    VectorN< T, N > out;

    for( int i = 0; i < N; ++i )
        out.data[i] = left.data[i] * right.data[i];

    return out;
}

template< typename T, size_t N >
VectorN< T, N > operator /( const VectorN< T, N >& left, const VectorN< T, N >& right ) {
    VectorN< T, N > out;

    for( int i = 0; i < N; ++i ) {
        BS_ASSERT_NONZERO_DIVISOR( right.data[i] );
        out.data[i] = left.data[i] / right.data[i];
    }

    return out;
}

template< typename T, size_t N >
VectorN< T, N > operator +( const T left, const VectorN< T, N >& right ) {
    VectorN< T, N > out;

    for( int i = 0; i < N; ++i )
        out[i] = left + right[i];

    return out;
}

template< typename T, size_t N >
VectorN< T, N > operator +( const VectorN< T, N >& left, const T right ) {
    VectorN< T, N > out;

    for( int i = 0; i < N; ++i )
        out[i] = left[i] + right;

    return out;
}

template< typename T, size_t N >
VectorN< T, N > operator -( const VectorN< T, N >& left, const T right ) {
    VectorN< T, N > out;

    for( int i = 0; i < N; ++i )
        out[i] = left[i] - right;

    return out;
}

template< typename T, size_t N >
VectorN< T, N > operator *( const T left, const VectorN< T, N >& right ) {
    VectorN< T, N > out;

    for( int i = 0; i < N; ++i )
        out[i] = left * right[i];

    return out;
}

template< typename T, size_t N >
VectorN< T, N > operator *( const VectorN< T, N >& left, const T right ) {
    VectorN< T, N > out;

    for( int i = 0; i < N; ++i )
        out[i] = left[i] * right;

    return out;
}

template< typename T, size_t N >
VectorN< T, N > operator /( const VectorN< T, N >& left, const T right ) {
    BS_ASSERT_NONZERO_DIVISOR( right );

    VectorN< T, N > out;

    for( int i = 0; i < N; ++i )
        out[i] = left[i] / right;

    return out;
}

template< typename T, size_t N >
T dot( const VectorN< T, N >& left, const VectorN< T, N >& right ) {
    T dp = 0;

    for( int i = 0; i < N; ++i )
        dp += left.data[i] * right.data[i];

    return dp;
}

namespace Private {

template< typename T, size_t N >
void normalize( const VectorN< T, N >& vec ) {
    //By default, do nothing for exotic types
    //Make an override if you want it to do something special
}

template< size_t N >
void normalize( VectorN< int8,   N >& vecInOut ) { intNormalize( vecInOut );   }
template< size_t N >
void normalize( VectorN< uint8,  N >& vecInOut ) { intNormalize( vecInOut );   }
template< size_t N >
void normalize( VectorN< int16,  N >& vecInOut ) { intNormalize( vecInOut );   }
template< size_t N >
void normalize( VectorN< uint16, N >& vecInOut ) { intNormalize( vecInOut );   }
template< size_t N >
void normalize( VectorN< int32,  N >& vecInOut ) { intNormalize( vecInOut );   }
template< size_t N >
void normalize( VectorN< uint32, N >& vecInOut ) { intNormalize( vecInOut );   }
template< size_t N >
void normalize( VectorN< int64,  N >& vecInOut ) { intNormalize( vecInOut );   }
template< size_t N >
void normalize( VectorN< uint64, N >& vecInOut ) { intNormalize( vecInOut );   }
template< size_t N >
void normalize( VectorN< float,  N >& vecInOut ) { floatNormalize( vecInOut ); }
template< size_t N >
void normalize( VectorN< double, N >& vecInOut ) { floatNormalize( vecInOut ); }

template< typename T, size_t N >
void intNormalize( VectorN< T, N >& vecInOut ) {
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
void floatNormalize( VectorN< T, N >& vecInOut ) {
    BS_ASSERT_CAN_NORMALIZE( vecInOut );

    T invLen = fastInvSqrt( vecInOut.getLengthSq() );
    for( int i = 0; i < N; ++i )
        vecInOut.data[i] *= invLen;
}

template< typename T, size_t N >
bool isUnitVector( const VectorN< T, N >& vec ) {
    //By default, return false for exotic types
    //Make an override if you want it to do something special
    return false;
}

template< size_t N >
bool isUnitVec( const VectorN< int8,   N >& vec ) { return intIsUnitVec( vec );   }
template< size_t N >
bool isUnitVec( const VectorN< uint8,  N >& vec ) { return intIsUnitVec( vec );   }
template< size_t N >
bool isUnitVec( const VectorN< int16,  N >& vec ) { return intIsUnitVec( vec );   }
template< size_t N >
bool isUnitVec( const VectorN< uint16, N >& vec ) { return intIsUnitVec( vec );   }
template< size_t N >
bool isUnitVec( const VectorN< int32,  N >& vec ) { return intIsUnitVec( vec );   }
template< size_t N >
bool isUnitVec( const VectorN< uint32, N >& vec ) { return intIsUnitVec( vec );   }
template< size_t N >
bool isUnitVec( const VectorN< int64,  N >& vec ) { return intIsUnitVec( vec );   }
template< size_t N >
bool isUnitVec( const VectorN< uint64, N >& vec ) { return intIsUnitVec( vec );   }
template< size_t N >
bool isUnitVec( const VectorN< float,  N >& vec ) { return floatIsUnitVec( vec ); }
template< size_t N >
bool isUnitVec( const VectorN< double, N >& vec ) { return floatIsUnitVec( vec ); }

template< typename T, size_t N >
bool intIsUnitVec( const VectorN< T, N >& vec ) {
    return vec.getLengthSq() == (T)1;
}

template< typename T, size_t N >
bool floatIsUnitVec( const VectorN< T, N >& vec ) {
    return abs( (T)1.0 - vec.getLengthSq() ) < BS_VECTOR_UNIT_MAX_ERROR;
}

}

}




#endif //BS_VECTOR_VECTORN_HPP