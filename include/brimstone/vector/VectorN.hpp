/*
vector/VectorN.hpp
------------------
Copyright (c) 2024, theJ89

Description:
    Adds a generic vector class, Vector.
    You can specify what type of data the vector uses (int, float, etc) as well as the dimension of the vector.
    2D, 3D and 4D specializations of Vector are provided in Vector2.hpp, Vector3.hpp, and Vector4.hpp, respectively.
*/
#ifndef BS_VECTOR_VECTORN_HPP
#define BS_VECTOR_VECTORN_HPP




//Includes
#include <cstddef>                    //std::size_t
#include <cstdlib>                    //std::abs
#include <initializer_list>           //std::initializer_list

#include <brimstone/Point.hpp>        //Brimstone::BasePoint
#include <brimstone/util/Math.hpp>    //Brimstone::fastSqrt, Brimstone::fastInvSqrt
#include <brimstone/util/MinMax.hpp>  //Brimstone::electMax




//Defines
/*
NOTE: These defines control how much error isUnitVec() tolerates.

isUnitVec() returns true if the square length of the vector is between 0.99900025 (1.0 - 0.00099975) and 1.00100025 (1.0 + 0.00100025).
These values are chosen with this in mind: For any vector, if you call .normalize() and then .isUnitVec(),
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
*/
#define BS_VECTOR_UNIT_MIN 0.99900025
#define BS_VECTOR_UNIT_MAX 1.00100025




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
    Vector getNormal() const;                                                       \
    bool isUnitVec() const;                                                         \
                                                                                    \
    void invert();                                                                  \
    void negate();                                                                  \
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
    Vector< T, N> Vector< T, N >::getNormal() const {                               \
        auto copy = Vector< T, N >( *this );                                        \
        copy.normalize();                                                           \
        return copy;                                                                \
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




//Forward declarations
template< typename T, std::size_t N >
class Vector;




} //namespace Brimstone




namespace Brimstone::Private {




//Forward declarations
template< typename T, std::size_t N >
void normalize( Vector< T, N >& vecInOut );

template< std::size_t N >
void normalize( Vector< int8, N >& vecInOut );

template< std::size_t N >
void normalize( Vector< uint8, N >& vecInOut );

template< std::size_t N >
void normalize( Vector< int16, N >& vecInOut );

template< std::size_t N >
void normalize( Vector< uint16, N >& vecInOut );

template< std::size_t N >
void normalize( Vector< int32, N >& vecInOut );

template< std::size_t N >
void normalize( Vector< uint32, N >& vecInOut );

template< std::size_t N >
void normalize( Vector< int64, N >& vecInOut );

template< std::size_t N >
void normalize( Vector< uint64, N >& vecInOut );

template< std::size_t N >
void normalize( Vector< float, N >& vecInOut );

template< std::size_t N >
void normalize( Vector< double, N >& vecInOut );

template< typename T, std::size_t N >
void intNormalize( Vector< T, N >& vecInOut );

template< typename T, std::size_t N >
void floatNormalize( Vector< T, N >& vecInOut );

template< typename T, std::size_t N >
bool isUnitVec( const Vector< T, N >& vec );

template< std::size_t N >
bool isUnitVec( const Vector< int8, N >& vec );

template< std::size_t N >
bool isUnitVec( const Vector< uint8, N >& vec );

template< std::size_t N >
bool isUnitVec( const Vector< int16, N >& vec );

template< std::size_t N >
bool isUnitVec( const Vector< uint16, N >& vec );

template< std::size_t N >
bool isUnitVec( const Vector< int32, N >& vec );

template< std::size_t N >
bool isUnitVec( const Vector< uint32, N >& vec );

template< std::size_t N >
bool isUnitVec( const Vector< int64, N >& vec );

template< std::size_t N >
bool isUnitVec( const Vector< uint64, N >& vec );

template< std::size_t N >
bool isUnitVec( const Vector< float, N >& vec );

template< std::size_t N >
bool isUnitVec( const Vector< double, N >& vec );

template< typename T, std::size_t N >
bool intIsUnitVec( const Vector< T, N >& vec );

template< typename T, std::size_t N >
bool floatIsUnitVec( const Vector< T, N >& vec );




} //namespace Brimstone::Private




