/*
Bounds2.hpp
-----------------------
Copyright (c) 2014, theJ89

Description:
    Defines BoundsN, a class for axis-aligned bounding rectangles, boxes, and similar higher order geometry.
    A 2D BoundsN is called a Rectangle.
    A 3D BoundsN is called a Box.

    A BoundsN is defined by two points: mins and maxs.
    These two points are the corners of the Bounds, containing the minimum
    and maximum coordinates of the area the BoundsN spans, respectively.

    For example, a Bounds2 with a mins of (1,1) and a maxs of (3,2) would look like so:

      3
                maxs
      2     _________x
    y      |         |
      1    x_________|
        mins
      0    1    2    3
              x

    Depending on how the coordinate system is laid out, these edges can be called different things.
    For example, in the screen's coordinate system, the edges in a rectangle correspond to:
        mins.x: left
        mins.y: top
        maxs.x: right
        maxs.y: bottom

    Methods are provided to set/get the width/height of the rectangle and set/get the "position" of the
    rectangle ( assumed to be the (minX, minY) corner of the rectangle ).

    Rect is a template class that can take any numerical type.
*/
#ifndef BS_BOUNDS_BOUNDSN_HPP
#define BS_BOUNDS_BOUNDSN_HPP




//Includes
#include <iostream>                     //std::ostream
#include <initializer_list>             //std::initializer_list
#include <iterator>                     //std::begin, std::end
#include <algorithm>                    //std::fill

#include <brimstone/types.hpp>          //int32
#include <brimstone/util/Macros.hpp>    //BS_ASSERT_NON_NULLPTR, BS_ASSERT_SIZE, etc
#include <brimstone/Point.hpp>          //PointN
#include <brimstone/util/Clamp.hpp>     //clamp




//Macros
#define BS_BOUNDS_DECLARE_METHODS( N )                                                                  \
    BS_ARRAY_DECLARE_INHERITED_METHODS( BoundsN, T )                                                    \
    BS_ARRAY_DECLARE_METHODS( BoundsN, T )                                                              \
    BoundsN();                                                                                          \
    BoundsN( const PointN< T, N >& mins, const PointN< T, N >& maxs );                                  \
    template< typename T2 >                                                                             \
    BoundsN( const BoundsN< T2, N >& toCopy );                                                          \
                                                                                                        \
    void set( const PointN< T, N >& mins, const PointN< T, N >& maxs );                                 \
    void get( PointN< T, N >& minsOut, PointN< T, N >& maxsOut ) const;                                 \
                                                                                                        \
    void setDimension( const size_t component, const T difference );                                    \
    T    getDimension( const size_t component ) const;                                                  \
                                                                                                        \
    void zero();                                                                                        \
    bool isZero() const;                                                                                \
                                                                                                        \
    template< typename T2 >                                                                             \
    BoundsN< T, N >& operator =( const BoundsN< T2, N >& toCopy );

#define BS_BOUNDS_DEFINE_METHODS( N, tmpl )                                                             \
    BS_ARRAY_DEFINE_METHODS( BoundsN, T, data, tmpl, BS_SPEC_2( T, N ) )                                \
    tmpl                                                                                                \
    BoundsN< T, N >::BoundsN( const PointN< T, N >& mins, const PointN< T, N >& maxs ) :                \
        mins( mins ),                                                                                   \
        maxs( maxs ) {                                                                                  \
    }                                                                                                   \
    tmpl                                                                                                \
    void BoundsN< T, N >::set( const PointN< T, N >& mins, const PointN< T, N >& maxs ) {               \
        BoundsN::mins = mins;                                                                           \
        BoundsN::maxs = maxs;                                                                           \
    }                                                                                                   \
    tmpl                                                                                                \
    void BoundsN< T, N >::get( PointN< T, N >& minsOut, PointN< T, N >& maxsOut ) const {               \
        minsOut = mins;                                                                                 \
        maxsOut = maxs;                                                                                 \
    }                                                                                                   \
    tmpl                                                                                                \
    void BoundsN< T, N >::setDimension( const size_t component, const T difference ) {                  \
        BS_ASSERT_INDEX( component, N - 1 );                                                            \
        maxs[ component ] = mins[ component ] + difference;                                             \
    }                                                                                                   \
    tmpl                                                                                                \
    T BoundsN< T, N >::getDimension( const size_t component ) const {                                   \
        BS_ASSERT_INDEX( component, N - 1 );                                                            \
        return maxs[ component ] - mins[ component ];                                                   \
    }


