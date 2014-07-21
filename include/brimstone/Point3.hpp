#ifndef BS_POINT3_HPP
#define BS_POINT3_HPP

#pragma warning( push )

//C4201: nonstandard extension used : nameless struct/union
//It's a non-standard feature, but VC++, G++, and LLVM support it so it shouldn't be too much of an issue
#pragma warning( disable: 4201 )




//Includes
#include <brimstone/PointN.hpp>




namespace Brimstone {

template< typename T >
class PointN< T, 3 > {
public:
    union {
        T data[3];
        struct { T x, y, z; };
    };
public:
    BS_POINT_DECLARE_METHODS()

    PointN();
    PointN( T x, T y, T z );

    void set( const T x, const T y, const T z );
    void set( const T* const xyz, const uintN count );
    void get( T& xOut, T& yOut, T& zOut );
    void get( T* const xyzOut, const uintN count );

    template< typename T2 >
    friend bool     operator ==( const PointN< T2, 3 >& left, const PointN< T2, 3 >& right );
    template< typename T2 >
    friend bool		operator !=( const PointN< T2, 3 >& left, const PointN< T2, 3 >& right );
};

BS_POINT_DEFINE_METHODS( 3, BS_POINT_TMPL( 3 ) )

template< typename T >
PointN< T, 3 >::PointN()
#ifdef BS_ZERO
    : x( 0 ), y( 0 ), z( 0 )
#endif //BS_ZERO
{
}

template< typename T >
PointN< T, 3 >::PointN( T x, T y, T z ) : x( x ), y( y ), z( z )
{
}

template< typename T >
void PointN< T, 3 >::set( T x, T y, T z ) {
    this->x = x;
    this->y = y;
}

template< typename T >
void PointN< T, 3 >::set( const T* const xyz, const uintN count ) {
    BS_ASSERT_NON_NULLPTR( xyz )
    BS_ASSERT_SIZE( count, 3 )

    x = xyz[0];
    y = xyz[1];
    z = xyz[2];
}

template< typename T >
void PointN< T, 3 >::get( T& xOut, T& yOut, T& zOut ) {
    xOut = x;
    yOut = y;
    zOut = z;
}

template< typename T >
void PointN< T, 3 >::get( T* const xyzOut, const uintN count ) {
    BS_ASSERT_NON_NULLPTR( xyzOut )
    BS_ASSERT_SIZE( count, 3 )

    xyzOut[0] = x;
    xyzOut[1] = y;
    xyzOut[2] = z;
}

template< typename T >
bool operator ==( const PointN< T, 3 >& left, PointN< T, 3 >& right ) {
    return left.x == right.x &&
           left.y == right.y &&
           left.z == right.z;
}

template< typename T >
bool operator !=( const PointN< T, 3 >& left, const PointN< T, 3 >& right ) {
    return left.x != right.x ||
           left.y != right.y ||
           left.z != right.z;
}

//Typedefs
//3D point using "T".
template< typename T >
using Point3 = PointN< T, 3 >;

//3D point using 32-bit signed integers
typedef Point3< int32  > Point3i;

//3D point using floats
typedef Point3< float  > Point3f;

//3D point using doubles
typedef Point3< double > Point3d;

}




#pragma warning( pop )

#endif //BS_POINT2_HPP