
/*
Bounds3.hpp
-----------------------
Copyright (c) 2014, theJ89

Description:
    Specialization of BoundsN for 3D bounds (boxes).
    Adds the following typedefs for convenience:
        * Bounds3< T >: BoundsN<T,3>
        * Bounds3i:     BoundsN<int32,3>
        * Bounds3f:     BoundsN<float,3>
        * Bounds3d:     BoundsN<double,3>
*/
#ifndef BS_BOUNDS3_HPP
#define BS_BOUNDS3_HPP




//Includes
#include <brimstone/bounds/BoundsN.hpp>




namespace Brimstone {

template< typename T >
class BoundsN< T, 3 > {
public:
//C4201: nonstandard extension used : nameless struct/union
//It's a non-standard feature, but VC++, G++, and LLVM support it so it shouldn't be too much of an issue
#pragma warning( push )
#pragma warning( disable: 4201 )

    union {
        struct {
            PointN< T, 3 > mins;
            PointN< T, 3 > maxs;
        };
        T data[ 6 ];
    };

#pragma warning( pop )
public:
    BS_BOUNDS_DECLARE_METHODS( 3 )

    //Constructors
    BoundsN( const T minX, const T minY, const T minZ, const T maxX, const T maxY, const T maxZ );
    BoundsN( const PointN< T, 3 >& mins, T width, T length, T height );

    //Set / get the bounds individually
    void set( const T minX, const T minY, const T minZ, const T maxX, const T maxY, const T maxZ );
    void get( T& minXOut, T& minYOut, T& minZOut, T& maxXOut, T& maxYOut, T& maxZOut ) const;

    //Set / get the width / length / height, treating (minX, minY, minZ) as an anchor
    void    setDimensions( const T width, const T length, const T height );
    void    getDimensions( T& widthOut, T& lengthOut, T& heightOut ) const;

    void    setWidth( const T width );
    T       getWidth() const;

    void    setLength( const T length );
    T       getLength() const;

    void    setHeight( const T height );
    T       getHeight() const;

