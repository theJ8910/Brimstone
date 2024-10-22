/*
bounds/Bounds3.hpp
------------------
Copyright (c) 2024, theJ89

Description:
    Specialization of Bounds for 3D bounds (boxes).
    Adds the following aliases for convenience:
        * Bounds3< T >: Bounds<T,3>
        * Bounds3i:     Bounds<int32,3>
        * Bounds3f:     Bounds<float,3>
        * Bounds3d:     Bounds<double,3>
*/
#ifndef BS_BOUNDS_BOUNDS3_HPP
#define BS_BOUNDS_BOUNDS3_HPP




//Includes
#include <brimstone/bounds/BoundsN.hpp>  //Brimstone::Bounds




namespace Brimstone {




template< typename T >
class Bounds< T, 3 > {
public:
//C4201: nonstandard extension used : nameless struct/union
//It's a non-standard feature, but VC++, G++, and LLVM support it so it shouldn't be too much of an issue
#pragma warning( push )
#pragma warning( disable: 4201 )

    union {
        struct {
            Point< T, 3 > mins;
            Point< T, 3 > maxs;
        };
        struct {
            T minX;
            T minY;
            T minZ;
            T maxX;
            T maxY;
            T maxZ;
        };
        T data[ 6 ];
    };

#pragma warning( pop )
public:
    BS_ARRAY_DECLARE_INHERITED_METHODS( Bounds, T )
    BS_ARRAY_DECLARE_METHODS( Bounds, T )
    BS_BOUNDS_DECLARE_METHODS( 3 )

    //Constructors
    Bounds( const T minX, const T minY, const T minZ, const T maxX, const T maxY, const T maxZ );

    //Set / get the bounds individually
    void    set( const T minX, const T minY, const T minZ, const T maxX, const T maxY, const T maxZ );
    void    get( T& minXOut, T& minYOut, T& minZOut, T& maxXOut, T& maxYOut, T& maxZOut ) const;

    //Set / get the position of the bounds, preserving width/length/height.
    void    setPosition( const T minX, const T minY, const T minZ );
    void    getPosition( T& minXOut, T& minYOut, T& minZOut ) const;

    //Set / get the width / length / height, treating (minX, minY, minZ) as an anchor
    void    setSize( const T width, const T length, const T height );
    void    getSize( T& widthOut, T& lengthOut, T& heightOut ) const;

    void    setWidth( const T width );
    T       getWidth() const;

    void    setLength( const T length );
    T       getLength() const;

    void    setHeight( const T height );
    T       getHeight() const;

    //Set / get both the position and the size of the bounds.
    void    setPositionAndSize( const Point< T, 3 >& mins, const T width, const T length, const T height );
    void    getPositionAndSize( Point< T, 3 >& minsOut, T& widthOut, T& lengthOut, T& heightOut ) const;

    void    setPositionAndSize( const T minX, const T minY, const T minZ, const Size< T, 3 >& sizes );
    void    getPositionAndSize( T& minXOut, T& minYOut, T& minZOut, Size< T, 3 >& sizesOut ) const;

