/*
bounds/Bounds4.hpp
-----------------------
Copyright (c) 2014, theJ89

Description:
    Specialization of Bounds for 4D bounds (tesseracts).
    Adds the following typedefs for convenience:
        * Bounds4< T >: Bounds<T,4>
        * Bounds4i:     Bounds<int32,4>
        * Bounds4f:     Bounds<float,4>
        * Bounds4d:     Bounds<double,4>
*/
#ifndef BS_BOUNDS_BOUNDS4_HPP
#define BS_BOUNDS_BOUNDS4_HPP




//Includes
#include <brimstone/bounds/BoundsN.hpp>




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
    Bounds( const Point< T, 4 >& mins, const T width, const T length, const T height, const T wlength );

    //Set / get the bounds individually
    void set( const T minX, const T minY, const T minZ, const T minW, const T maxX, const T maxY, const T maxZ, const T maxW );
    void get( T& minXOut, T& minYOut, T& minZOut, T& minWOut, T& maxXOut, T& maxYOut, T& maxZOut, T& maxWOut ) const;

    //Set / get the width / length / height, treating (minX, minY, minZ) as an anchor
    void    setDimensions( const T width, const T length, const T height, const T wlength );
    void    getDimensions( T& widthOut, T& lengthOut, T& heightOut, T& wlengthOut ) const;

    void    setWidth( const T width );
    T       getWidth() const;

    void    setLength( const T length );
    T       getLength() const;

    void    setHeight( const T height );
    T       getHeight() const;

    void    setWLength( const T wlength );
    T       getWLength() const;

    //Miscellaneous utility methods
    T       getVolume() const;
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
Bounds< T, 4 >::Bounds( const Point< T, 4 >& mins, const T width, const T length, const T height, const T wlength ) :
    mins( mins ),
    maxX( mins.x + width   ),
    maxY( mins.y + length  ),
    maxZ( mins.z + height  ),
    maxW( mins.w + wlength ) {
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
void Bounds< T, 4 >::setDimensions( const T width, const T length, const T height, const T wlength ) {
    maxX = minX + width;
    maxY = minY + length;
    maxZ = minZ + height;
    maxW = minW + wlength;
}

template< typename T >
void Bounds< T, 4 >::getDimensions( T& widthOut, T& lengthOut, T& heightOut, T& wlengthOut ) const {
    widthOut   = maxX - minX;
    lengthOut  = maxY - minY;
    heightOut  = maxZ - minZ;
    wlengthOut = maxW - minW;
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
void Bounds< T, 4 >::setWLength( const T wlength ) {
    maxW = minW + wlength;
}

template< typename T >
T Bounds< T, 4 >::getWLength() const {
    return maxW - minW;
}

template< typename T >
T Bounds< T, 4 >::getVolume() const {
    return getWidth() * getLength() * getHeight() * getWLength();
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

//Typedefs
template< typename T >
using Bounds4 = Bounds< T, 4 >;
typedef Bounds4< int32 >    Bounds4i;
typedef Bounds4< float >    Bounds4f;
typedef Bounds4< double >   Bounds4d;

}




#endif //BS_BOUNDS_BOUNDS4_HPP