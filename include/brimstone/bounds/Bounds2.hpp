/*
bounds/Bounds2.hpp
-----------------------
Copyright (c) 2014, theJ89

Description:
    Specialization of Bounds for 2D bounds (rectangles).
    Adds the following typedefs for convenience:
        * Bounds2< T >: Bounds<T,2>
        * Bounds2i:     Bounds<int32,2>
        * Bounds2f:     Bounds<float,2>
        * Bounds2d:     Bounds<double,2>
*/
#ifndef BS_BOUNDS_BOUNDS2_HPP
#define BS_BOUNDS_BOUNDS2_HPP




//Includes
#include <brimstone/bounds/BoundsN.hpp>




namespace Brimstone {

template< typename T >
class Bounds< T, 2 > {
public:
//C4201: nonstandard extension used : nameless struct/union
//It's a non-standard feature, but VC++, G++, and LLVM support it so it shouldn't be too much of an issue
#pragma warning( push )
#pragma warning( disable: 4201 )

    union {
        struct {
            Point< T, 2 > mins;
            Point< T, 2 > maxs;
        };
        struct {
            T minX;
            T minY;
            T maxX;
            T maxY;
        };
        T data[ 4 ];
    };

#pragma warning( pop )
public:
    BS_BOUNDS_DECLARE_METHODS( 2 )

    //Construtors
    Bounds( const T minX, const T minY, const T maxX, const T maxY );
    Bounds( const Point< T, 2 >& mins, const T width, const T height );

    //Set / get the bounds individually
    void    set( const T minX, const T minY, const T maxX, const T maxY );
    void    get( T& minXOut, T& minYOut, T& maxXOut, T& maxYOut ) const;

    //Set / get the width / height, treating (minX, minY) as an anchor
    void    setDimensions( const T width, const T height );
    void    getDimensions( T& widthOut, T& heightOut );

    void    setWidth( const T width );
    T       getWidth() const;

    void    setHeight( const T height );
    T       getHeight() const;