    void    setPositionAndSize( const T minX, const T minY, const T minZ, const T width, const T length, const T height );
    void    getPositionAndSize( T& minXOut, T& minYOut, T& minZOut, T& widthOut, T& lengthOut, T& heightOut ) const;
};
BS_ARRAY_DEFINE_METHODS( Bounds, T, data, BS_TMPL_1( typename T ), BS_SPEC_2( T, 3 ) )
BS_BOUNDS_DEFINE_METHODS( 3, BS_TMPL_1( typename T ) )

template< typename T >
inline Bounds< T, 3 >::Bounds( const T& elem ) :
    minX( elem ), minY( elem ), minZ( elem ),
    maxX( elem ), maxY( elem ), maxZ( elem ) {
}

template< typename T >
template< typename T2 >
inline Bounds< T, 3 >::Bounds( const T2& cppRange ) :
    minX( cppRange[0] ), minY( cppRange[1] ), minZ( cppRange[2] ),
    maxX( cppRange[3] ), maxY( cppRange[4] ), maxZ( cppRange[5] ) {
    BS_ASSERT_SIZE( rangeSize( cppRange ), 6 );
}

template< typename T >
inline Bounds< T, 3 >::Bounds( std::initializer_list< T > il ) :
    minX( *( std::begin( il )     ) ),
    minY( *( std::begin( il ) + 1 ) ),
    minZ( *( std::begin( il ) + 2 ) ),
    maxX( *( std::begin( il ) + 3 ) ),
    maxY( *( std::begin( il ) + 4 ) ),
    maxZ( *( std::begin( il ) + 5 ) ) {
    BS_ASSERT_SIZE( rangeSize( il ), 6 );
}

template< typename T >
template< typename T2 >
inline void Bounds< T, 3 >::set( const T2& cppRange ) {
    BS_ASSERT_SIZE( rangeSize( cppRange ), 6 );
    minX = cppRange[0];
    minY = cppRange[1];
    minZ = cppRange[2];
    maxX = cppRange[3];
    maxY = cppRange[4];
    maxZ = cppRange[5];
}

template< typename T >
inline void Bounds< T, 3 >::set( std::initializer_list< T > il ) {
    BS_ASSERT_SIZE( rangeSize( il ), 6 );
    auto it = std::begin( il );
    minX = *( it     );
    minY = *( it + 1 );
    minZ = *( it + 2 );
    maxX = *( it + 3 );
    maxY = *( it + 4 );
    maxZ = *( it + 5 );
}

template< typename T >
template< typename T2 >
inline void Bounds< T, 3 >::get( T2& cppRangeOut ) const {
    BS_ASSERT_SIZE( rangeSize( cppRangeOut ), 6 );
    cppRangeOut[0] = minX;
    cppRangeOut[1] = minY;
    cppRangeOut[2] = minZ;
    cppRangeOut[3] = maxX;
    cppRangeOut[4] = maxY;
    cppRangeOut[5] = maxZ;
}

template< typename T >
inline void Bounds< T, 3 >::fill( const T& elem ) {
    minX = elem;
    minY = elem;
    minZ = elem;
    maxX = elem;
    maxY = elem;
    maxZ = elem;
}

template< typename T >
Bounds< T, 3 >::Bounds()
#ifdef BS_ZERO
    : minX( 0 ), minY( 0 ), minZ( 0 ),
      maxX( 0 ), maxY( 0 ), maxZ( 0 )
#endif  //BS_ZERO
{
}

template< typename T >
Bounds< T, 3 >::Bounds( const Point< T, 3 >& mins, const Size< T, 3 >& sizes ) :
    minX( mins.x ),               minY( mins.y ),                minZ( mins.z ),
    maxX( mins.x + sizes.width ), maxY( mins.y + sizes.length ), maxZ( mins.z + sizes.height ) {
}

template< typename T >
template< typename T2 >
Bounds< T, 3 >::Bounds( const Bounds< T2, 3 >& toCopy ) :
    minX( static_cast< T >( toCopy.minX ) ), minY( static_cast< T >( toCopy.minY ) ), minZ( static_cast< T >( toCopy.minZ ) ),
    maxX( static_cast< T >( toCopy.maxX ) ), maxY( static_cast< T >( toCopy.maxY ) ), maxZ( static_cast< T >( toCopy.maxZ ) ) {
}

template< typename T >
Bounds< T, 3 >::Bounds( const T minX, const T minY, const T minZ, const T maxX, const T maxY, const T maxZ ) :
    minX( minX ), minY( minY ), minZ( minZ ),
    maxX( maxX ), maxY( maxY ), maxZ( maxZ ) {
}

template< typename T >
void Bounds< T, 3 >::set( const T minX, const T minY, const T minZ, const T maxX, const T maxY, const T maxZ ) {
    Bounds::minX = minX;
    Bounds::minY = minY;
    Bounds::minZ = minZ;

    Bounds::maxX = maxX;
    Bounds::maxY = maxY;
    Bounds::maxZ = maxZ;
}

template< typename T >
void Bounds< T, 3 >::get( T& minXOut, T& minYOut, T& minZOut, T& maxXOut, T& maxYOut, T& maxZOut ) const {
    minXOut = minX;
    minYOut = minY;
    minZOut = minZ;

    maxXOut = maxX;
    maxYOut = maxY;
    maxZOut = maxZ;
}

template< typename T >
void Bounds< T, 3 >::setPosition( const Point< T, 3 >& mins ) {
    maxX = mins.x + ( maxX - minX );
    maxY = mins.y + ( maxY - minY );
    maxZ = mins.z + ( maxZ - minZ );

    minX = mins.x;
    minY = mins.y;
    minZ = mins.z;
}

template< typename T >
void Bounds< T, 3 >::setPosition( const T minX, const T minY, const T minZ ) {
    maxX = minX + ( maxX - Bounds::minX );
    maxY = minY + ( maxY - Bounds::minY );
    maxZ = minZ + ( maxZ - Bounds::minZ );

    Bounds::minX = minX;
    Bounds::minY = minY;
    Bounds::minZ = minZ;
}

template< typename T >
void Bounds< T, 3 >::getPosition( T& minXOut, T& minYOut, T& minZOut ) const {
    minXOut = minX;
    minYOut = minY;
    minZOut = minZ;
}

template< typename T >
void Bounds< T, 3 >::setSize( const Size< T, 3 >& sizes ) {
    maxs.x = mins.x + sizes.width;
    maxs.y = mins.y + sizes.length;
    maxs.z = mins.z + sizes.height;
}

template< typename T >
Size< T, 3 > Bounds< T, 3 >::getSize() const {
    return Size< T, 3 >(
        maxs.x - mins.x,
        maxs.y - mins.y,
        maxs.z - mins.z
    );
}

template< typename T >
void Bounds< T, 3 >::setSize( const T width, const T length, const T height ) {
    maxX = minX + width;
    maxY = minY + length;
    maxZ = minZ + height;
}

template< typename T >
void Bounds< T, 3 >::getSize( T& widthOut, T& lengthOut, T& heightOut ) const {
    widthOut  = maxX - minX;
    lengthOut = maxY - minY;
    heightOut = maxZ - minZ;
}

template< typename T >
void Bounds< T, 3 >::setWidth( const T width ) {
    maxX = minX + width;
}

template< typename T >
T Bounds< T, 3 >::getWidth() const {
    return maxX - minX;
}

template< typename T >
void Bounds< T, 3 >::setLength( const T length ) {
    maxY = minY + length;
}

template< typename T >
T Bounds< T, 3 >::getLength() const {
    return maxY - minY;
}

template< typename T >
void Bounds< T, 3 >::setHeight( const T height ) {
    maxZ = minZ + height;
}

template< typename T >
T Bounds< T, 3 >::getHeight() const {
    return maxZ - minZ;
}

template< typename T >
void Bounds< T, 3 >::setPositionAndSize( const Point< T, 3 >& mins, const Size< T, 3 >& sizes ) {
    Bounds::mins = mins;

    maxX = minX + sizes.width;
    maxY = minY + sizes.length;
    maxZ = minZ + sizes.height;
}

template< typename T >
void Bounds< T, 3 >::getPositionAndSize( Point< T, 3 >& minsOut, Size< T, 3 >& sizesOut ) const {
    minsOut = mins;

    sizesOut.width  = maxX - minX;
    sizesOut.length = maxY - minY;
    sizesOut.height = maxZ - minZ;
}

template< typename T >
void Bounds< T, 3 >::setPositionAndSize( const T minX, const T minY, const T minZ, const Size< T, 3 >& sizes ) {
    Bounds::minX = minX;
    Bounds::minY = minY;
    Bounds::minZ = minZ;

    maxX = minX + sizes.width;
    maxY = minY + sizes.length;
    maxZ = minZ + sizes.height;
}

template< typename T >
void Bounds< T, 3 >::getPositionAndSize( T& minXOut, T& minYOut, T& minZOut, Size< T, 3 >& sizesOut ) const {
    minXOut = minX;
    minYOut = minY;
    minZOut = minZ;

    sizesOut.width  = maxX - minX;
    sizesOut.length = maxY - minY;
    sizesOut.height = maxZ - minZ;
}

template< typename T >
void Bounds< T, 3 >::setPositionAndSize( const Point< T, 3 >& mins, const T width, const T length, const T height ) {
    Bounds::mins = mins;

    maxX = minX + width;
    maxY = minY + length;
    maxZ = minZ + height;
}

template< typename T >
void Bounds< T, 3 >::getPositionAndSize( Point< T, 3 >& minsOut, T& widthOut, T& lengthOut, T& heightOut ) const {
    minsOut = mins;

    widthOut   = maxX - minX;
    lengthOut  = maxY - minY;
    heightOut  = maxZ - minZ;
}

template< typename T >
void Bounds< T, 3 >::setPositionAndSize( const T minX, const T minY, const T minZ, const T width, const T length, const T height ) {
    Bounds::minX = minX;
    Bounds::minY = minY;
    Bounds::minZ = minZ;

    maxX = minX + width;
    maxY = minY + length;
    maxZ = minZ + height;
}

template< typename T >
void Bounds< T, 3 >::getPositionAndSize( T& minXOut, T& minYOut, T& minZOut, T& widthOut, T& lengthOut, T& heightOut ) const {
    minXOut = minX;
    minYOut = minY;
    minZOut = minZ;

    widthOut  = maxX - minX;
    lengthOut = maxY - minY;
    heightOut = maxZ - minZ;
}

template< typename T >
Point< T, 3 > Bounds< T, 3 >::getCenter() const {
    return Point< T, 3 >(
        ( maxX + minX ) / static_cast<T>( 2 ),
        ( maxY + minY ) / static_cast<T>( 2 ),
        ( maxZ + minZ ) / static_cast<T>( 2 )
    );
}

template< typename T >
void Bounds< T, 3 >::zero() {
    minX = 0;
    minY = 0;
    minZ = 0;

    maxX = 0;
    maxY = 0;
    maxZ = 0;
}

template< typename T >
bool Bounds< T, 3 >::isZero() const {
    return minX == 0 &&
           minY == 0 &&
           minZ == 0 &&
           maxX == 0 &&
           maxY == 0 &&
           maxZ == 0;
}

template< typename T >
void Bounds< T, 3 >::normalize() {
    minMax( minX, maxX );
    minMax( minY, maxY );
    minMax( minZ, maxZ );
}

template< typename T >
bool Bounds< T, 3 >::isNormal() const {
    return minX <= maxX &&
           minY <= maxY &&
           minZ <= maxZ;
}

template< typename T >
bool Bounds< T, 3 >::contains( const Point< T, 3 >& point ) const {
    return point.x >= minX && point.x <= maxX &&
           point.y >= minY && point.y <= maxY &&
           point.z >= minZ && point.z <= maxZ;
}

template< typename T >
bool Bounds< T, 3 >::contains_mIME( const Point< T, 3 >& point ) const {
    return point.x >= minX && point.x < maxX &&
           point.y >= minY && point.y < maxY &&
           point.z >= minZ && point.z < maxZ;
}

template< typename T >
void Bounds< T, 3 >::include( const Point< T, 3 >& point ) {
    electMinMax( minX, maxX, point.x );
    electMinMax( minY, maxY, point.y );
    electMinMax( minZ, maxZ, point.z );
}

template< typename T >
bool Bounds< T, 3 >::intersects( const Bounds< T, 3 >& bounds ) const {
    return minX <= bounds.maxX &&
           minY <= bounds.maxY &&
           minZ <= bounds.maxZ &&
           maxX >= bounds.minX &&
           maxY >= bounds.minY &&
           maxZ >= bounds.minZ;
}

template< typename T >
bool Bounds< T, 3 >::intersects_EE( const Bounds< T, 3 >& bounds ) const {
    return minX < bounds.maxX &&
           minY < bounds.maxY &&
           minZ < bounds.maxZ &&
           maxX > bounds.minX &&
           maxY > bounds.minY &&
           maxZ > bounds.minZ;
}

template< typename T >
template< typename T2 >
Bounds< T, 3 >& Bounds< T, 3 >::operator =( const Bounds< T2, 3 >& right ) {
    minX = static_cast< T >( right.minX );
    minY = static_cast< T >( right.minY );
    minZ = static_cast< T >( right.minZ );

    maxX = static_cast< T >( right.maxX );
    maxY = static_cast< T >( right.maxY );
    maxZ = static_cast< T >( right.maxZ );

    return (*this);
}

template< typename T >
bool operator ==( const Bounds< T, 3 >& left, const Bounds< T, 3 >& right ) {
    return left.minX == right.minX &&
           left.minY == right.minY &&
           left.minZ == right.minZ &&
           left.maxX == right.maxX &&
           left.maxY == right.maxY &&
           left.maxZ == right.maxZ;
}

template< typename T >
bool operator !=( const Bounds< T, 3 >& left, const Bounds< T, 3 >& right ) {
    return left.minX != right.minX ||
           left.minY != right.minY ||
           left.minZ != right.minZ ||
           left.maxX != right.maxX ||
           left.maxY != right.maxY ||
           left.maxZ != right.maxZ;
}

template< typename T >
void clamp( Point< T, 3 >& pointInOut, const Bounds< T, 3 >& bounds ) {
    clamp( pointInOut.x, bounds.minX, bounds.maxX );
    clamp( pointInOut.y, bounds.minY, bounds.maxY );
    clamp( pointInOut.z, bounds.minZ, bounds.maxZ );
}

template< typename T >
Point< T, 3 > clampedPoint( const Point< T, 3 >& point, const Bounds< T, 3 >& bounds ) {
    return Point< T, 3 >(
        clampedValue( point.x, bounds.minX, bounds.maxX ),
        clampedValue( point.y, bounds.minY, bounds.maxY ),
        clampedValue( point.z, bounds.minZ, bounds.maxZ )
    );
}

template< typename T >
Bounds< T, 3 > intersect( const Bounds< T, 3 >& a, const Bounds< T, 3 >& b ) {
    return Bounds< T, 3 >(
        max( a.minX, b.minX ),
        max( a.minY, b.minY ),
        max( a.minZ, b.minZ ),
        min( a.maxX, b.maxX ),
        min( a.maxY, b.maxY ),
        min( a.maxZ, b.maxZ )
    );
}




//Types
template< typename T >
using Bounds3  = Bounds< T, 3 >;
using Bounds3i = Bounds3< int32 >;
using Bounds3f = Bounds3< float >;
using Bounds3d = Bounds3< double >;




} //namespace Brimstone




#endif //BS_BOUNDS_BOUNDS3_HPP
