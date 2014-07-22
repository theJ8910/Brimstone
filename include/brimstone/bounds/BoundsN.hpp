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
#ifndef BS_BOUNDSN_HPP
#define BS_BOUNDSN_HPP




//Includes
#include <iostream>                     //std::ostream
#include <initializer_list>             //std::initializer_list

#include <brimstone/types.hpp>          //int32
#include <brimstone/util/Macros.hpp>    //BS_ASSERT_NON_NULLPTR, BS_ASSERT_SIZE, etc
#include <brimstone/Point.hpp>          //PointN




//Macros
#define BS_BOUNDS_THIS_TMPL() \
    template< typename T, int N >
#define BS_BOUNDS_TMPL() \
    template< typename T >
#define BS_BOUNDS_DECLARE_METHODS( N ) \
    BoundsN( std::initializer_list< T > il ); \
    BoundsN( const T* const values, const uintN count ); \
    \
    void set( const PointN< T, N >& mins, const PointN< T, N >& maxs ); \
    void get( const PointN< T, N >& minsOut, const PointN< T, N >& maxsOut ) const; \
    \
    void setDimension( const intN component, const T difference ); \
    T    getDimension( const intN component ) const;

#define BS_BOUNDS_DEFINE_METHODS( N, tmpl ) \
    tmpl \
    BoundsN< T, N >::BoundsN( std::initializer_list< T > il ) { \
        set( il.begin(), il.size() ); \
    } \
    tmpl \
    BoundsN< T, N >::BoundsN( const T* const values, const uintN count ) { \
        set( values, count ); \
    } \
    tmpl \
    void BoundsN< T, N >::set( const PointN< T, N >& mins, const PointN< T, N >& maxs ) { \
        BoundsN::mins = mins; \
        BoundsN::maxs = maxs; \
    } \
    \
    tmpl \
    void BoundsN< T, N >::get( const PointN< T, N >& minsOut, const PointN< T, N >& maxsOut ) const { \
        minsOut = mins; \
        maxsOut = maxs; \
    } \
    tmpl \
    void BoundsN< T, N >::setDimension( const intN component, const T difference ) { \
        BS_ASSERT_INDEX( component, N - 1 ); \
        maxs[ component ] = mins[ component ] + difference; \
    } \
    \
    tmpl \
    T BoundsN< T, N >::getDimension( const intN component ) const { \
        BS_ASSERT_INDEX( component, N - 1 ); \
        return maxs[ component ] - mins[ component ]; \
    }
    

namespace Brimstone {

template< typename T, int N >
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

    //Constructors
    BoundsN();
    
    //Bounds represented as an array of minimum and maximum coordinates
    void set( const T* const values, const uintN count );
    void get( T* const valuesOut, const uintN count ) const;

    //Miscellaneous utility methods
    void zero();
    bool isZero() const;

    //Related free functions
    template< typename T2, int N2 >
    friend  std::ostream&       operator <<( std::ostream& left, const BoundsN< T2, N2 >& right );

    template< typename T2, int N2 >
    friend  bool                operator ==( BoundsN< T2, N2 > left, BoundsN< T2, N2 > right );

    template< typename T2, int N2 >
    friend  bool                operator !=( BoundsN< T2, N2 > left, BoundsN< T2, N2 > right );

    template< typename T2, int N2 >
    friend  void                clamp( PointN< T2, N2 >& pointInOut, const BoundsN< T2, N2 >& bounds );

    template< typename T2, int N2 >
    friend  PointN< T2, N2 >    clampedPoint( const PointN< T2, N2 >& point, const BoundsN< T2, N2 >& bounds );
};

template< typename T, int N >
BoundsN< T, N >::BoundsN() {
#ifdef BS_ZERO
for( int i = 0; i < 2*N; ++i )
    data[i] = 0;
#endif //BS_ZERO
}

template< typename T, int N >
void BoundsN< T, N >::set( const T* const values, const uintN count ) {
    BS_ASSERT_NON_NULLPTR( values );
    BS_ASSERT_SIZE( count, 2*N );
    for( int i = 0; i < 2*N; ++i )
        data[i] = values[ i ];
}

template< typename T, int N >
void BoundsN< T, N >::get( T* const valuesOut, const uintN count ) const {
    BS_ASSERT_NON_NULLPTR( valuesOut );
    BS_ASSERT_SIZE( count, 2*N );
    for( int i = 0; i < 2*N; ++i )
        valuesOut[i] = data[i];
}

template< typename T, int N >
void BoundsN< T, N >::zero() {
    for( int i = 0; i < 2*N; ++i )
        data[i] = 0;
}

template< typename T, int N >
bool BoundsN< T, N >::isZero() const {
    for( int i = 0; i < 2*N; ++i )
        if( data[i] != 0 )
            return false;
    return true;
}

template< typename T, int N >
std::ostream& operator <<( std::ostream& left, const BoundsN< T, N >& right ) {
    return left << "[ " << right.mins << ", " << right.maxs << " ]";
}

template< typename T, int N >
bool operator ==( BoundsN< T, N > left, BoundsN< T, N > right ) {
    for( int i = 0; i < 2*N; ++i )
        if( left.data[ i ] != right.data[ i ] )
            return false;
    return true;
}

template< typename T, int N >
bool operator !=( BoundsN< T, N > left, BoundsN< T, N > right ) {
    for( int i = 0; i < 2*N; ++i )
        if( left.data[ i ] != right.data[ i ] )
            return true;
    return false;
}

template< typename T, int N >
void clamp( PointN< T, N >& pointInOut, const BoundsN< T, N >& bounds ) {
    for( int i = 0; i < N; ++i )
        clamp( pointInOut[i], bounds.mins[i], bounds.maxs[i] );
}

template< typename T, int N >
PointN< T, N > clampedPoint( const PointN< T, N >& point, const BoundsN< T, N >& bounds ) {
    PointN< T, N > pointOut;

    for( int i = 0; i < N; ++i )
        pointOut[i] = clampedValue( point[i], bounds.mins[i], bounds.maxs[i] );

    return pointOut;
}

}




#endif //BS_BOUNDSN_HPP