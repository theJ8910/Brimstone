/*
vector/Vector2.hpp
------------------
Copyright (c) 2024, theJ89

Description:
    Specialization of Vector for 2D vectors.
    Adds the following aliases for convenience:
        Vector2< T >: Vector<T,2>
        Vector2i:     Vector<int32,2>
        Vector2f:     Vector<float,2>
        Vector2d:     Vector<double,2>
*/
#ifndef BS_VECTOR_VECTOR2_HPP
#define BS_VECTOR_VECTOR2_HPP




//Includes
#include <brimstone/vector/VectorN.hpp>  //Brimstone::Vector




namespace Brimstone::Private {




template< typename T >
void intNormalize( Vector< T, 2 >& vecInOut );

template< typename T >
void floatNormalize( Vector< T, 2 >& vecInOut );




} //namespace Brimstone::Private




namespace Brimstone {




template< typename T >
class Vector< T, 2 > {
public:
//C4201: nonstandard extension used : nameless struct/union
//It's a non-standard feature, but VC++, G++, and LLVM support it so it shouldn't be too much of an issue
#pragma warning( push )
#pragma warning( disable: 4201 )

    union {
        T data[2];
        struct { T x, y; };
    };

#pragma warning( pop )
public:
    BS_ARRAY_DECLARE_INHERITED_METHODS( Vector, T    )
    BS_ARRAY_DECLARE_METHODS(           Vector, T    )
    BS_BASEPOINT_DECLARE_METHODS(       Vector,    2 )
    BS_BASEPOINT2_DECLARE_METHODS(      Vector       )
    BS_VECTOR_DECLARE_METHODS(                     2 )

    void left();
    void right();
};
BS_ARRAY_DEFINE_METHODS(         Vector, T,    data, BS_TMPL_1( typename T ), BS_SPEC_2( T, 2 ) )
BS_BASEPOINT2_DEFINE_METHODS(    Vector                                                         )
BS_VECTOR_DEFINE_METHODS(                   2,       BS_TMPL_1( typename T )                    )




//Forward declarations
template< typename T >
T dot( const Vector< T, 2 >& left, const Vector< T, 2 >& right );




template< typename T >
Vector< T, 2 >::Vector()
#ifdef BS_ZERO
    : x( 0 ), y( 0 )
#endif //BS_ZERO
{
}

template< typename T >
template< typename T2 >
Vector< T, 2 >::Vector( const Point< T2, 2 >& to ) :
    Vector(
        static_cast< T >( to.x ),
        static_cast< T >( to.y )
    ) {
}

template< typename T >
Vector< T, 2 >::Vector( const Point< T, 2 >& from, const Point< T, 2 >& to ) :
    Vector(
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
void Vector< T, 2 >::invert() {
    BS_ASSERT_NONZERO_DIVISOR( x );
    BS_ASSERT_NONZERO_DIVISOR( y );

    const T one = static_cast< T >( 1 );
    x = one / x;
    y = one / y;
}

template< typename T >
void Vector< T, 2 >::negate() {
    x = -x;
    y = -y;
}

template< typename T >
void Vector< T, 2 >::left() {
    const T oldX = x;
    x = -y;
    y =  oldX;
}

template< typename T >
void Vector< T, 2 >::right() {
    const T oldX = x;
    x =  y;
    y = -oldX;
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
Vector< T, 2 > operator -( const Point< T, 2 >& left, const Point< T, 2 >& right ) {
    return Vector< T, 2 >(
        left.x - right.x,
        left.y - right.y
    );
}

template< typename T >
Point< T, 2 > operator +( const Point< T, 2 >& left, const Vector< T, 2 >& right ) {
    return Point< T, 2 >(
        left.x + right.x,
        left.y + right.y
    );
}

template< typename T >
Point< T, 2 > operator +( const Vector< T, 2 >& left, const Point< T, 2 >& right ) {
    return Point< T, 2 >(
        left.x + right.x,
        left.y + right.y
    );
}

template< typename T >
Point< T, 2 > operator -( const Point< T, 2 >& left, const Vector< T, 2 >& right ) {
    return Point< T, 2 >(
        left.x - right.x,
        left.y - right.y
    );
}

template< typename T >
Vector< T, 2 > invert( const Vector< T, 2 >& vector ) {
    BS_ASSERT_NONZERO_DIVISOR( vector.x );
    BS_ASSERT_NONZERO_DIVISOR( vector.y );

    const T one = static_cast< T >( 1 );
    return Vector< T, 2 >(
        one / vector.x,
        one / vector.y
    );
}

template< typename T >
Vector< T, 2 > left( const Vector< T, 2 >& vector ) {
    return Vector< T, 2 >( -vector.y, vector.x );
}

template< typename T >
Vector< T, 2 > right( const Vector< T, 2 >& vector ) {
    return Vector< T, 2 >( vector.y, -vector.x );
}

template< typename T >
T dot( const Vector< T, 2 >& left, const Vector< T, 2 >& right ) {
    return left.x * right.x +
           left.y * right.y;
}




} //namespace Brimstone




namespace Brimstone::Private {




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




} //namespace Brimstone::Private




namespace Brimstone {




//Types
template< typename T >
using Vector2  = Vector< T, 2 >;
using Vector2i = Vector2< int32  >;
using Vector2f = Vector2< float  >;
using Vector2d = Vector2< double >;




} //namespace Brimstone




#endif //BS_VECTOR_VECTOR2_HPP
