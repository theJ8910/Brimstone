/*
Point2.hpp
-----------------------
Copyright (c) 2014, theJ89

Description:
    Specialization of PointN for 2D points.
    Adds the following typedefs for convenience:
        Point2< T >: PointN<T,2>
        Point2i:     PointN<int32,2>
        Point2f:     PointN<float,2>
        Point2d:     PointN<double,2>
*/
#ifndef BS_POINT2_HPP
#define BS_POINT2_HPP




//Includes
#include <brimstone/point/PointN.hpp>   //PointN




namespace Brimstone {

template< typename T >
class PointN< T, 2 > {
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
    BS_POINT_DECLARE_METHODS()

    //Constructors
    PointN( const T x, const T y );

    //Set / get the coordinates
    void set( const T x, const T y );
    void get( T& xOut, T& yOut ) const;
};

BS_POINT_DEFINE_METHODS( 2, BS_POINT_TMPL() )

template< typename T >
PointN< T, 2 >::PointN()
#ifdef BS_ZERO
    : x( 0 ), y( 0 )
#endif //BS_ZERO
{
}

template< typename T >
PointN< T, 2 >::PointN( const T x, const T y ) :
    x( x ), y( y ) {
}

template< typename T >
void PointN< T, 2 >::set( const T x, const T y ) {
    PointN::x = x;
    PointN::y = y;
}

template< typename T >
void PointN< T, 2 >::set( const T* const xy, const uintN count ) {
    BS_ASSERT_NON_NULLPTR( xy );
    BS_ASSERT_SIZE( count, 2 );

    x = xy[0];
    y = xy[1];
}

template< typename T >
void PointN< T, 2 >::get( T& xOut, T& yOut ) const {
    xOut = x;
    yOut = y;
}

template< typename T >
void PointN< T, 2 >::get( T* const xyOut, const uintN count ) const {
    BS_ASSERT_NON_NULLPTR( xyOut );
    BS_ASSERT_SIZE( count, 2 );

    xyOut[0] = x;
    xyOut[1] = y;
}

template< typename T >
void PointN< T, 2 >::zero() {
    x = 0;
    y = 0;
}

template< typename T >
bool PointN< T, 2 >::isZero() const {
    return x == 0 &&
           y == 0;
}

template< typename T >
std::ostream& operator <<( std::ostream& left, const PointN< T, 2 >& right ) {
    return left << "( " << right.x << ", " << right.y << " )";
}

template< typename T >
bool operator ==( const PointN< T, 2 >& left, const PointN< T, 2 >& right ) {
    return left.x == right.x &&
           left.y == right.y;
}

template< typename T >
bool operator !=( const PointN< T, 2 >& left, const PointN< T, 2 >& right ) {
    return left.x != right.x ||
           left.y != right.y;
}


//Typedefs
template< typename T >
using Point2 = PointN< T, 2 >;
typedef Point2< int32  > Point2i;
typedef Point2< float  > Point2f;
typedef Point2< double > Point2d;

}




#endif //BS_POINT2_HPP