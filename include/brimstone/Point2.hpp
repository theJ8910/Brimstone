#ifndef BS_POINT2_HPP
#define BS_POINT2_HPP




//Includes
#include <brimstone/PointN.hpp>




namespace Brimstone {

template< typename T >
class PointN< T, 2 > {
public:
    union {
        T data[2];
        T x, y;
    };
public:
    BS_POINT_DECLARE_METHODS()

    PointN();
    PointN( T x, T y );

    void set( const T x, const T y );
    void set( const T* const xy, const uintN count );
    void get( T& xOut, T& yOut );
    void get( T* const xyOut, const uintN count );
};

BS_POINT_DEFINE_METHODS( 2, BS_POINT_TMPL( 2 ) )

template< typename T >
PointN< T, 2 >::PointN()
#ifdef BS_ZERO
    : x( 0 ), y( 0 )
#endif //BS_ZERO
{
}

template< typename T >
PointN< T, 2 >::PointN( T x, T y ) : x( x ), y( y )
{
}

template< typename T >
void PointN< T, 2 >::set( T x, T y ) {
    this->x = x;
    this->y = y;
}

template< typename T >
void PointN< T, 2 >::set( const T* const xy, const uintN count ) {
    BS_ASSERT_NON_NULLPTR( xy )
    BS_ASSERT_SIZE( count, 2 )

    x = xy[0];
    y = xy[1];
}

template< typename T >
void PointN< T, 2 >::get( T& xOut, T& yOut ) {
    xOut = x;
    yOut = y;
}

template< typename T >
void PointN< T, 2 >::get( T* const xyOut, const uintN count ) {
    BS_ASSERT_NON_NULLPTR( xyOut )
    BS_ASSERT_SIZE( count, 2 )

    xyOut[0] = x;
    xyOut[1] = y;
}


//Typedefs
//2D point using "T".
template< typename T >
using Point2 = PointN< T, 2 >;

//2D point using 32-bit signed integers
typedef Point2< int32  > Point2i;

//2D point using floats
typedef Point2< float  > Point2f;

//2D point using doubles
typedef Point2< double > Point2d;

}

#endif //BS_POINT2_HPP