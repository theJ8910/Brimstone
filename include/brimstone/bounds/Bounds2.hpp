
/*
Bounds2.hpp
-----------------------
Copyright (c) 2014, theJ89

Description:
    
    Adds the following typedefs for convenience:
        * RectI: Rect<int32>
        * RectF: Rect<float>
        * RectD: Rect<double>
*/

#ifndef BS_BOUNDS2_HPP
#define BS_BOUNDS2_HPP




//Includes
#include <brimstone/bounds/BoundsN.hpp>




namespace Brimstone {

template< typename T >
class BoundsN< T, 2 > {
public:
//C4201: nonstandard extension used : nameless struct/union
//It's a non-standard feature, but VC++, G++, and LLVM support it so it shouldn't be too much of an issue
#pragma warning( push )
#pragma warning( disable: 4201 )

    union {
        struct {
            PointN< T, 2 > mins;
            PointN< T, 2 > maxs;
        };
        T data[ 4 ];
    };

#pragma warning( pop )
public:
    BS_BOUNDS_DECLARE_METHODS( 2 )

    //Construtors
    BoundsN();
    BoundsN( const T minX, const T minY, const T maxX, const T maxY );
    BoundsN( const PointN< T, 2 >& mins, T width, T height );

    //Set / get the bounds individually
    void    set( const T minX, const T minY, const T maxX, const T maxY );
    void    get( T& minXOut, T& minYOut, T& maxXOut, T& maxYOut ) const;

    void    set( const T* const values, const uintN count );
    void    get( T* const valuesOut, const uintN count ) const;

    //Set / get the width / height, treating (minX, minY) as an anchor
    void    setDimensions( const T width, const T height );
    void    getDimensions( T& widthOut, T& heightOut );

    void    setWidth( const T width );
    T       getWidth() const;

    void    setHeight( const T height );
    T       getHeight() const;

    //Miscellaneous utility methods
    T       getArea() const;
    void    zero();
    bool    isZero() const;

    //Related free functions
    template< typename T2 >
    friend  bool            operator ==( BoundsN< T2, 2 > left, BoundsN< T2, 2 > right );

    template< typename T2 >
    friend  bool            operator !=( BoundsN< T2, 2 > left, BoundsN< T2, 2 > right );

    template< typename T2 >
    friend  void            clamp( PointN< T2, 2 >& pointInOut, const BoundsN< T2, 2 >& bounds );

    template< typename T2 >
    friend  PointN< T2, 2 > clampedPoint( const PointN< T2, 2 >& point, const BoundsN< T2, 2 >& bounds );
};

BS_BOUNDS_DEFINE_METHODS( 2, BS_BOUNDS_TMPL() )

template< typename T >
BoundsN< T, 2 >::BoundsN()
#ifdef BS_ZERO
    : mins( 0, 0 ), maxs( 0, 0 )
#endif  //BS_ZERO
{
}

template< typename T >
BoundsN< T, 2 >::BoundsN( const T minX, const T minY, const T maxX, const T maxY ) :
    mins( minX, minY ),
    maxs( maxX, maxY ) {
}

template< typename T >
BoundsN< T, 2 >::BoundsN( const PointN< T, 2 >& mins, T width, T height ) :
    mins( mins ),
    maxs(
        mins.x + width,
        mins.y + height
    ) {
}

template< typename T >
void BoundsN< T, 2 >::set( const T minX, const T minY, const T maxX, const T maxY ) {
    mins.x = minX;
    mins.y = minY;
    maxs.x = maxX;
    maxs.y = maxY;
}

template< typename T >
void BoundsN< T, 2 >::get( T& minXOut, T& minYOut, T& maxXOut, T& maxYOut ) const {
    minXOut = mins.x;
    minYOut = mins.y;
    maxXOut = maxs.x;
    maxYOut = maxs.y;
}

template< typename T >
void BoundsN< T, 2 >::set( const T* const values, const uintN count ) {
    BS_ASSERT_NON_NULLPTR( values );
    BS_ASSERT_SIZE( count, 4 );

    mins.x = values[ 0 ];
    mins.y = values[ 1 ];
    maxs.x = values[ 2 ];
    maxs.y = values[ 3 ];
}

template< typename T >
void BoundsN< T, 2 >::get( T* const valuesOut, const uintN count ) const {
    BS_ASSERT_NON_NULLPTR( values );
    BS_ASSERT_SIZE( count, 4 );

    valuesOut[ 0 ] = mins.x;
    valuesOut[ 1 ] = mins.y;
    valuesOut[ 2 ] = maxs.x;
    valuesOut[ 3 ] = maxs.y;
}

template< typename T >
void BoundsN< T, 2 >::setDimensions( const T width, const T height ) {
    maxs.x = mins.x + width;
    maxs.y = mins.y + height;
}

template< typename T >
void BoundsN< T, 2 >::getDimensions( T& widthOut, T& heightOut ) {
    widthOut   = maxs.x - mins.x;
    heightOut  = maxs.y - mins.y;
}

template< typename T >
void BoundsN< T, 2 >::setWidth( const T width ) {
    maxs.x = mins.x + width;
}

template< typename T >
T BoundsN< T, 2 >::getWidth() const {
    return maxs.x - mins.x;
}

template< typename T >
void BoundsN< T, 2 >::setHeight( const T height ) {
    maxs.y = mins.y + height;
}

template< typename T >
T BoundsN< T, 2 >::getHeight() const {
    return maxs.y - mins.y;
}

template< typename T >
T BoundsN< T, 2 >::getArea() const {
    return getWidth() * getHeight();
}

template< typename T >
void BoundsN< T, 2 >::zero() {
    mins.x = 0;
    mins.y = 0;

    maxs.x = 0;
    maxs.y = 0;
}

template< typename T >
bool BoundsN< T, 2 >::isZero() const {
    return mins.x == 0 &&
           mins.y == 0 &&
           maxs.x == 0 &&
           maxs.y == 0;
}

template< typename T >
bool operator ==( BoundsN< T, 2 > left, BoundsN< T, 2 > right ) {
    return left.mins.x == right.mins.x &&
           left.mins.y == right.mins.y &&
           left.maxs.x == right.maxs.x &&
           left.maxs.y == right.maxs.y;
}

template< typename T >
bool operator !=( BoundsN< T, 2 > left, BoundsN< T, 2 > right ) {
    return left.mins.x != right.mins.x ||
           left.mins.y != right.mins.y ||
           left.maxs.x != right.maxs.x ||
           left.maxs.y != right.maxs.y;
}

template< typename T >
void clamp( PointN< T, 2 >& pointInOut, const BoundsN< T, 2 >& bounds ) {
    clamp( pointInOut.x, bounds.mins.x, bounds.maxs.x );
    clamp( pointInOut.y, bounds.mins.y, bounds.maxs.y );
}

template< typename T >
PointN< T, 2 > clampedPoint( const PointN< T, 2 >& point, const BoundsN< T, 2 >& bounds ) {
    return PointN< T, 2 >(
        clampedValue( point.x, bounds.mins.x, bounds.maxs.x ),
        clampedValue( point.y, bounds.mins.y, bounds.maxs.y )
    );
}

//Typedefs
template< typename T >
using Bounds2 = BoundsN< T, 2 >;
typedef Bounds2< int32 >    Bounds2i;
typedef Bounds2< float >    Bounds2f;
typedef Bounds2< double >   Bounds2d;

}




#endif //BS_BOUNDS2_HPP
