/*
bounds/BoundsN.hpp
-----------------------
Copyright (c) 2014, theJ89

Description:
    Defines Bounds, a class for axis-aligned bounding rectangles, boxes, and similar higher order geometry.
    A 2D Bounds is called a Rectangle.
    A 3D Bounds is called a Box.
    A 4D Bounds is called a Tesseract.

    Bounds is a template class that implements box-like geometry of the specified dimension,
    and can be provided any numerical type to use internally (int, float, etc).

    A Bounds is defined by two points: mins and maxs.
    These two points are the corners of the Bounds, containing the minimum
    and maximum coordinates of the area the Bounds spans, respectively.

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

    Methods are provided to set/get the dimensions (width/length/height/etc) and
    the "position" ( assumed to be the .mins corner ) of
    the Bounds.

    2D, 3D and 4D specializations of Bounds are provided in Bounds2.hpp, Bounds3.hpp, and Bounds4.hpp, respectively.
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
#include <brimstone/util/Clamp.hpp>     //clamp
#include <brimstone/util/MinMax.hpp>    //min
#include <brimstone/Point.hpp>          //Point




//Macros
#define BS_BOUNDS_DECLARE_METHODS( N )                                                                  \
    Bounds();                                                                                           \
    Bounds( const Point< T, N >& mins, const Point< T, N >& maxs );                                     \
    template< typename T2 >                                                                             \
    Bounds( const Bounds< T2, N >& toCopy );                                                            \
                                                                                                        \
    void set( const Point< T, N >& mins, const Point< T, N >& maxs );                                   \
    void get( Point< T, N >& minsOut, Point< T, N >& maxsOut ) const;                                   \
                                                                                                        \
    void setPosition( const Point< T, N >& mins );                                                      \
    void getPosition( Point< T, N >& minsOut ) const;                                                   \
                                                                                                        \
    void setDimension( const size_t component, const T difference );                                    \
    T    getDimension( const size_t component ) const;                                                  \
                                                                                                        \
    void zero();                                                                                        \
    bool isZero() const;                                                                                \
                                                                                                        \
    void normalize();                                                                                   \
    bool isNormal() const;                                                                              \
                                                                                                        \
    void include( const Point< T, N >& point );                                                         \
    bool contains( const Point< T, N >& point ) const;                                                  \
                                                                                                        \
    template< typename T2 >                                                                             \
    Bounds< T, N >& operator =( const Bounds< T2, N >& toCopy );

#define BS_BOUNDS_DEFINE_METHODS( N, tmpl )                                                             \
    tmpl                                                                                                \
    Bounds< T, N >::Bounds( const Point< T, N >& mins, const Point< T, N >& maxs ) :                    \
        mins( mins ),                                                                                   \
        maxs( maxs ) {                                                                                  \
    }                                                                                                   \
    tmpl                                                                                                \
    void Bounds< T, N >::set( const Point< T, N >& mins, const Point< T, N >& maxs ) {                  \
        Bounds::mins = mins;                                                                            \
        Bounds::maxs = maxs;                                                                            \
    }                                                                                                   \
    tmpl                                                                                                \
    void Bounds< T, N >::get( Point< T, N >& minsOut, Point< T, N >& maxsOut ) const {                  \
        minsOut = mins;                                                                                 \
        maxsOut = maxs;                                                                                 \
    }                                                                                                   \
    tmpl                                                                                                \
    void Bounds< T, N >::getPosition( Point< T, N >& minsOut ) const {                                  \
        minsOut = mins;                                                                                 \
    }                                                                                                   \
    tmpl                                                                                                \
    void Bounds< T, N >::setDimension( const size_t component, const T difference ) {                   \
        BS_ASSERT_INDEX( component, N - 1 );                                                            \
        maxs[ component ] = mins[ component ] + difference;                                             \
    }                                                                                                   \
    tmpl                                                                                                \
    T Bounds< T, N >::getDimension( const size_t component ) const {                                    \
        BS_ASSERT_INDEX( component, N - 1 );                                                            \
        return maxs[ component ] - mins[ component ];                                                   \
    }


namespace Brimstone {

template< typename T, size_t N >
class Bounds {
public:
//C4201: nonstandard extension used : nameless struct/union
//It's a non-standard feature, but VC++, G++, and LLVM support it so it shouldn't be too much of an issue
#pragma warning( push )
#pragma warning( disable: 4201 )

    union {
        struct {
            Point< T, N > mins;
            Point< T, N > maxs;
        };
        T data[ 2*N ];
    };

#pragma warning( pop )
public:
    BS_ARRAY_DECLARE_INHERITED_METHODS( Bounds, T )
    BS_ARRAY_DECLARE_METHODS( Bounds, T )
    BS_BOUNDS_DECLARE_METHODS( N )
};
BS_ARRAY_DEFINE_GENERIC_METHODS( Bounds, T, data, BS_TMPL_2( typename T, size_t N ), BS_SPEC_2( T, N ) )
BS_ARRAY_DEFINE_METHODS( Bounds, T, data, BS_TMPL_2( typename T, size_t N ), BS_SPEC_2( T, N ) )
BS_BOUNDS_DEFINE_METHODS( N, BS_TMPL_2( typename T, size_t N ) )

template< typename T, size_t N >
Bounds< T, N >::Bounds() {
#ifdef BS_ZERO
for( size_t i = 0; i < 2*N; ++i )
    data[i] = 0;
#endif //BS_ZERO
}

template< typename T, size_t N >
template< typename T2 >
Bounds< T, N >::Bounds( const Bounds< T2, N >& toCopy ) {
    (*this) = toCopy;
}

template< typename T, size_t N >
void Bounds< T, N >::setPosition( const Point< T, N >& mins ) {
    for( size_t i = 0; i < N; ++i )
        maxs.data[i] = mins.data[i] + ( maxs.data[i] - Bounds::mins.data[i] );
    Bounds::mins = mins;
}

template< typename T, size_t N >
void Bounds< T, N >::zero() {
    std::fill( std::begin( data ), std::end( data ), static_cast< T >( 0 ) );
}

template< typename T, size_t N >
bool Bounds< T, N >::isZero() const {
    for( size_t i = 0; i < 2*N; ++i )
        if( data[i] != 0 )
            return false;
    return true;
}

/*
Bounds::normalize
-----------------------

Description:
    Makes this Bounds normal.

    For each dimension "i" where mins[i] <= maxs[i],
    mins[i] and maxs[i] are swapped.

Arguments:
    N/A

Returns:
    void:   N/A
*/
template< typename T, size_t N >
void Bounds< T, N >::normalize() {
    for( size_t i = 0; i < N; ++i )
        minMax( mins.data[i], maxs.data[i] );
}

