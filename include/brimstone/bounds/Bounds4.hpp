/*
bounds/Bounds4.hpp
------------------
Copyright (c) 2024, theJ89

Description:
    Specialization of Bounds for 4D bounds (tesseracts).
    Adds the following aliases for convenience:
        * Bounds4< T >: Bounds<T,4>
        * Bounds4i:     Bounds<int32,4>
        * Bounds4f:     Bounds<float,4>
        * Bounds4d:     Bounds<double,4>
*/
#ifndef BS_BOUNDS_BOUNDS4_HPP
#define BS_BOUNDS_BOUNDS4_HPP




//Includes
#include <brimstone/bounds/BoundsN.hpp>  //Brimstone::Bounds




namespace Brimstone {




template< typename T >
class Bounds< T, 4 > {
public:
//C4201: nonstandard extension used : nameless struct/union
//It's a non-standard feature, but VC++, G++, and LLVM support it so it shouldn't be too much of an issue
#pragma warning( push )
#pragma warning( disable: 4201 )

    union {
        struct {
            Point< T, 4 > mins;
            Point< T, 4 > maxs;
        };
        struct {
            T minX;
            T minY;
            T minZ;
            T minW;
            T maxX;
            T maxY;
            T maxZ;
            T maxW;
        };
        T data[ 8 ];
    };

#pragma warning( pop )
public:
    BS_ARRAY_DECLARE_INHERITED_METHODS( Bounds, T )
    BS_ARRAY_DECLARE_METHODS( Bounds, T )
    BS_BOUNDS_DECLARE_METHODS( 4 )

    //Constructors
    Bounds( const T minX, const T minY, const T minZ, const T minW, const T maxX, const T maxY, const T maxZ, const T maxW );

    //Set / get the bounds individually
    void set( const T minX, const T minY, const T minZ, const T minW, const T maxX, const T maxY, const T maxZ, const T maxW );
    void get( T& minXOut, T& minYOut, T& minZOut, T& minWOut, T& maxXOut, T& maxYOut, T& maxZOut, T& maxWOut ) const;

    //Set / get the position of the bounds, preserving width/length/height/depth.
    void    setPosition( const T minX, const T minY, const T minZ, const T minW );
    void    getPosition( T& minXOut, T& minYOut, T& minZOut, T& minWOut ) const;

    //Set / get the width / length / height / depth, treating (minX, minY, minZ, minW) as an anchor
    void    setSize( const T width, const T length, const T height, const T depth );
    void    getSize( T& widthOut, T& lengthOut, T& heightOut, T& depthOut ) const;

    void    setWidth( const T width );
    T       getWidth() const;

    void    setLength( const T length );
    T       getLength() const;

    void    setHeight( const T height );
    T       getHeight() const;

    void    setDepth( const T depth );
    T       getDepth() const;

    //Set / get both the position and the size of the bounds.
    void    setPositionAndSize( const Point< T, 4 >& mins, const T width, const T length, const T height, const T depth );
    void    getPositionAndSize( Point< T, 4 >& minsOut, T& widthOut, T& lengthOut, T& heightOut, T& depthOut ) const;

    void    setPositionAndSize( const T minX, const T minY, const T minZ, const T minW, const Size< T, 4 >& sizes );
    void    getPositionAndSize( T& minXOut, T& minYOut, T& minZOut, T& minWOut, Size< T, 4 >& sizesOut ) const;