namespace Brimstone {

template< typename T, size_t N >
class BoundsN {
public:
//C4201: nonstandard extension used : nameless struct/union
//It's a non-standard feature, but VC++, G++, and LLVM support it so it shouldn't be too much of an issue
#pragma warning( push )
#pragma warning( disable: 4201 )

    union {
        struct {
            PointN< T, N > mins;
            PointN< T, N > maxs;
        };
        T data[ 2*N ];
    };

#pragma warning( pop )
public:
    BS_BOUNDS_DECLARE_METHODS( N )
};

BS_ARRAY_DEFINE_GENERIC_METHODS( BoundsN, T, data, BS_TMPL_2( typename T, size_t N ), BS_SPEC_2( T, N ) )
BS_BOUNDS_DEFINE_METHODS( N, BS_TMPL_2( typename T, size_t N ) )

template< typename T, size_t N >
BoundsN< T, N >::BoundsN() {
#ifdef BS_ZERO
for( size_t i = 0; i < 2*N; ++i )
    data[i] = 0;
#endif //BS_ZERO
}

template< typename T, size_t N >
template< typename T2 >
BoundsN< T, N >::BoundsN( const BoundsN< T2, N >& toCopy ) {
    (*this) = toCopy;
}

template< typename T, size_t N >
void BoundsN< T, N >::zero() {
    std::fill( std::begin( data ), std::end( data ), static_cast< T >( 0 ) );
}

template< typename T, size_t N >
bool BoundsN< T, N >::isZero() const {
    for( size_t i = 0; i < 2*N; ++i )
        if( data[i] != 0 )
            return false;
    return true;
}

template< typename T, size_t N >
template< typename T2 >
BoundsN< T, N >& BoundsN< T, N >::operator =( const BoundsN< T2, N >& right ) {
    for( size_t i = 0; i < 2*N; ++i )
        data[i] = static_cast<T>( right.data[i] );

    return (*this);
}

template< typename T, size_t N >
std::ostream& operator <<( std::ostream& left, const BoundsN< T, N >& right ) {
    return left << "[ " << right.mins << ", " << right.maxs << " ]";
}

template< typename T, size_t N >
bool operator ==( const BoundsN< T, N >& left, const BoundsN< T, N >& right ) {
    for( size_t i = 0; i < 2*N; ++i )
        if( left.data[ i ] != right.data[ i ] )
            return false;
    return true;
}

template< typename T, size_t N >
bool operator !=( const BoundsN< T, N >& left, const BoundsN< T, N >& right ) {
    for( size_t i = 0; i < 2*N; ++i )
        if( left.data[ i ] != right.data[ i ] )
            return true;
    return false;
}

template< typename T, size_t N >
void clamp( PointN< T, N >& pointInOut, const BoundsN< T, N >& bounds ) {
    for( size_t i = 0; i < N; ++i )
        clamp( pointInOut[i], bounds.mins[i], bounds.maxs[i] );
}

template< typename T, size_t N >
PointN< T, N > clampedPoint( const PointN< T, N >& point, const BoundsN< T, N >& bounds ) {
    PointN< T, N > pointOut;

    for( size_t i = 0; i < N; ++i )
        pointOut[i] = clampedValue( point[i], bounds.mins[i], bounds.maxs[i] );

    return pointOut;
}

}




#endif //BS_BOUNDS_BOUNDSN_HPP