namespace Brimstone {




template< typename T, std::size_t N >
class Vector {
public:
    T data[N];
public:
    BS_ARRAY_DECLARE_INHERITED_METHODS( Vector, T    )
    BS_ARRAY_DECLARE_METHODS(           Vector, T    )
    BS_BASEPOINT_DECLARE_METHODS(       Vector,    N )
    BS_VECTOR_DECLARE_METHODS( N )
};
BS_ARRAY_DEFINE_GENERIC_METHODS( Vector, T,    data, BS_TMPL_2( typename T, std::size_t N ), BS_SPEC_2( T, N ) )
BS_ARRAY_DEFINE_METHODS(         Vector, T,    data, BS_TMPL_2( typename T, std::size_t N ), BS_SPEC_2( T, N ) )
BS_BASEPOINTN_DEFINE_METHODS(    Vector                                                                        )
BS_VECTOR_DEFINE_METHODS(                   N,       BS_TMPL_2( typename T, std::size_t N )                    )




//Forward declarations
template< typename T, std::size_t N >
T dot( const Vector< T, N >& left, const Vector< T, N >& right );
template< typename T, std::size_t N >
Vector< T, N > invert( const Vector< T, N >& vector );




template< typename T, std::size_t N >
Vector< T, N >::Vector() {
#ifdef BS_ZERO
    std::fill( std::begin( data ), std::end( data ), static_cast< T >( 0 ) );
#endif //BS_ZERO
}

template< typename T, std::size_t N >
template< typename T2 >
Vector< T, N >::Vector( const Point< T2, N >& to ) {
    for( std::size_t i = 0; i < N; ++i )
        data[i] = static_cast< T >( to.data[i] );
}

template< typename T, std::size_t N >
Vector< T, N >::Vector( const Point< T, N >& from, const Point< T, N >& to ) {
    for( std::size_t i = 0; i < N; ++i )
        data[i] = to.data[i] - from.data[i];
}

template< typename T, std::size_t N >
T Vector< T, N >::getLengthSq() const {
    T lengthSq = 0;

    for( std::size_t i = 0; i < N; ++i )
        lengthSq += data[i] * data[i];

    return lengthSq;
}

#if defined( BS_BUILD_LINUX ) && !defined( BS_BUILD_64BIT ) && !defined( BS_BUILD_DEBUG )

namespace Private {

//Meant to fix a G++ optimization problem (bug?) that only occurs on the 32-bit release (-O3) build
//It only seems to affect this particular method, in this particular class (specializations aren't affected)
//Having the operation performed on a volatile vector seems to work around the problem.
//The only real way this differs from any other similar function is that it does "data[i] = static_cast< T > / data[i];"
//Maybe it's got something to do with that?
template< typename T, std::size_t N >
void invertImpl( volatile Vector< T, N >& v ) {
    const T one = static_cast< T >( 1 );
    for( std::size_t i = 0; i < N; ++i ) {
        BS_ASSERT_NONZERO_DIVISOR( v.data[i] );
        v.data[i] = one / v.data[i];
    }
}

}

#endif

template< typename T, std::size_t N >
void Vector< T, N >::invert() {
    #if defined( BS_BUILD_LINUX ) && !defined( BS_BUILD_64BIT ) && !defined( BS_BUILD_DEBUG )
    Private::invertImpl( *this );
    #else

    const T one = static_cast< T >( 1 );
    for( std::size_t i = 0; i < N; ++i ) {
        BS_ASSERT_NONZERO_DIVISOR( data[i] );
        data[i] = one / data[i];
    }

    #endif
}

template< typename T, std::size_t N >
void Vector< T, N >::negate() {
    for( std::size_t i = 0; i < N; ++i )
        data[i] = -data[i];
}

template< typename T, std::size_t N >
Vector< T, N >& Vector< T, N >::operator +=( const Vector& right ) {
    for( std::size_t i = 0; i < N; ++i )
        data[i] += right.data[i];

    return ( *this );
}

template< typename T, std::size_t N >
Vector< T, N >& Vector< T, N >::operator -=( const Vector& right ) {
    for( std::size_t i = 0; i < N; ++i )
        data[i] -= right.data[i];

    return ( *this );
}

template< typename T, std::size_t N >
Vector< T, N >& Vector< T, N >::operator *=( const Vector& right ) {
    for( std::size_t i = 0; i < N; ++i )
        data[i] *= right.data[i];

    return ( *this );
}

template< typename T, std::size_t N >
Vector< T, N >& Vector< T, N >::operator /=( const Vector& right ) {
    for( std::size_t i = 0; i < N; ++i ) {
        BS_ASSERT_NONZERO_DIVISOR( right.data[i] );
        data[i] /= right.data[i];
    }

    return ( *this );
}

template< typename T, std::size_t N >
Vector< T, N >& Vector< T, N >::operator +=( const T right ) {
    for( std::size_t i = 0; i < N; ++i )
        data[i] += right;

    return ( *this );
}

template< typename T, std::size_t N >
Vector< T, N >& Vector< T, N >::operator -=( const T right ) {
    for( std::size_t i = 0; i < N; ++i )
        data[i] -= right;

    return ( *this );
}

template< typename T, std::size_t N >
Vector< T, N >& Vector< T, N >::operator *=( const T right ) {
    for( std::size_t i = 0; i < N; ++i )
        data[i] *= right;

    return ( *this );
}

template< typename T, std::size_t N >
Vector< T, N >& Vector< T, N >::operator /=( const T right ) {
    BS_ASSERT_NONZERO_DIVISOR( right );

    for( std::size_t i = 0; i < N; ++i )
        data[i] /= right;

    return ( *this );
}

template< typename T, std::size_t N >
std::ostream& operator <<( std::ostream& left, const Vector< T, N >& right ) {
    left << "< "
         << ( boost::format( "%|.5f|" ) % right.data[0] ).str();
    for( std::size_t i = 1; i < N; ++i )
        left << ", " << ( boost::format( "%|.5f|" ) % right.data[i] ).str();
    left << " >";

    return left;
}

template< typename T, std::size_t N >
bool operator ==( const Vector< T, N >& left, const Vector< T, N >& right ) {
    for( std::size_t i = 0; i < N; ++i )
        if( left.data[i] != right.data[i] )
            return false;
    return true;
}

template< typename T, std::size_t N >
bool operator !=( const Vector< T, N >& left, const Vector< T, N >& right ) {
    for( std::size_t i = 0; i < N; ++i )
        if( left.data[i] != right.data[i] )
            return true;
    return false;
}

template< typename T, std::size_t N >
Vector< T, N > operator +( const Vector< T, N >& right ) {
    return right;
}

template< typename T, std::size_t N >
Vector< T, N > operator -( const Vector< T, N >& right ) {
    Vector< T, N > out;

    for( std::size_t i = 0; i < N; ++i )
        out.data[i] = -right.data[i];

    return out;
}

template< typename T, std::size_t N >
Vector< T, N > operator +( const Vector< T, N >& left, const Vector< T, N >& right ) {
    Vector< T, N > out;

    for( std::size_t i = 0; i < N; ++i )
        out.data[i] = left.data[i] + right.data[i];

    return out;
}

template< typename T, std::size_t N >
Vector< T, N > operator -( const Vector< T, N >& left, const Vector< T, N >& right ) {
    Vector< T, N > out;

    for( std::size_t i = 0; i < N; ++i )
        out.data[i] = left.data[i] - right.data[i];

    return out;
}

template< typename T, std::size_t N >
Vector< T, N > operator *( const Vector< T, N >& left, const Vector< T, N >& right ) {
    Vector< T, N > out;

    for( std::size_t i = 0; i < N; ++i )
        out.data[i] = left.data[i] * right.data[i];

    return out;
}

template< typename T, std::size_t N >
Vector< T, N > operator /( const Vector< T, N >& left, const Vector< T, N >& right ) {
    Vector< T, N > out;

    for( std::size_t i = 0; i < N; ++i ) {
        BS_ASSERT_NONZERO_DIVISOR( right.data[i] );
        out.data[i] = left.data[i] / right.data[i];
    }

    return out;
}

template< typename T, std::size_t N >
Vector< T, N > operator +( const T left, const Vector< T, N >& right ) {
    Vector< T, N > out;

    for( std::size_t i = 0; i < N; ++i )
        out[i] = left + right[i];

    return out;
}

template< typename T, std::size_t N >
Vector< T, N > operator +( const Vector< T, N >& left, const T right ) {
    Vector< T, N > out;

    for( std::size_t i = 0; i < N; ++i )
        out[i] = left[i] + right;

    return out;
}

template< typename T, std::size_t N >
Vector< T, N > operator -( const Vector< T, N >& left, const T right ) {
    Vector< T, N > out;

    for( std::size_t i = 0; i < N; ++i )
        out[i] = left[i] - right;

    return out;
}

template< typename T, std::size_t N >
Vector< T, N > operator *( const T left, const Vector< T, N >& right ) {
    Vector< T, N > out;

    for( std::size_t i = 0; i < N; ++i )
        out[i] = left * right[i];

    return out;
}

template< typename T, std::size_t N >
Vector< T, N > operator *( const Vector< T, N >& left, const T right ) {
    Vector< T, N > out;

    for( std::size_t i = 0; i < N; ++i )
        out[i] = left[i] * right;

    return out;
}

template< typename T, std::size_t N >
Vector< T, N > operator /( const Vector< T, N >& left, const T right ) {
    BS_ASSERT_NONZERO_DIVISOR( right );

    Vector< T, N > out;

    for( std::size_t i = 0; i < N; ++i )
        out[i] = left[i] / right;

    return out;
}

template< typename T, std::size_t N >
Vector< T, N > operator -( const Point< T, N >& left, const Point< T, N >& right ) {
    Vector< T, N > out;

    for( std::size_t i = 0; i < N; ++i )
        out.data[i] = left.data[i] - right.data[i];

    return out;
}

template< typename T, std::size_t N >
Point< T, N > operator +( const Point< T, N >& left, const Vector< T, N >& right ) {
    Point< T, N > out;

    for( std::size_t i = 0; i < N; ++i )
        out.data[i] = left.data[i] + right.data[i];

    return out;
}

template< typename T, std::size_t N >
Point< T, N > operator +( const Vector< T, N >& left, const Point< T, N >& right ) {
    Point< T, N > out;

    for( std::size_t i = 0; i < N; ++i )
        out.data[i] = left.data[i] + right.data[i];

    return out;
}

template< typename T, std::size_t N >
Point< T, N > operator -( const Point< T, N >& left, const Vector< T, N >& right ) {
    Point< T, N > out;

    for( std::size_t i = 0; i < N; ++i )
        out.data[i] = left.data[i] - right.data[i];

    return out;
}

template< typename T, std::size_t N >
Vector< T, N > invert( const Vector< T, N >& vector ) {
    Vector< T, N > out;

    const T one = static_cast< T >( 1 );
    for( std::size_t i = 0; i < N; ++i ) {
        BS_ASSERT_NONZERO_DIVISOR( vector.data[i] );
        out.data[i] = one / vector.data[i];
    }

    return out;
}

template< typename T, std::size_t N >
T dot( const Vector< T, N >& left, const Vector< T, N >& right ) {
    T dp = 0;

    for( std::size_t i = 0; i < N; ++i )
        dp += left.data[i] * right.data[i];

    return dp;
}




} //namespace Brimstone