    //Miscellaneous utility methods
    T       getVolume() const;
};

BS_BOUNDS_DEFINE_METHODS( 3, BS_BOUNDS_TMPL() )

template< typename T >
BoundsN< T, 3 >::BoundsN()
#ifdef BS_ZERO
    : mins( 0, 0, 0 ), maxs( 0, 0, 0 )
#endif  //BS_ZERO
{
}

template< typename T >
BoundsN< T, 3 >::BoundsN( const T minX, const T minY, const T minZ, const T maxX, const T maxY, const T maxZ ) :
    mins( minX, minY, minZ ), maxs( maxX, maxY, maxZ ) {
}

template< typename T >
BoundsN< T, 3 >::BoundsN( const PointN< T, 3 >& mins, T width, T length, T height ) :
    mins( mins ),
    maxs(
        mins.x + width,
        mins.y + length,
        mins.z + height
    ) {
}

template< typename T >
void BoundsN< T, 3 >::set( const T minX, const T minY, const T minZ, const T maxX, const T maxY, const T maxZ ) {
    mins.x = minX;
    mins.y = minY;
    mins.z = minZ;

    maxs.x = maxX;
    maxs.y = maxY;
    maxs.z = maxZ;
}

template< typename T >
void BoundsN< T, 3 >::get( T& minXOut, T& minYOut, T& minZOut, T& maxXOut, T& maxYOut, T& maxZOut ) const {
    minXOut = mins.x;
    minYOut = mins.y;
    minZOut = mins.z;

    maxXOut = maxs.x;
    maxYOut = maxs.y;
    maxZOut = maxs.z;
}

template< typename T >
void BoundsN< T, 3 >::set( const T* const values, const uintN count ) {
    BS_ASSERT_NON_NULLPTR( values );
    BS_ASSERT_SIZE( count, 6 );

    mins.x = values[ 0 ];
    mins.y = values[ 1 ];
    mins.z = values[ 2 ];

    maxs.x = values[ 3 ];
    maxs.y = values[ 4 ];
    maxs.z = values[ 5 ];
}

template< typename T >
void BoundsN< T, 3 >::get( T* const valuesOut, const uintN count ) const {
    BS_ASSERT_NON_NULLPTR( values );
    BS_ASSERT_SIZE( count, 6 );

    valuesOut[ 0 ] = mins.x;
    valuesOut[ 1 ] = mins.y;
    valuesOut[ 2 ] = mins.z;

    valuesOut[ 3 ] = maxs.x;
    valuesOut[ 4 ] = maxs.y;
    valuesOut[ 5 ] = maxs.z;
}

template< typename T >
void BoundsN< T, 3 >::setDimensions( const T width, const T length, const T height ) {
    maxs.x = mins.x + width;
    maxs.y = mins.y + length;
    maxs.z = mins.z + height;
}

template< typename T >
void BoundsN< T, 3 >::getDimensions( T& widthOut, T& lengthOut, T& heightOut ) const {
    widthOut  = maxs.x - mins.x;
    lengthOut = maxs.y - mins.y;
    heightOut = maxs.z - mins.z;
}

template< typename T >
void BoundsN< T, 3 >::setWidth( const T width ) {
    maxs.x = mins.x + width;
}

template< typename T >
T BoundsN< T, 3 >::getWidth() const {
    return maxs.x - mins.x;
}

template< typename T >
void BoundsN< T, 3 >::setLength( const T length ) {
    maxs.y = mins.y + length;
}

template< typename T >
T BoundsN< T, 3 >::getLength() const {
    return maxs.y - mins.y;
}

template< typename T >
void BoundsN< T, 3 >::setHeight( const T height ) {
    maxs.z = mins.z + height;
}

template< typename T >
T BoundsN< T, 3 >::getHeight() const {
    return maxs.z - mins.z;
}

template< typename T >
T BoundsN< T, 3 >::getVolume() const {
    return getWidth() * getLength() * getHeight();
}

template< typename T >
void BoundsN< T, 3 >::zero() {
    mins.x = 0;
    mins.y = 0;
    mins.z = 0;

    maxs.x = 0;
    maxs.y = 0;
    maxs.z = 0;
}

template< typename T >
bool BoundsN< T, 3 >::isZero() const {
    return mins.x == 0 &&
           mins.y == 0 &&
           mins.z == 0 &&
           maxs.x == 0 &&
           maxs.y == 0 &&
           maxs.z == 0;
}


template< typename T >
bool operator ==( BoundsN< T, 3 > left, BoundsN< T, 3 > right ) {
    return left.mins.x == right.mins.x &&
           left.mins.y == right.mins.y &&
           left.mins.z == right.mins.z &&
           left.maxs.x == right.maxs.x &&
           left.maxs.y == right.maxs.y &&
           left.maxs.z == right.maxs.z;
}

template< typename T >
bool operator !=( BoundsN< T, 3 > left, BoundsN< T, 3 > right ) {
    return left.mins.x != right.mins.x ||
           left.mins.y != right.mins.y ||
           left.mins.z != right.mins.z ||
           left.maxs.x != right.maxs.x ||
           left.maxs.y != right.maxs.y ||
           left.maxs.z != right.maxs.z;
}

template< typename T >
void clamp( PointN< T, 3 >& pointInOut, const BoundsN< T, 3 >& bounds ) {
    clamp( pointInOut.x, bounds.mins.x, bounds.maxs.x );
    clamp( pointInOut.y, bounds.mins.y, bounds.maxs.y );
    clamp( pointInOut.z, bounds.mins.z, bounds.maxs.z );
}

template< typename T >
PointN< T, 3 > clampedPoint( const PointN< T, 3 >& point, const BoundsN< T, 3 >& bounds ) {
    return PointN< T, 3 >(
        clampedValue( point.x, bounds.mins.x, bounds.maxs.x ),
        clampedValue( point.y, bounds.mins.y, bounds.maxs.y ),
        clampedValue( point.z, bounds.mins.z, bounds.maxs.z )
    );
}

//Typedefs
template< typename T >
using Bounds3 = BoundsN< T, 3 >;
typedef Bounds3< int32 >    Bounds3i;
typedef Bounds3< float >    Bounds3f;
typedef Bounds3< double >   Bounds3d;

}




#endif //BS_BOUNDS3_HPP