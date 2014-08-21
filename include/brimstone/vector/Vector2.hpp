/*
Vector2.hpp
-----------------------
Copyright (c) 2014, theJ89

Description:
    Specialization of VectorN for 2D vectors.
    Adds the following typedefs for convenience:
        Vector2< T >: VectorN<T,2>
        Vector2i:     VectorN<int32,2>
        Vector2f:     VectorN<float,2>
        Vector2d:     VectorN<double,2>
*/
#ifndef BS_VECTOR_VECTOR2_HPP
#define BS_VECTOR_VECTOR2_HPP




//Includes
#include <brimstone/vector/VectorN.hpp>




namespace Brimstone {

namespace Private {

template< typename T >
void intNormalize( VectorN< T, 2 >& vecInOut );

template< typename T >
void floatNormalize( VectorN< T, 2 >& vecInOut );

}

template< typename T >
class VectorN< T, 2 > : public Private::BasePointN< T, 2 > {
public:
    BS_POINT2_DECLARE_INHERITED_METHODS( VectorN, BS_SPEC_2( T2, 2 ) )
    BS_VECTOR_DECLARE_METHODS( 2 )
};
BS_POINT2_DEFINE_INHERITED_METHODS( VectorN, BS_TMPL_1( typename T ), BS_SPEC_2( T, 2 ), BS_SPEC_2( T2, 2 ) )
BS_VECTOR_DEFINE_METHODS( 2, BS_TMPL_1( typename T ) )

template< typename T >
template< typename T2 >
VectorN< T, 2 >::VectorN( const PointN< T2, 2 >& to ) :
    BasePointN(
        static_cast< T >( to.x ),
        static_cast< T >( to.y )
    ) {
}

template< typename T >
VectorN< T, 2 >::VectorN( const PointN< T, 2 >& from, const PointN< T, 2 >& to ) :
    BasePointN(
        ( to.x - from.x ),
        ( to.y - from.y )
    ) {
}

template< typename T >
T VectorN< T, 2 >::getLengthSq() const {
    return x * x +
           y * y;
}

template< typename T >
VectorN< T, 2 >& VectorN< T, 2 >::operator +=( const VectorN& right ) {
    x += right.x;
    y += right.y;

    return ( *this );
}

template< typename T >
VectorN< T, 2 >& VectorN< T, 2 >::operator -=( const VectorN& right ) {
    x -= right.x;
    y -= right.y;

    return ( *this );
}

template< typename T >
VectorN< T, 2 >& VectorN< T, 2 >::operator *=( const VectorN& right ) {
    x *= right.x;
    y *= right.y;

    return ( *this );
}

template< typename T >
VectorN< T, 2 >& VectorN< T, 2 >::operator /=( const VectorN& right ) {
    BS_ASSERT_NONZERO_DIVISOR( right.x );
    BS_ASSERT_NONZERO_DIVISOR( right.y );

    x /= right.x;
    y /= right.y;

    return ( *this );
}

template< typename T >
VectorN< T, 2 >& VectorN< T, 2 >::operator +=( const T right ) {
    x += right;
    y += right;

    return ( *this );
}

template< typename T >
VectorN< T, 2 >& VectorN< T, 2 >::operator -=( const T right ) {
    x -= right;
    y -= right;

    return ( *this );
}

template< typename T >
VectorN< T, 2 >& VectorN< T, 2 >::operator *=( const T right ) {
    x *= right;
    y *= right;

    return ( *this );
}

template< typename T >
VectorN< T, 2 >& VectorN< T, 2 >::operator /=( const T right ) {
    BS_ASSERT_NONZERO_DIVISOR( right );

    x /= right;
    y /= right;

    return ( *this );
}

template< typename T >
std::ostream& operator <<( std::ostream& left, const VectorN< T, 2 >& right ) {
    return left << "< " << right.x << ", " << right.y << " >";
}

template< typename T >
VectorN< T, 2 > operator -( const VectorN< T, 2 >& right ) {
    return VectorN< T, 2 >(
        -right.x,
        -right.y
    );
}

template< typename T >
VectorN< T, 2 > operator +( const VectorN< T, 2 >& left, const VectorN< T, 2 >& right ) {
    return VectorN< T, 2 >(
        left.x + right.x,
        left.y + right.y
    );
}

template< typename T >
VectorN< T, 2 > operator -( const VectorN< T, 2 >& left, const VectorN< T, 2 >& right ) {
    return VectorN< T, 2 >(
        left.x - right.x,
        left.y - right.y
    );
}

template< typename T >
VectorN< T, 2 > operator *( const VectorN< T, 2 >& left, const VectorN< T, 2 >& right ) {
    return VectorN< T, 2 >(
        left.x * right.x,
        left.y * right.y
    );
}

template< typename T >
VectorN< T, 2 > operator /( const VectorN< T, 2 >& left, const VectorN< T, 2 >& right ) {
    BS_ASSERT_NONZERO_DIVISOR( right.x );
    BS_ASSERT_NONZERO_DIVISOR( right.y );

    return VectorN< T, 2 >(
        left.x / right.x,
        left.y / right.y
    );
}

template< typename T >
VectorN< T, 2 > operator +( const T left, const VectorN< T, 2 >& right ) {
    return VectorN< T, 2 >(
        left + right.x,
        left + right.y
    );
}

template< typename T >
VectorN< T, 2 > operator +( const VectorN< T, 2 >& left, const T right ) {
    return VectorN< T, 2 >(
        left.x + right,
        left.y + right
    );
}

template< typename T >
VectorN< T, 2 > operator -( const VectorN< T, 2 >& left, const T right ) {
    return VectorN< T, 2 >(
        left.x - right,
        left.y - right
    );
}

template< typename T >
VectorN< T, 2 > operator *( const T left, const VectorN< T, 2 >& right ) {
    return VectorN< T, 2 >(
        left * right.x,
        left * right.y
    );
}

template< typename T >
VectorN< T, 2 > operator *( const VectorN< T, 2 >& left, const T right ) {
    return VectorN< T, 2 >(
        left.x * right,
        left.y * right
    );
}

template< typename T >
VectorN< T, 2 > operator /( const VectorN< T, 2 >& left, const T right ) {
    BS_ASSERT_NONZERO_DIVISOR( right );

    return VectorN< T, 2 >(
        left.x / right,
        left.y / right
    );
}

template< typename T >
T dot( const VectorN< T, 2 >& left, const VectorN< T, 2 >& right ) {
    return left.x * right.x +
           left.y * right.y;
}

namespace Private {

template< typename T >
void intNormalize( VectorN< T, 2 >& vecInOut ) {
    BS_ASSERT_CAN_NORMALIZE( vecInOut );

    T absX = abs( vecInOut.x );
    T absY = abs( vecInOut.y );

    if( absX > absY ) {
        vecInOut.x /= absX;
        vecInOut.y  = 0;
    } else {
        vecInOut.x  = 0;
        vecInOut.y /= absY;
    }
}


template< typename T >
void floatNormalize( VectorN< T, 2 >& vecInOut ) {
    BS_ASSERT_CAN_NORMALIZE( vecInOut );

    T invLen = fastInvSqrt( vecInOut.getLengthSq() );
    vecInOut.x *= invLen;
    vecInOut.y *= invLen;
}

}

//Typedefs
template< typename T >
using Vector2 = VectorN< T, 2 >;
typedef Vector2< int32  > Vector2i;
typedef Vector2< float  > Vector2f;
typedef Vector2< double > Vector2d;

}




#endif //BS_VECTOR_VECTOR2_HPP