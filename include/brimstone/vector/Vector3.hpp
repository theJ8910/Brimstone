/*
Vector3.hpp
-----------------------
Copyright (c) 2014, theJ89

Description:
    Specialization of VectorN for 3D vectors.
    Adds the following typedefs for convenience:
        Vector3< T >: VectorN<T,3>
        Vector3i:     VectorN<int32,3>
        Vector3f:     VectorN<float,3>
        Vector3d:     VectorN<double,3>
*/
#ifndef BS_VECTOR_VECTOR3_HPP
#define BS_VECTOR_VECTOR3_HPP




//Includes
#include <brimstone/vector/VectorN.hpp>




namespace Brimstone {

namespace Private {

template< typename T >
void intNormalize( VectorN< T, 3 >& vecInOut );

template< typename T >
void floatNormalize( VectorN< T, 3 >& vecInOut );

}

template< typename T >
class VectorN< T, 3 > : public Private::BasePointN< T, 3 > {
public:
    BS_POINT3_DECLARE_INHERITED_METHODS( VectorN, BS_SPEC_2( T2, 3 ) )
    BS_VECTOR_DECLARE_METHODS( 3 )
};
BS_POINT3_DEFINE_INHERITED_METHODS( VectorN, BS_TMPL_1( typename T ), BS_SPEC_2( T, 3 ), BS_SPEC_2( T2, 3 ) )
BS_VECTOR_DEFINE_METHODS( 3, BS_TMPL_1( typename T ) )

template< typename T >
template< typename T2 >
VectorN< T, 3 >::VectorN( const PointN< T2, 3 >& to ) :
    BasePointN(
        static_cast< T >( to.x ),
        static_cast< T >( to.y ),
        static_cast< T >( to.z )
    ) {
}

template< typename T >
VectorN< T, 3 >::VectorN( const PointN< T, 3 >& from, const PointN< T, 3 >& to ) :
    BasePointN(
        ( to.x - from.x ),
        ( to.y - from.y ),
        ( to.z - from.z )
    ) {
}

template< typename T >
T VectorN< T, 3 >::getLengthSq() const {
    return x * x +
           y * y +
           z * z;
}

template< typename T >
VectorN< T, 3 >& VectorN< T, 3 >::operator +=( const VectorN& right ) {
    x += right.x;
    y += right.y;
    z += right.z;

    return ( *this );
}

template< typename T >
VectorN< T, 3 >& VectorN< T, 3 >::operator -=( const VectorN& right ) {
    x -= right.x;
    y -= right.y;
    z -= right.z;

    return ( *this );
}

template< typename T >
VectorN< T, 3 >& VectorN< T, 3 >::operator *=( const VectorN& right ) {
    x *= right.x;
    y *= right.y;
    z *= right.z;

    return ( *this );
}

template< typename T >
VectorN< T, 3 >& VectorN< T, 3 >::operator /=( const VectorN& right ) {
    BS_ASSERT_NONZERO_DIVISOR( right.x );
    BS_ASSERT_NONZERO_DIVISOR( right.y );
    BS_ASSERT_NONZERO_DIVISOR( right.z );

    x /= right.x;
    y /= right.y;
    z /= right.z;

    return ( *this );
}

template< typename T >
VectorN< T, 3 >& VectorN< T, 3 >::operator +=( const T right ) {
    x += right;
    y += right;
    z += right;

    return ( *this );
}

template< typename T >
VectorN< T, 3 >& VectorN< T, 3 >::operator -=( const T right ) {
    x -= right;
    y -= right;
    z -= right;

    return ( *this );
}

template< typename T >
VectorN< T, 3 >& VectorN< T, 3 >::operator *=( const T right ) {
    x *= right;
    y *= right;
    z *= right;

    return ( *this );
}

template< typename T >
VectorN< T, 3 >& VectorN< T, 3 >::operator /=( const T right ) {
    BS_ASSERT_NONZERO_DIVISOR( right );

    x /= right;
    y /= right;
    z /= right;

    return ( *this );
}

template< typename T >
std::ostream& operator <<( std::ostream& left, const VectorN< T, 3 >& right ) {
    return left << "< " << right.x << ", " << right.y << ", " << right.z << " >";
}

template< typename T >
VectorN< T, 3 > operator -( const VectorN< T, 3 >& right ) {
    return VectorN< T, 3 >(
        -right.x,
        -right.y,
        -right.z
    );
}

template< typename T >
VectorN< T, 3 > operator -( const VectorN< T, 3 >& left, const VectorN< T, 3 >& right ) {
    return VectorN< T, 3 >(
        left.x - right.x,
        left.y - right.y,
        left.z - right.z
    );
}

template< typename T >
VectorN< T, 3 > operator *( const VectorN< T, 3 >& left, const VectorN< T, 3 >& right ) {
    return VectorN< T, 3 >(
        left.x * right.x,
        left.y * right.y,
        left.z * right.z
    );
}

template< typename T >
VectorN< T, 3 > operator /( const VectorN< T, 3 >& left, const VectorN< T, 3 >& right ) {
    BS_ASSERT_NONZERO_DIVISOR( right.x );
    BS_ASSERT_NONZERO_DIVISOR( right.y );
    BS_ASSERT_NONZERO_DIVISOR( right.z );

    return VectorN< T, 3 >(
        left.x / right.x,
        left.y / right.y,
        left.z / right.z
    );
}

template< typename T >
VectorN< T, 3 > operator +( const T left, const VectorN< T, 3 >& right ) {
    return VectorN< T, 3 >(
        left + right.x,
        left + right.y,
        left + right.z
    );
}

template< typename T >
VectorN< T, 3 > operator +( const VectorN< T, 3 >& left, const T right ) {
    return VectorN< T, 3 >(
        left.x + right,
        left.y + right,
        left.z + right
    );
}

template< typename T >
VectorN< T, 3 > operator -( const VectorN< T, 3 >& left, const T right ) {
    return VectorN< T, 3 >(
        left.x - right,
        left.y - right,
        left.z - right
    );
}

template< typename T >
VectorN< T, 3 > operator *( const T left, const VectorN< T, 3 >& right ) {
    return VectorN< T, 3 >(
        left * right.x,
        left * right.y,
        left * right.z
    );
}

template< typename T >
VectorN< T, 3 > operator *( const VectorN< T, 3 >& left, const T right ) {
    return VectorN< T, 3 >(
        left.x * right,
        left.y * right,
        left.z * right
    );
}

template< typename T >
VectorN< T, 3 > operator /( const VectorN< T, 3 >& left, const T right ) {
    BS_ASSERT_NONZERO_DIVISOR( right );

    return VectorN< T, 3 >(
        left.x / right,
        left.y / right,
        left.z / right
    );
}

template< typename T >
T dot( const VectorN< T, 3 >& left, const VectorN< T, 3 >& right ) {
    return left.x * right.x +
           left.y * right.y +
           left.z * right.z;
}

template< typename T >
VectorN< T, 3 > cross( const VectorN< T, 3 >& left, const VectorN< T, 3 >& right ) {
    return VectorN< T, 3 >(
        left.y * right.z - left.z * right.y,
        left.z * right.x - left.x * right.z,
        left.x * right.y - left.y * right.x
    );
}

namespace Private {

template< typename T >
void intNormalize( VectorN< T, 3 >& vecInOut ) {
    BS_ASSERT_CAN_NORMALIZE( vecInOut );

    T absX = abs( vecInOut.x );
    T absY = abs( vecInOut.y );
    T absZ = abs( vecInOut.z );

    //X >= Y
    if( absX >= absY ) {

        // X >= Y && X >= Z
        if( absX >= absZ ) {
            vecInOut.x /= absX;
            vecInOut.y  = 0;
            vecInOut.z  = 0;
            return;
        }

    //Y > X && Y >= Z
    } else if( absY >= absZ ) {
        vecInOut.x  = 0;
        vecInOut.y /= absY;
        vecInOut.z  = 0;
        return;
    }

    //Z > X && Z > X
    vecInOut.x  = 0;
    vecInOut.y  = 0;
    vecInOut.z /= absZ;
}


template< typename T >
void floatNormalize( VectorN< T, 3 >& vecInOut ) {
    BS_ASSERT_CAN_NORMALIZE( vecInOut );

    T invLen = fastInvSqrt( vecInOut.getLengthSq() );
    vecInOut.x *= invLen;
    vecInOut.y *= invLen;
    vecInOut.z *= invLen;
}

}

//Typedefs
template< typename T >
using Vector3 = VectorN< T, 3 >;
typedef Vector3< int32  > Vector3i;
typedef Vector3< float  > Vector3f;
typedef Vector3< double > Vector3d;

}




#endif //BS_VECTOR_VECTOR3_HPP