/*
Bounds::isNormal
-----------------------

Description:
    Checks if the bounds is normal.

    A bounds is considered "normal" if, for each dimension "i",
    mins[i] <= maxs[i].

Arguments:
    N/A

Returns:
    bool:   true if the Bounds is normal, false otherwise.
*/
template< typename T, size_t N >
bool Bounds< T, N >::isNormal() const {
    for( size_t i = 0; i < N; ++i )
        if( mins.data[i] > maxs.data[i] )
            return false;
    return true;
}

/*
Bounds::contains
-----------------------

Description:
    Checks if the given point is contained within this Bounds.

    The bounds checks are inclusive; that is, the point is considered inside the Bounds
    if it is on a corner, edge, face, etc. of the Bounds.

Arguments:
    point:  The point to check.

Returns:
    bool:   true if the point is contained within the Bounds, false otherwise.
*/
template< typename T, size_t N >
bool Bounds< T, N >::contains( const Point< T, N >& point ) const {
    for( size_t i = 0; i < N; ++i )
        if( point.data[i] < mins.data[i] || point.data[i] > maxs.data[i] )
            return false;

    return true;
}

/*
Bounds::include
-----------------------

Description:
    Expands this Bounds to include the given point if it doesn't already.

    If the point is not in this Bounds, the Bounds is resized such that
    afterwards the point will be on the extremeties (a corner, edge, face, etc.) of the Bounds.

Arguments:
    point:      The point to include in the Bounds.

Returns:
    void:       N/A
*/
template< typename T, size_t N >
void Bounds< T, N >::include( const Point< T, N >& point ) {
    for( size_t i = 0; i < N; ++i )
        electMinMax( mins.data[i], maxs.data[i], point.data[i] );
}

template< typename T, size_t N >
template< typename T2 >
Bounds< T, N >& Bounds< T, N >::operator =( const Bounds< T2, N >& right ) {
    for( size_t i = 0; i < 2*N; ++i )
        data[i] = static_cast<T>( right.data[i] );

    return (*this);
}

template< typename T, size_t N >
std::ostream& operator <<( std::ostream& left, const Bounds< T, N >& right ) {
    return left << "[ " << right.mins << ", " << right.maxs << " ]";
}

template< typename T, size_t N >
bool operator ==( const Bounds< T, N >& left, const Bounds< T, N >& right ) {
    for( size_t i = 0; i < 2*N; ++i )
        if( left.data[ i ] != right.data[ i ] )
            return false;
    return true;
}

template< typename T, size_t N >
bool operator !=( const Bounds< T, N >& left, const Bounds< T, N >& right ) {
    for( size_t i = 0; i < 2*N; ++i )
        if( left.data[ i ] != right.data[ i ] )
            return true;
    return false;
}

template< typename T, size_t N >
void clamp( Point< T, N >& pointInOut, const Bounds< T, N >& bounds ) {
    for( size_t i = 0; i < N; ++i )
        clamp( pointInOut[i], bounds.mins[i], bounds.maxs[i] );
}

template< typename T, size_t N >
Point< T, N > clampedPoint( const Point< T, N >& point, const Bounds< T, N >& bounds ) {
    Point< T, N > pointOut;

    for( size_t i = 0; i < N; ++i )
        pointOut[i] = clampedValue( point[i], bounds.mins[i], bounds.maxs[i] );

    return pointOut;
}

}




#endif //BS_BOUNDS_BOUNDSN_HPP