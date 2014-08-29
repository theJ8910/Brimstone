/*
vector/Vector4.hpp
-----------------------
Copyright (c) 2014, theJ89

Description:
    Specialization of Vector for 4D vectors.
    Adds the following typedefs for convenience:
        Vector4< T >: Vector<T,4>
        Vector4i:     Vector<int32,4>
        Vector4f:     Vector<float,4>
        Vector4d:     Vector<double,4>
*/
#ifndef BS_VECTOR_VECTOR4_HPP
#define BS_VECTOR_VECTOR4_HPP




//Includes
#include <brimstone/vector/VectorN.hpp>




namespace Brimstone {

namespace Private {

template< typename T >
void intNormalize( Vector< T, 4 >& vecInOut );

template< typename T >
void floatNormalize( Vector< T, 4 >& vecInOut );

}

template< typename T >
class Vector< T, 4 > : public Private::BasePoint< T, 4 > {
private:
    typedef Private::BasePoint< T, 4 > BaseClass;
public:
    BS_ARRAY_DECLARE_INHERITED_METHODS( Vector, T )
    BS_BASEPOINT_DECLARE_INHERITED_METHODS( Vector, 4, BS_SPEC_2( T2, 4 ) )
    BS_BASEPOINT4_DECLARE_INHERITED_METHODS( Vector, BS_SPEC_2( T2, 4 ) )
    BS_VECTOR_DECLARE_METHODS( 4 )
};
BS_ARRAY_DEFINE_INHERITED_METHODS( Vector, T, BaseClass, BS_TMPL_1( typename T ), BS_SPEC_2( T, 4 ) )
BS_BASEPOINT_DEFINE_INHERITED_METHODS( Vector, 4, BS_TMPL_1( typename T ), BS_SPEC_2( T, 4 ), BS_SPEC_2( T2, 4 ) )
BS_BASEPOINT4_DEFINE_INHERITED_METHODS( Vector, BS_TMPL_1( typename T ), BS_SPEC_2( T, 4 ), BS_SPEC_2( T2, 4 ) )
BS_VECTOR_DEFINE_METHODS( 4, BS_TMPL_1( typename T ) )

template< typename T >
template< typename T2 >
Vector< T, 4 >::Vector( const Point< T2, 4 >& to ) :
    BasePoint(
        static_cast< T >( to.x ),
        static_cast< T >( to.y ),
        static_cast< T >( to.z ),
        static_cast< T >( to.w )
    ) {
}

template< typename T >
Vector< T, 4 >::Vector( const Point< T, 4 >& from, const Point< T, 4 >& to ) :
    BasePoint(
        ( to.x - from.x ),
        ( to.y - from.y ),
        ( to.z - from.z ),
        ( to.w - from.w )
    ) {
}

template< typename T >
T Vector< T, 4 >::getLengthSq() const {
    return x * x +
           y * y +
           z * z +
           w * w;
}

template< typename T >
Vector< T, 4 >& Vector< T, 4 >::operator +=( const Vector& right ) {
    x += right.x;
    y += right.y;
    z += right.z;
    w += right.w;

    return ( *this );
}

template< typename T >
Vector< T, 4 >& Vector< T, 4 >::operator -=( const Vector& right ) {
    x -= right.x;
    y -= right.y;
    z -= right.z;
    w -= right.w;

    return ( *this );
}

template< typename T >
Vector< T, 4 >& Vector< T, 4 >::operator *=( const Vector& right ) {
    x *= right.x;
    y *= right.y;
    z *= right.z;
    w *= right.w;

    return ( *this );
}

template< typename T >
Vector< T, 4 >& Vector< T, 4 >::operator /=( const Vector& right ) {
    BS_ASSERT_NONZERO_DIVISOR( right.x );
    BS_ASSERT_NONZERO_DIVISOR( right.y );
    BS_ASSERT_NONZERO_DIVISOR( right.z );
    BS_ASSERT_NONZERO_DIVISOR( right.w );

    x /= right.x;
    y /= right.y;
    z /= right.z;
    w /= right.w;

    return ( *this );
}

template< typename T >
Vector< T, 4 >& Vector< T, 4 >::operator +=( const T right ) {
    x += right;
    y += right;
    z += right;
    w += right;

    return ( *this );
}

template< typename T >
Vector< T, 4 >& Vector< T, 4 >::operator -=( const T right ) {
    x -= right;
    y -= right;
    z -= right;
    w -= right;

    return ( *this );
}

template< typename T >
Vector< T, 4 >& Vector< T, 4 >::operator *=( const T right ) {
    x *= right;
    y *= right;
    z *= right;
    w *= right;

    return ( *this );
}

template< typename T >
Vector< T, 4 >& Vector< T, 4 >::operator /=( const T right ) {
    BS_ASSERT_NONZERO_DIVISOR( right );

    x /= right;
    y /= right;
    z /= right;
    w /= right;

    return ( *this );
}

template< typename T >
std::ostream& operator <<( std::ostream& left, const Vector< T, 4 >& right ) {
    return left << "< "
                << ( boost::format( "%|.5f|" ) % right.x ).str() << ", "
                << ( boost::format( "%|.5f|" ) % right.y ).str() << ", "
                << ( boost::format( "%|.5f|" ) % right.z ).str() << ", "
                << ( boost::format( "%|.5f|" ) % right.w ).str()
                << " >";
}

template< typename T >
Vector< T, 4 > operator -( const Vector< T, 4 >& right ) {
    return Vector< T, 4 >(
        -right.x,
        -right.y,
        -right.z,
        -right.w
    );
}

template< typename T >
Vector< T, 4 > operator -( const Vector< T, 4 >& left, const Vector< T, 4 >& right ) {
    return Vector< T, 4 >(
        left.x - right.x,
        left.y - right.y,
        left.z - right.z,
        left.w - right.w
    );
}

template< typename T >
Vector< T, 4 > operator *( const Vector< T, 4 >& left, const Vector< T, 4 >& right ) {
    return Vector< T, 4 >(
        left.x * right.x,
        left.y * right.y,
        left.z * right.z,
        left.w * right.w
    );
}

template< typename T >
Vector< T, 4 > operator /( const Vector< T, 4 >& left, const Vector< T, 4 >& right ) {
    BS_ASSERT_NONZERO_DIVISOR( right.x );
    BS_ASSERT_NONZERO_DIVISOR( right.y );
    BS_ASSERT_NONZERO_DIVISOR( right.z );
    BS_ASSERT_NONZERO_DIVISOR( right.w );

    return Vector< T, 4 >(
        left.x / right.x,
        left.y / right.y,
        left.z / right.z,
        left.w / right.w
    );
}

template< typename T >
Vector< T, 4 > operator +( const T left, const Vector< T, 4 >& right ) {
    return Vector< T, 4 >(
        left + right.x,
        left + right.y,
        left + right.z,
        left + right.w
    );
}

template< typename T >
Vector< T, 4 > operator +( const Vector< T, 4 >& left, const T right ) {
    return Vector< T, 4 >(
        left.x + right,
        left.y + right,
        left.z + right,
        left.w + right
    );
}

template< typename T >
Vector< T, 4 > operator -( const Vector< T, 4 >& left, const T right ) {
    return Vector< T, 4 >(
        left.x - right,
        left.y - right,
        left.z - right,
        left.w - right
    );
}

template< typename T >
Vector< T, 4 > operator *( const T left, const Vector< T, 4 >& right ) {
    return Vector< T, 4 >(
        left * right.x,
        left * right.y,
        left * right.z,
        left * right.w
    );
}

template< typename T >
Vector< T, 4 > operator *( const Vector< T, 4 >& left, const T right ) {
    return Vector< T, 4 >(
        left.x * right,
        left.y * right,
        left.z * right,
        left.w * right
    );
}

template< typename T >
Vector< T, 4 > operator /( const Vector< T, 4 >& left, const T right ) {
    BS_ASSERT_NONZERO_DIVISOR( right );

    return Vector< T, 4 >(
        left.x / right,
        left.y / right,
        left.z / right,
        left.w / right
    );
}

template< typename T >
T dot( const Vector< T, 4 >& left, const Vector< T, 4 >& right ) {
    return left.x * right.x +
           left.y * right.y +
           left.z * right.z +
           left.w * right.w;
}

namespace Private {

template< typename T >
void intNormalize( Vector< T, 4 >& vecInOut ) {
    BS_ASSERT_CAN_NORMALIZE( vecInOut );

    T absX = abs( vecInOut.x );
    T absY = abs( vecInOut.y );
    T absZ = abs( vecInOut.z );
    T absW = abs( vecInOut.w );

    //TODO fixme

    //X >= Y
    if( absX >= absY ) {

        //X >= Y &&
        //X >= Z
        if( absX >= absZ ) {

            //X >= Y &&
            //X >= Z &&
            //X >= W    => max( X, Y, Z, W ) == X
            if( absX >= absW ) {
                vecInOut.x /= absX;
                vecInOut.y  = 0;
                vecInOut.z  = 0;
                vecInOut.w  = 0;
                return;
            //X >= Y &&
            //X >= Z &&
            //W > X    => max( X, Y, Z, W ) == W
            }

        //Z > X >= Y &&
        //Z >= W       => max( X, Y, Z, W ) == Z
        } else if( absZ >= absW ) {
            vecInOut.x  = 0;
            vecInOut.y  = 0;
            vecInOut.z /= absZ;
            vecInOut.w  = 0;
            return;
        //W > Z > X >= Y => max( X, Y, Z, W ) == W
        }

    //Y > X &&
    //Y >= Z
    } else if( absY >= absZ ) {
        //Y > X  &&
        //Y >= Z &&
        //Y >= W    => max( X, Y, Z, W ) == Y
        if( absY >= absW ) {
            vecInOut.x  = 0;
            vecInOut.y /= absY;
            vecInOut.z  = 0;
            vecInOut.w  = 0;
            return;
        //Y > X &&
        //W > Y >= Z => max( X, Y, Z, W ) == W
        }
    //Z > Y > X &&
    //Z >= W       => max( X, Y, Z, W ) == Z
    } else if( absZ >= absW ) {
        vecInOut.x  = 0;
        vecInOut.y  = 0;
        vecInOut.z /= absZ;
        vecInOut.w  = 0;
        return;

    //W > Z > Y > X => max( X, Y, Z, W ) == W
    }

    //W > Z > Y > X
    vecInOut.x  = 0;
    vecInOut.y  = 0;
    vecInOut.z  = 0;
    vecInOut.w /= absW;
}


template< typename T >
void floatNormalize( Vector< T, 4 >& vecInOut ) {
    BS_ASSERT_CAN_NORMALIZE( vecInOut );

    T invLen = fastInvSqrt( vecInOut.getLengthSq() );
    vecInOut.x *= invLen;
    vecInOut.y *= invLen;
    vecInOut.z *= invLen;
    vecInOut.w *= invLen;
}

}

//Typedefs
template< typename T >
using Vector4 = Vector< T, 4 >;
typedef Vector4< int32  > Vector4i;
typedef Vector4< float  > Vector4f;
typedef Vector4< double > Vector4d;

}




#endif //BS_VECTOR_VECTOR4_HPP