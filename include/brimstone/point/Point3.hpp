/*
Point3.hpp
-----------------------
Copyright (c) 2014, theJ89

Description:
    Specialization of PointN for 3D points.
    Adds the following typedefs for convenience:
        Point3< T >: PointN<T,3>
        Point3i:     PointN<int32,3>
        Point3f:     PointN<float,3>
        Point3d:     PointN<double,3>
*/
#ifndef BS_POINT3_HPP
#define BS_POINT3_HPP




//Includes
#include <brimstone/point/PointN.hpp>   //PointN




namespace Brimstone {

template< typename T >
class PointN< T, 3 > {
public:
//C4201: nonstandard extension used : nameless struct/union
//It's a non-standard feature, but VC++, G++, and LLVM support it so it shouldn't be too much of an issue
#pragma warning( push )
#pragma warning( disable: 4201 )

    union {
        T data[3];
        struct { T x, y, z; };
    };

#pragma warning( pop )
public:
    BS_POINT_DECLARE_METHODS()

    //Constructors
    PointN( const T x, const T y, const T z );

    //Set / get the coordinates
    void set( const T x, const T y, const T z );
    void get( T& xOut, T& yOut, T& zOut ) const;
};

BS_POINT_DEFINE_METHODS( 3, BS_POINT_TMPL() )

template< typename T >
PointN< T, 3 >::PointN()
#ifdef BS_ZERO
    : x( 0 ), y( 0 ), z( 0 )
#endif //BS_ZERO
{
}

template< typename T >
PointN< T, 3 >::PointN( const T x, const T y, const T z ) :
    x( x ), y( y ), z( z ) {
}

template< typename T >
void PointN< T, 3 >::set( const T x, const T y, const T z ) {
    PointN::x = x;
    PointN::y = y;
    PointN::z = z;
}

template< typename T >
void PointN< T, 3 >::set( const T* const xyz, const uintN count ) {
    BS_ASSERT_NON_NULLPTR( xyz );
    BS_ASSERT_SIZE( count, 3 );

    x = xyz[0];
    y = xyz[1];
    z = xyz[2];
}

template< typename T >
void PointN< T, 3 >::get( T& xOut, T& yOut, T& zOut ) const {
    xOut = x;
    yOut = y;
    zOut = z;
}

template< typename T >
void PointN< T, 3 >::get( T* const xyzOut, const uintN count ) const {
    BS_ASSERT_NON_NULLPTR( xyzOut );
    BS_ASSERT_SIZE( count, 3 );

    xyzOut[0] = x;
    xyzOut[1] = y;
    xyzOut[2] = z;
}

template< typename T >
void PointN< T, 3 >::zero() {
    x = 0;
    y = 0;
    z = 0;
}

template< typename T >
bool PointN< T, 3 >::isZero() const {
    return x == 0 &&
           y == 0 &&
           z == 0;
}

template< typename T >
std::ostream& operator <<( std::ostream& left, const PointN< T, 3 >& right ) {
    return left << "( " << right[0] << ", " << right[1] << ", " << right[2] << " )";
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
template< typename T >
using Point3 = PointN< T, 3 >;
typedef Point3< int32  > Point3i;
typedef Point3< float  > Point3f;
typedef Point3< double > Point3d;

}




#endif //BS_POINT2_HPP