    void    setPositionAndSize( const T minX, const T minY, const T minZ, const T minW, const T width, const T length, const T height, const T depth );
    void    getPositionAndSize( T& minXOut, T& minYOut, T& minZOut, T& minWOut, T& widthOut, T& lengthOut, T& heightOut, T& depthOut ) const;
};
BS_ARRAY_DEFINE_METHODS( Bounds, T, data, BS_TMPL_1( typename T ), BS_SPEC_2( T, 4 ) )
BS_BOUNDS_DEFINE_METHODS( 4, BS_TMPL_1( typename T ) )

template< typename T >
inline Bounds< T, 4 >::Bounds( const T& elem ) :
    minX( elem ), minY( elem ), minZ( elem ), minW( elem ),
    maxX( elem ), maxY( elem ), maxZ( elem ), maxW( elem ) {
}

template< typename T >
template< typename T2 >
inline Bounds< T, 4 >::Bounds( const T2& cppRange ) :
    minX( cppRange[0] ), minY( cppRange[1] ), minZ( cppRange[2] ), minW( cppRange[3] ),
    maxX( cppRange[4] ), maxY( cppRange[5] ), maxZ( cppRange[6] ), maxW( cppRange[7] ) {
    BS_ASSERT_SIZE( rangeSize( cppRange ), 8 );
}

template< typename T >
inline Bounds< T, 4 >::Bounds( std::initializer_list< T > il ) :
    minX( *( std::begin( il )     ) ),
    minY( *( std::begin( il ) + 1 ) ),
    minZ( *( std::begin( il ) + 2 ) ),
    minW( *( std::begin( il ) + 3 ) ),
    maxX( *( std::begin( il ) + 4 ) ),
    maxY( *( std::begin( il ) + 5 ) ),
    maxZ( *( std::begin( il ) + 6 ) ),
    maxW( *( std::begin( il ) + 7 ) ){
    BS_ASSERT_SIZE( rangeSize( il ), 8 );
}

template< typename T >
template< typename T2 >
inline void Bounds< T, 4 >::set( const T2& cppRange ) {
    BS_ASSERT_SIZE( rangeSize( cppRange ), 8 );
    minX = cppRange[0];
    minY = cppRange[1];
    minZ = cppRange[2];
    minW = cppRange[3];
    maxX = cppRange[4];
    maxY = cppRange[5];
    maxZ = cppRange[6];
    maxW = cppRange[7];
}

template< typename T >
inline void Bounds< T, 4 >::set( std::initializer_list< T > il ) {
    BS_ASSERT_SIZE( rangeSize( il ), 8 );
    auto it = std::begin( il );
    minX = *( it     );
    minY = *( it + 1 );
    minZ = *( it + 2 );
    minW = *( it + 3 );
    maxX = *( it + 4 );
    maxY = *( it + 5 );
    maxZ = *( it + 6 );
    maxW = *( it + 7 );
}

template< typename T >
template< typename T2 >
inline void Bounds< T, 4 >::get( T2& cppRangeOut ) const {
    BS_ASSERT_SIZE( rangeSize( cppRangeOut ), 8 );
    cppRangeOut[0] = minX;
    cppRangeOut[1] = minY;
    cppRangeOut[2] = minZ;
    cppRangeOut[3] = minW;
    cppRangeOut[4] = maxX;
    cppRangeOut[5] = maxY;
    cppRangeOut[6] = maxZ;
    cppRangeOut[7] = maxW;
}

template< typename T >
inline void Bounds< T, 4 >::fill( const T& elem ) {
    minX = elem;
    minY = elem;
    minZ = elem;
    minW = elem;
    maxX = elem;
    maxY = elem;
    maxZ = elem;
    maxW = elem;
}

template< typename T >
Bounds< T, 4 >::Bounds()
#ifdef BS_ZERO
    : minX( 0 ), minY( 0 ), minZ( 0 ), minW( 0 ),
      maxX( 0 ), maxY( 0 ), maxZ( 0 ), maxW( 0 )
#endif  //BS_ZERO
{
}

template< typename T >
Bounds< T, 4 >::Bounds( const Point< T, 4 >& mins, const Size< T, 4 >& sizes ) :
    minX( mins.x ),               minY( mins.y ),                minZ( mins.z ),                minW( mins.w ),
    maxX( mins.x + sizes.width ), maxY( mins.y + sizes.length ), maxZ( mins.z + sizes.height ), maxW( mins.w + sizes.depth ) {
}

template< typename T >
template< typename T2 >
Bounds< T, 4 >::Bounds( const Bounds< T2, 4 >& toCopy ) :
    minX( static_cast< T >( toCopy.minX ) ), minY( static_cast< T >( toCopy.minY ) ), minZ( static_cast< T >( toCopy.minZ ) ), minW( static_cast< T >( toCopy.minW ) ),
    maxX( static_cast< T >( toCopy.maxX ) ), maxY( static_cast< T >( toCopy.maxY ) ), maxZ( static_cast< T >( toCopy.maxZ ) ), maxW( static_cast< T >( toCopy.maxW ) ) {
}

template< typename T >
Bounds< T, 4 >::Bounds( const T minX, const T minY, const T minZ, const T minW, const T maxX, const T maxY, const T maxZ, const T maxW ) :
    minX( minX ), minY( minY ), minZ( minZ ), minW( minW ),
    maxX( maxX ), maxY( maxY ), maxZ( maxZ ), maxW( maxW ) {
}

template< typename T >
void Bounds< T, 4 >::set( const T minX, const T minY, const T minZ, const T minW, const T maxX, const T maxY, const T maxZ, const T maxW ) {
    Bounds::minX = minX;
    Bounds::minY = minY;
    Bounds::minZ = minZ;
    Bounds::minW = minW;

    Bounds::maxX = maxX;
    Bounds::maxY = maxY;
    Bounds::maxZ = maxZ;
    Bounds::maxW = maxW;
}

template< typename T >
void Bounds< T, 4 >::get( T& minXOut, T& minYOut, T& minZOut, T& minWOut, T& maxXOut, T& maxYOut, T& maxZOut, T& maxWOut ) const {
    minXOut = minX;
    minYOut = minY;
    minZOut = minZ;
    minWOut = minW;

    maxXOut = maxX;
    maxYOut = maxY;
    maxZOut = maxZ;
    maxWOut = maxW;
}

template< typename T >
void Bounds< T, 4 >::setPosition( const Point< T, 4 >& mins ) {
    maxX = mins.x + ( maxX - minX );
    maxY = mins.y + ( maxY - minY );
    maxZ = mins.z + ( maxZ - minZ );
    maxW = mins.w + ( maxW - minW );

    minX = mins.x;
    minY = mins.y;
    minZ = mins.z;
    minW = mins.w;
}

template< typename T >
void Bounds< T, 4 >::setPosition( const T minX, const T minY, const T minZ, const T minW ) {
    maxX = minX + ( maxX - Bounds::minX );
    maxY = minY + ( maxY - Bounds::minY );
    maxZ = minZ + ( maxZ - Bounds::minZ );
    maxW = minW + ( maxW - Bounds::minW );

    Bounds::minX = minX;
    Bounds::minY = minY;
    Bounds::minZ = minZ;
    Bounds::minW = minW;
}

template< typename T >
void Bounds< T, 4 >::getPosition( T& minXOut, T& minYOut, T& minZOut, T& minWOut ) const {
    minXOut = minX;
    minYOut = minY;
    minZOut = minZ;
    minWOut = minW;
}

template< typename T >
void Bounds< T, 4 >::setSize( const Size< T, 4 >& sizes ) {
    maxs.x = mins.x + sizes.width;
    maxs.y = mins.y + sizes.length;
    maxs.z = mins.z + sizes.height;
    maxs.w = mins.w + sizes.depth;
}

template< typename T >
Size< T, 4 > Bounds< T, 4 >::getSize() const {
    return Size< T, 4 >(
        maxs.x - mins.x,
        maxs.y - mins.y,
        maxs.z - mins.z,
        maxs.w - mins.w
    );
}

template< typename T >
void Bounds< T, 4 >::setSize( const T width, const T length, const T height, const T depth ) {
    maxX = minX + width;
    maxY = minY + length;
    maxZ = minZ + height;
    maxW = minW + depth;
}

template< typename T >
void Bounds< T, 4 >::getSize( T& widthOut, T& lengthOut, T& heightOut, T& depthOut ) const {
    widthOut  = maxX - minX;
    lengthOut = maxY - minY;
    heightOut = maxZ - minZ;
    depthOut  = maxW - minW;
}

template< typename T >
void Bounds< T, 4 >::setWidth( const T width ) {
    maxX = minX + width;
}

template< typename T >
T Bounds< T, 4 >::getWidth() const {
    return maxX - minX;
}

template< typename T >
void Bounds< T, 4 >::setLength( const T length ) {
    maxY = minY + length;
}

template< typename T >
T Bounds< T, 4 >::getLength() const {
    return maxY - minY;
}

template< typename T >
void Bounds< T, 4 >::setHeight( const T height ) {
    maxZ = minZ + height;
}

template< typename T >
T Bounds< T, 4 >::getHeight() const {
    return maxZ - minZ;
}

template< typename T >
void Bounds< T, 4 >::setDepth( const T depth ) {
    maxW = minW + depth;
}

template< typename T >
T Bounds< T, 4 >::getDepth() const {
    return maxW - minW;
}

template< typename T >
void Bounds< T, 4 >::setPositionAndSize( const Point< T, 4 >& mins, const Size< T, 4 >& sizes ) {
    Bounds::mins = mins;

    maxX = minX + sizes.width;
    maxY = minY + sizes.length;
    maxZ = minZ + sizes.height;
    maxW = minW + sizes.depth;
}

template< typename T >
void Bounds< T, 4 >::getPositionAndSize( Point< T, 4 >& minsOut, Size< T, 4 >& sizesOut ) const {
    minsOut = mins;

    sizesOut.width  = maxX - minX;
    sizesOut.length = maxY - minY;
    sizesOut.height = maxZ - minZ;
    sizesOut.depth  = maxW - minW;
}

template< typename T >
void Bounds< T, 4 >::setPositionAndSize( const T minX, const T minY, const T minZ, const T minW, const Size< T, 4 >& sizes ) {
    Bounds::minX = minX;
    Bounds::minY = minY;
    Bounds::minZ = minZ;
    Bounds::minW = minW;

    maxX = minX + sizes.width;
    maxY = minY + sizes.length;
    maxZ = minZ + sizes.height;
    maxW = minW + sizes.depth;
}

template< typename T >
void Bounds< T, 4 >::getPositionAndSize( T& minXOut, T& minYOut, T& minZOut, T& minWOut, Size< T, 4 >& sizesOut ) const {
    minXOut = minX;
    minYOut = minY;
    minZOut = minZ;
    minWOut = minW;

    sizesOut.width  = maxX - minX;
    sizesOut.length = maxY - minY;
    sizesOut.height = maxZ - minZ;
    sizesOut.depth  = maxW - minW;
}

template< typename T >
void Bounds< T, 4 >::setPositionAndSize( const Point< T, 4 >& mins, const T width, const T length, const T height, const T depth ) {
    Bounds::mins = mins;

    maxX = minX + width;
    maxY = minY + length;
    maxZ = minZ + height;
    maxW = minW + depth;
}

template< typename T >
void Bounds< T, 4 >::getPositionAndSize( Point< T, 4 >& minsOut, T& widthOut, T& lengthOut, T& heightOut, T& depthOut ) const {
    minsOut = mins;

    widthOut   = maxX - minX;
    lengthOut  = maxY - minY;
    heightOut  = maxZ - minZ;
    depthOut   = maxW - minW;
}

template< typename T >
void Bounds< T, 4 >::setPositionAndSize( const T minX, const T minY, const T minZ, const T minW, const T width, const T length, const T height, const T depth ) {
    Bounds::minX = minX;
    Bounds::minY = minY;
    Bounds::minZ = minZ;
    Bounds::minW = minW;

    maxX = minX + width;
    maxY = minY + length;
    maxZ = minZ + height;
    maxW = minW + depth;
}

template< typename T >
void Bounds< T, 4 >::getPositionAndSize( T& minXOut, T& minYOut, T& minZOut, T& minWOut, T& widthOut, T& lengthOut, T& heightOut, T& depthOut ) const {
    minXOut = minX;
    minYOut = minY;
    minZOut = minZ;
    minWOut = minW;

    widthOut  = maxX - minX;
    lengthOut = maxY - minY;
    heightOut = maxZ - minZ;
    depthOut  = maxW - minW;
}

template< typename T >
Point< T, 4 > Bounds< T, 4 >::getCenter() const {
    return Point< T, 4 >(
        ( maxX + minX ) / static_cast<T>( 2 ),
        ( maxY + minY ) / static_cast<T>( 2 ),
        ( maxZ + minZ ) / static_cast<T>( 2 ),
        ( maxW + minW ) / static_cast<T>( 2 )
    );
}

template< typename T >
void Bounds< T, 4 >::zero() {
    minX = 0;
    minY = 0;
    minZ = 0;
    minW = 0;

    maxX = 0;
    maxY = 0;
    maxZ = 0;
    maxW = 0;
}

template< typename T >
bool Bounds< T, 4 >::isZero() const {
    return minX == 0 &&
           minY == 0 &&
           minZ == 0 &&
           minW == 0 &&
           maxX == 0 &&
           maxY == 0 &&
           maxZ == 0 &&
           maxW == 0;
}

template< typename T >
void Bounds< T, 4 >::normalize() {
    minMax( minX, maxX );
    minMax( minY, maxY );
    minMax( minZ, maxZ );
    minMax( minW, maxW );
}

template< typename T >
bool Bounds< T, 4 >::isNormal() const {
    return minX <= maxX &&
           minY <= maxY &&
           minZ <= maxZ &&
           minW <= maxW;
}

template< typename T >
bool Bounds< T, 4 >::contains( const Point< T, 4 >& point ) const {
    return point.x >= minX && point.x <= maxX &&
           point.y >= minY && point.y <= maxY &&
           point.z >= minZ && point.z <= maxZ &&
           point.w >= minW && point.w <= maxW;
}

template< typename T >
bool Bounds< T, 4 >::contains_mIME( const Point< T, 4 >& point ) const {
    return point.x >= minX && point.x < maxX &&
           point.y >= minY && point.y < maxY &&
           point.z >= minZ && point.z < maxZ &&
           point.w >= minW && point.w < maxW;
}

template< typename T >
void Bounds< T, 4 >::include( const Point< T, 4 >& point ) {
    electMinMax( minX, maxX, point.x );
    electMinMax( minY, maxY, point.y );
    electMinMax( minZ, maxZ, point.z );
    electMinMax( minW, maxW, point.w );
}

template< typename T >
bool Bounds< T, 4 >::intersects( const Bounds< T, 4 >& bounds ) const {
    return minX <= bounds.maxX &&
           minY <= bounds.maxY &&
           minZ <= bounds.maxZ &&
           minW <= bounds.maxW &&
           maxX >= bounds.minX &&
           maxY >= bounds.minY &&
           maxZ >= bounds.minZ &&
           maxW >= bounds.minW;
}

template< typename T >
bool Bounds< T, 4 >::intersects_EE( const Bounds< T, 4 >& bounds ) const {
    return minX < bounds.maxX &&
           minY < bounds.maxY &&
           minZ < bounds.maxZ &&
           minW < bounds.maxW &&
           maxX > bounds.minX &&
           maxY > bounds.minY &&
           maxZ > bounds.minZ &&
           maxW > bounds.minW;
}

template< typename T >
template< typename T2 >
Bounds< T, 4 >& Bounds< T, 4 >::operator =( const Bounds< T2, 4 >& right ) {
    minX = static_cast< T >( right.minX );
    minY = static_cast< T >( right.minY );
    minZ = static_cast< T >( right.minZ );
    minW = static_cast< T >( right.minW );

    maxX = static_cast< T >( right.maxX );
    maxY = static_cast< T >( right.maxY );
    maxZ = static_cast< T >( right.maxZ );
    maxW = static_cast< T >( right.maxW );

    return (*this);
}

template< typename T >
bool operator ==( const Bounds< T, 4 >& left, const Bounds< T, 4 >& right ) {
    return left.minX == right.minX &&
           left.minY == right.minY &&
           left.minZ == right.minZ &&
           left.minW == right.minW &&
           left.maxX == right.maxX &&
           left.maxY == right.maxY &&
           left.maxZ == right.maxZ &&
           left.maxW == right.maxW;
}

template< typename T >
bool operator !=( const Bounds< T, 4 >& left, const Bounds< T, 4 >& right ) {
    return left.minX != right.minX ||
           left.minY != right.minY ||
           left.minZ != right.minZ ||
           left.minW != right.minW ||
           left.maxX != right.maxX ||
           left.maxY != right.maxY ||
           left.maxZ != right.maxZ ||
           left.maxW != right.maxW;
}

template< typename T >
void clamp( Point< T, 4 >& pointInOut, const Bounds< T, 4 >& bounds ) {
    clamp( pointInOut.x, bounds.minX, bounds.maxX );
    clamp( pointInOut.y, bounds.minY, bounds.maxY );
    clamp( pointInOut.z, bounds.minZ, bounds.maxZ );
    clamp( pointInOut.w, bounds.minW, bounds.maxW );
}

template< typename T >
Point< T, 4 > clampedPoint( const Point< T, 4 >& point, const Bounds< T, 4 >& bounds ) {
    return Point< T, 4 >(
        clampedValue( point.x, bounds.minX, bounds.maxX ),
        clampedValue( point.y, bounds.minY, bounds.maxY ),
        clampedValue( point.z, bounds.minZ, bounds.maxZ ),
        clampedValue( point.w, bounds.minW, bounds.maxW )
    );
}

template< typename T >
Bounds< T, 4 > intersect( const Bounds< T, 4 >& a, const Bounds< T, 4 >& b ) {
    return Bounds< T, 4 >(
        max( a.minX, b.minX ),
        max( a.minY, b.minY ),
        max( a.minZ, b.minZ ),
        max( a.minW, b.minW ),
        min( a.maxX, b.maxX ),
        min( a.maxY, b.maxY ),
        min( a.maxZ, b.maxZ ),
        min( a.maxW, b.maxW )
    );
}




//Types
template< typename T >
using Bounds4  = Bounds< T, 4 >;
using Bounds4i = Bounds4< int32 >;
using Bounds4f = Bounds4< float >;
using Bounds4d = Bounds4< double >;




} //namespace Brimstone




#endif //BS_BOUNDS_BOUNDS4_HPP