    //Miscellaneous utility methods
    T       getArea() const;
};

BS_BOUNDS_DEFINE_METHODS( 2, BS_TMPL_1( typename T ) )

template< typename T >
inline Bounds< T, 2 >::Bounds( const T& elem ) :
    minX( elem ), minY( elem ),
    maxX( elem ), maxY( elem ) {
}

template< typename T >
template< typename T2 >
inline Bounds< T, 2 >::Bounds( const T2& cppRange ) :
    minX( cppRange[0] ), minY( cppRange[1] ),
    maxX( cppRange[2] ), maxY( cppRange[3] ) {
    BS_ASSERT_SIZE( rangeSize( cppRange ), 4 );
}

template< typename T >
inline Bounds< T, 2 >::Bounds( std::initializer_list< T > il ) :
    minX( *( std::begin( il )     ) ),
    minY( *( std::begin( il ) + 1 ) ),
    maxX( *( std::begin( il ) + 2 ) ),
    maxY( *( std::begin( il ) + 3 ) ) {
    BS_ASSERT_SIZE( rangeSize( il ), 4 );
}

template< typename T >
template< typename T2 >
inline void Bounds< T, 2 >::set( const T2& cppRange ) {
    BS_ASSERT_SIZE( rangeSize( cppRange ), 4 );
    minX = cppRange[0];
    minY = cppRange[1];
    maxX = cppRange[2];
    maxY = cppRange[3];
}

template< typename T >
inline void Bounds< T, 2 >::set( std::initializer_list< T > il ) {
    BS_ASSERT_SIZE( rangeSize( il ), 4 );
    auto it = std::begin( il );
    minX = *( it     );
    minY = *( it + 1 );
    maxX = *( it + 2 );
    maxY = *( it + 3 );
}

template< typename T >
template< typename T2 >
inline void Bounds< T, 2 >::get( T2& cppRangeOut ) const {
    BS_ASSERT_SIZE( rangeSize( cppRangeOut ), 4 );
    cppRangeOut[0] = minX;
    cppRangeOut[1] = minY;
    cppRangeOut[2] = maxX;
    cppRangeOut[3] = maxY;
}

template< typename T >
inline void Bounds< T, 2 >::fill( const T& elem ) {
    minX = elem;
    minY = elem;
    maxX = elem;
    maxY = elem;
}

template< typename T >
Bounds< T, 2 >::Bounds()
#ifdef BS_ZERO
    : minX( 0 ), minY( 0 ), maxX( 0 ), maxY( 0 )
#endif  //BS_ZERO
{
}

template< typename T >
template< typename T2 >
Bounds< T, 2 >::Bounds( const Bounds< T2, 2 >& toCopy ) :
    minX( static_cast< T >( toCopy.minX ) ), minY( static_cast< T >( toCopy.minY ) ),
    maxX( static_cast< T >( toCopy.maxX ) ), maxY( static_cast< T >( toCopy.maxY ) ) {
}

template< typename T >
Bounds< T, 2 >::Bounds( const T minX, const T minY, const T maxX, const T maxY ) :
    minX( minX ), minY( minY ),
    maxX( maxX ), maxY( maxY ) {
}

template< typename T >
Bounds< T, 2 >::Bounds( const Point< T, 2 >& mins, const T width, const T height ) :
    mins( mins ),
    maxX( mins.x + width  ),
    maxY( mins.y + height ) {
}

template< typename T >
void Bounds< T, 2 >::set( const T minX, const T minY, const T maxX, const T maxY ) {
    Bounds::minX = minX;
    Bounds::minY = minY;
    Bounds::maxX = maxX;
    Bounds::maxY = maxY;
}

template< typename T >
void Bounds< T, 2 >::get( T& minXOut, T& minYOut, T& maxXOut, T& maxYOut ) const {
    minXOut = minX;
    minYOut = minY;
    maxXOut = maxX;
    maxYOut = maxY;
}

template< typename T >
void Bounds< T, 2 >::setDimensions( const T width, const T height ) {
    maxX = minX + width;
    maxY = minY + height;
}

template< typename T >
void Bounds< T, 2 >::getDimensions( T& widthOut, T& heightOut ) {
    widthOut   = maxX - minX;
    heightOut  = maxY - minY;
}

template< typename T >
void Bounds< T, 2 >::setWidth( const T width ) {
    maxX = minX + width;
}

template< typename T >
T Bounds< T, 2 >::getWidth() const {
    return maxX - minX;
}

template< typename T >
void Bounds< T, 2 >::setHeight( const T height ) {
    maxY = minY + height;
}

template< typename T >
T Bounds< T, 2 >::getHeight() const {
    return maxY - minY;
}

template< typename T >
T Bounds< T, 2 >::getArea() const {
    return getWidth() * getHeight();
}

template< typename T >
void Bounds< T, 2 >::zero() {
    minX = 0;
    minY = 0;

    maxX = 0;
    maxY = 0;
}

template< typename T >
bool Bounds< T, 2 >::isZero() const {
    return minX == 0 &&
           minY == 0 &&
           maxX == 0 &&
           maxY == 0;
}

template< typename T >
template< typename T2 >
Bounds< T, 2 >& Bounds< T, 2 >::operator =( const Bounds< T2, 2 >& right ) {
    minX = static_cast< T >( right.minX );
    minY = static_cast< T >( right.minY );

    maxX = static_cast< T >( right.maxX );
    maxY = static_cast< T >( right.maxY );

    return (*this);
}

template< typename T >
bool operator ==( const Bounds< T, 2 >& left, const Bounds< T, 2 >& right ) {
    return left.minX == right.minX &&
           left.minY == right.minY &&
           left.maxX == right.maxX &&
           left.maxY == right.maxY;
}

template< typename T >
bool operator !=( const Bounds< T, 2 >& left, const Bounds< T, 2 >& right ) {
    return left.minX != right.minX ||
           left.minY != right.minY ||
           left.maxX != right.maxX ||
           left.maxY != right.maxY;
}

template< typename T >
void clamp( Point< T, 2 >& pointInOut, const Bounds< T, 2 >& bounds ) {
    clamp( pointInOut.x, bounds.minX, bounds.maxX );
    clamp( pointInOut.y, bounds.minY, bounds.maxY );
}

template< typename T >
Point< T, 2 > clampedPoint( const Point< T, 2 >& point, const Bounds< T, 2 >& bounds ) {
    return Point< T, 2 >(
        clampedValue( point.x, bounds.minX, bounds.maxX ),
        clampedValue( point.y, bounds.minY, bounds.maxY )
    );
}

//Typedefs
template< typename T >
using Bounds2 = Bounds< T, 2 >;
typedef Bounds2< int32 >    Bounds2i;
typedef Bounds2< float >    Bounds2f;
typedef Bounds2< double >   Bounds2d;

}




#endif //BS_BOUNDS_BOUNDS2_HPP