namespace Brimstone::Private {




template< typename T, std::size_t N >
void normalize( const Vector< T, N >& vec ) {
    //By default, do nothing for exotic types
    //Make an override if you want it to do something special
}

template< std::size_t N >
void normalize( Vector< int8,   N >& vecInOut ) { intNormalize( vecInOut );   }
template< std::size_t N >
void normalize( Vector< uint8,  N >& vecInOut ) { intNormalize( vecInOut );   }
template< std::size_t N >
void normalize( Vector< int16,  N >& vecInOut ) { intNormalize( vecInOut );   }
template< std::size_t N >
void normalize( Vector< uint16, N >& vecInOut ) { intNormalize( vecInOut );   }
template< std::size_t N >
void normalize( Vector< int32,  N >& vecInOut ) { intNormalize( vecInOut );   }
template< std::size_t N >
void normalize( Vector< uint32, N >& vecInOut ) { intNormalize( vecInOut );   }
template< std::size_t N >
void normalize( Vector< int64,  N >& vecInOut ) { intNormalize( vecInOut );   }
template< std::size_t N >
void normalize( Vector< uint64, N >& vecInOut ) { intNormalize( vecInOut );   }
template< std::size_t N >
void normalize( Vector< float,  N >& vecInOut ) { floatNormalize( vecInOut ); }
template< std::size_t N >
void normalize( Vector< double, N >& vecInOut ) { floatNormalize( vecInOut ); }

template< typename T, std::size_t N >
void intNormalize( Vector< T, N >& vecInOut ) {
    BS_ASSERT_CAN_NORMALIZE( vecInOut );

    T* maxp   = vecInOut.data;
    T  maxmag = std::abs( vecInOut.data[0] );
    T  mag;

    //There are only 2 * N possible "normalized" vectors for integer vectors,
    //i.e. vectors with a length of 1, because integers can't represent floating point numbers.
    //We can't use the floating point version of normalize() for integer vectors because for most vectors,
    //such as (1,1), it would normalize the vector to some non-normal vector, such as (0,0), due to integer truncation.
    //So instead, we approximate the normal by finding the coordinate with the largest magnitude,
    //setting its magnitude to 1, and setting all other coordinates to 0.
    for( T* p = vecInOut.data + 1; p != vecInOut.data + N; ++p ) {
        mag = std::abs( *p );
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


template< typename T, std::size_t N >
void floatNormalize( Vector< T, N >& vecInOut ) {
    BS_ASSERT_CAN_NORMALIZE( vecInOut );

    T invLen = fastInvSqrt( vecInOut.getLengthSq() );
    for( std::size_t i = 0; i < N; ++i )
        vecInOut.data[i] *= invLen;
}

template< typename T, std::size_t N >
bool isUnitVector( const Vector< T, N >& vec ) {
    //By default, return false for exotic types
    //Make an override if you want it to do something special
    return false;
}

template< std::size_t N >
bool isUnitVec( const Vector< int8,   N >& vec ) { return intIsUnitVec( vec );   }
template< std::size_t N >
bool isUnitVec( const Vector< uint8,  N >& vec ) { return intIsUnitVec( vec );   }
template< std::size_t N >
bool isUnitVec( const Vector< int16,  N >& vec ) { return intIsUnitVec( vec );   }
template< std::size_t N >
bool isUnitVec( const Vector< uint16, N >& vec ) { return intIsUnitVec( vec );   }
template< std::size_t N >
bool isUnitVec( const Vector< int32,  N >& vec ) { return intIsUnitVec( vec );   }
template< std::size_t N >
bool isUnitVec( const Vector< uint32, N >& vec ) { return intIsUnitVec( vec );   }
template< std::size_t N >
bool isUnitVec( const Vector< int64,  N >& vec ) { return intIsUnitVec( vec );   }
template< std::size_t N >
bool isUnitVec( const Vector< uint64, N >& vec ) { return intIsUnitVec( vec );   }
template< std::size_t N >
bool isUnitVec( const Vector< float,  N >& vec ) { return floatIsUnitVec( vec ); }
template< std::size_t N >
bool isUnitVec( const Vector< double, N >& vec ) { return floatIsUnitVec( vec ); }

template< typename T, std::size_t N >
bool intIsUnitVec( const Vector< T, N >& vec ) {
    return vec.getLengthSq() == (T)1;
}

template< typename T, std::size_t N >
bool floatIsUnitVec( const Vector< T, N >& vec ) {
    auto lengthSq = vec.getLengthSq();
    return BS_VECTOR_UNIT_MIN < lengthSq && lengthSq < BS_VECTOR_UNIT_MAX;
}




} //namespace Brimstone::Private




#endif //BS_VECTOR_VECTORN_HPP
