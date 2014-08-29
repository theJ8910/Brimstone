/*
vector/Vector2.hpp
-----------------------
Copyright (c) 2014, theJ89

Description:
    Specialization of Vector for 2D vectors.
    Adds the following typedefs for convenience:
        Vector2< T >: Vector<T,2>
        Vector2i:     Vector<int32,2>
        Vector2f:     Vector<float,2>
        Vector2d:     Vector<double,2>
*/
#ifndef BS_VECTOR_VECTOR2_HPP
#define BS_VECTOR_VECTOR2_HPP




//Includes
#include <brimstone/vector/VectorN.hpp>




namespace Brimstone {

namespace Private {

template< typename T >
void intNormalize( Vector< T, 2 >& vecInOut );

template< typename T >
void floatNormalize( Vector< T, 2 >& vecInOut );

}

template< typename T >
class Vector< T, 2 > : public Private::BasePoint< T, 2 > {
private:
    typedef Private::BasePoint< T, 2 > BaseClass;
public:
    BS_ARRAY_DECLARE_INHERITED_METHODS( Vector, T )
    BS_BASEPOINT_DECLARE_INHERITED_METHODS( Vector, 2, BS_SPEC_2( T2, 2 ) )
    BS_BASEPOINT2_DECLARE_INHERITED_METHODS( Vector, BS_SPEC_2( T2, 2 ) )
    BS_VECTOR_DECLARE_METHODS( 2 )
};
BS_ARRAY_DEFINE_INHERITED_METHODS( Vector, T, BaseClass, BS_TMPL_1( typename T ), BS_SPEC_2( T, 2 ) )
BS_BASEPOINT_DEFINE_INHERITED_METHODS( Vector, 2, BS_TMPL_1( typename T ), BS_SPEC_2( T, 2 ), BS_SPEC_2( T2, 2 ) )
BS_BASEPOINT2_DEFINE_INHERITED_METHODS( Vector, BS_TMPL_1( typename T ), BS_SPEC_2( T, 2 ), BS_SPEC_2( T2, 2 ) )
BS_VECTOR_DEFINE_METHODS( 2, BS_TMPL_1( typename T ) )

template< typename T >
template< typename T2 >
Vector< T, 2 >::Vector( const Point< T2, 2 >& to ) :
    BasePoint(
        static_cast< T >( to.x ),
        static_cast< T >( to.y )
    ) {
}

template< typename T >
Vector< T, 2 >::Vector( const Point< T, 2 >& from, const Point< T, 2 >& to ) :
    BasePoint(
        ( to.x - from.x ),
        ( to.y - from.y )
    ) {
}

template< typename T >
T Vector< T, 2 >::getLengthSq() const {
    return x * x +
           y * y;
}

template< typename T >
Vector< T, 2 >& Vector< T, 2 >::operator +=( const Vector& right ) {
    x += right.x;
    y += right.y;

    return ( *this );
}

template< typename T >
Vector< T, 2 >& Vector< T, 2 >::operator -=( const Vector& right ) {
    x -= right.x;
    y -= right.y;

    return ( *this );
}

template< typename T >
Vector< T, 2 >& Vector< T, 2 >::operator *=( const Vector& right ) {
    x *= right.x;
    y *= right.y;

    return ( *this );
}

template< typename T >
Vector< T, 2 >& Vector< T, 2 >::operator /=( const Vector& right ) {
    BS_ASSERT_NONZERO_DIVISOR( right.x );
    BS_ASSERT_NONZERO_DIVISOR( right.y );

    x /= right.x;
    y /= right.y;

    return ( *this );
}

template< typename T >
Vector< T, 2 >& Vector< T, 2 >::operator +=( const T right ) {
    x += right;
    y += right;

    return ( *this );
}

template< typename T >
Vector< T, 2 >& Vector< T, 2 >::operator -=( const T right ) {
    x -= right;
    y -= right;

    return ( *this );
}

template< typename T >
Vector< T, 2 >& Vector< T, 2 >::operator *=( const T right ) {
    x *= right;
    y *= right;

    return ( *this );
}

template< typename T >
Vector< T, 2 >& Vector< T, 2 >::operator /=( const T right ) {
    BS_ASSERT_NONZERO_DIVISOR( right );

    x /= right;
    y /= right;

    return ( *this );
}

template< typename T >
std::ostream& operator <<( std::ostream& left, const Vector< T, 2 >& right ) {
    return left << "< "
                << ( boost::format( "%|.5f|" ) % right.x ).str() << ", "
                << ( boost::format( "%|.5f|" ) % right.y ).str()
                << " >";
}

template< typename T >
Vector< T, 2 > operator -( const Vector< T, 2 >& right ) {
    return Vector< T, 2 >(
        -right.x,
        -right.y
    );
}

template< typename T >
Vector< T, 2 > operator +( const Vector< T, 2 >& left, const Vector< T, 2 >& right ) {
    return Vector< T, 2 >(
        left.x + right.x,
        left.y + right.y
    );
}

template< typename T >
Vector< T, 2 > operator -( const Vector< T, 2 >& left, const Vector< T, 2 >& right ) {
    return Vector< T, 2 >(
        left.x - right.x,
        left.y - right.y
    );
}

template< typename T >
Vector< T, 2 > operator *( const Vector< T, 2 >& left, const Vector< T, 2 >& right ) {
    return Vector< T, 2 >(
        left.x * right.x,
        left.y * right.y
    );
}

template< typename T >
Vector< T, 2 > operator /( const Vector< T, 2 >& left, const Vector< T, 2 >& right ) {
    BS_ASSERT_NONZERO_DIVISOR( right.x );
    BS_ASSERT_NONZERO_DIVISOR( right.y );

    return Vector< T, 2 >(
        left.x / right.x,
        left.y / right.y
    );
}

template< typename T >
Vector< T, 2 > operator +( const T left, const Vector< T, 2 >& right ) {
    return Vector< T, 2 >(
        left + right.x,
        left + right.y
    );
}

template< typename T >
Vector< T, 2 > operator +( const Vector< T, 2 >& left, const T right ) {
    return Vector< T, 2 >(
        left.x + right,
        left.y + right
    );
}

template< typename T >
Vector< T, 2 > operator -( const Vector< T, 2 >& left, const T right ) {
    return Vector< T, 2 >(
        left.x - right,
        left.y - right
    );
}

template< typename T >
Vector< T, 2 > operator *( const T left, const Vector< T, 2 >& right ) {
    return Vector< T, 2 >(
        left * right.x,
        left * right.y
    );
}

template< typename T >
Vector< T, 2 > operator *( const Vector< T, 2 >& left, const T right ) {
    return Vector< T, 2 >(
        left.x * right,
        left.y * right
    );
}

template< typename T >
Vector< T, 2 > operator /( const Vector< T, 2 >& left, const T right ) {
    BS_ASSERT_NONZERO_DIVISOR( right );

    return Vector< T, 2 >(
        left.x / right,
        left.y / right
    );
}

template< typename T >
T dot( const Vector< T, 2 >& left, const Vector< T, 2 >& right ) {
    return left.x * right.x +
           left.y * right.y;
}

namespace Private {

template< typename T >
void intNormalize( Vector< T, 2 >& vecInOut ) {
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
void floatNormalize( Vector< T, 2 >& vecInOut ) {
    BS_ASSERT_CAN_NORMALIZE( vecInOut );

    T invLen = fastInvSqrt( vecInOut.getLengthSq() );
    vecInOut.x *= invLen;
    vecInOut.y *= invLen;
}

}

//Typedefs
template< typename T >
using Vector2 = Vector< T, 2 >;
typedef Vector2< int32  > Vector2i;
typedef Vector2< float  > Vector2f;
typedef Vector2< double > Vector2d;

}




#endif //BS_VECTOR_VECTOR2_HPP