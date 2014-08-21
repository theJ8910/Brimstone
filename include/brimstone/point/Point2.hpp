/*
Point2.hpp
-----------------------
Copyright (c) 2014, theJ89

Description:
    Specialization of PointN for 2D points.
    Adds the following typedefs for convenience:
        Point2< T >: PointN<T,2>
        Point2i:     PointN<int32,2>
        Point2f:     PointN<float,2>
        Point2d:     PointN<double,2>
*/
#ifndef BS_POINT_POINT2_HPP
#define BS_POINT_POINT2_HPP




//Includes
#include <brimstone/point/PointN.hpp>   //PointN




//Macros
//Inherited methods from Point2 specializaton, PointN, and Array
#define BS_POINT2_DECLARE_INHERITED_METHODS( className, spec2 )             \
    BS_POINT_DECLARE_INHERITED_METHODS( className, 2, spec2 )               \
    className( const T x, const T y );
#define BS_POINT2_DEFINE_INHERITED_METHODS( className, tmpl, spec, spec2 )  \
    BS_POINT_DEFINE_INHERITED_METHODS( className, 2, tmpl, spec, spec2 )    \
    tmpl                                                                    \
    className spec::className( const T x, const T y ) :                     \
        BasePointN( x, y ) {                                                \
    }
namespace Brimstone {
namespace Private {

template< typename T >
class BasePointN< T, 2 > {
public:
//C4201: nonstandard extension used : nameless struct/union
//It's a non-standard feature, but VC++, G++, and LLVM support it so it shouldn't be too much of an issue
#pragma warning( push )
#pragma warning( disable: 4201 )

    union {
        T data[2];
        struct { T x, y; };
    };

#pragma warning( pop )
public:
    BS_POINT2_DECLARE_INHERITED_METHODS( BasePointN, BS_SPEC_2( T2, 2 ) )

    //Generic methods
    BS_POINT_DECLARE_METHODS( 2 )

    //Point2-specific methods
    void set( const T x, const T y );
    void get( T& xOut, T& yOut ) const;
};

BS_ARRAY_DEFINE_METHODS( BasePointN, T, data, BS_TMPL_1( typename T ), BS_SPEC_2( T, 2 ) )

template< typename T >
inline BasePointN< T, 2 >::BasePointN( const T& elem ) :
    x( elem ),
    y( elem ) {
}

template< typename T >
template< typename T2 >
inline BasePointN< T, 2 >::BasePointN( const T2& cppRange ) :
    x( cppRange[0] ),
    y( cppRange[1] ) {
    BS_ASSERT_SIZE( rangeSize( cppRange ), 2 );
}

template< typename T >
inline BasePointN< T, 2 >::BasePointN( std::initializer_list< T > il ) :
    x( *( std::begin( il )     ) ),
    y( *( std::begin( il ) + 1 ) ) {
    BS_ASSERT_SIZE( rangeSize( il ), 2 );
}

template< typename T >
template< typename T2 >
inline void BasePointN< T, 2 >::set( const T2& cppRange ) {
    BS_ASSERT_SIZE( rangeSize( cppRange ), 2 );
    x = cppRange[0];
    y = cppRange[1];
}
template< typename T >
inline void BasePointN< T, 2 >::set( std::initializer_list< T > il ) {
    BS_ASSERT_SIZE( rangeSize( il ), 2 );
    auto it = std::begin( il );
    x = *( it     );
    y = *( it + 1 );
}
template< typename T >
template< typename T2 >
inline void BasePointN< T, 2 >::get( T2& cppRangeOut ) const {
    BS_ASSERT_SIZE( rangeSize( cppRangeOut ), 2 );
    cppRangeOut[0] = x;
    cppRangeOut[1] = y;
}
template< typename T >
inline void BasePointN< T, 2 >::fill( const T& elem ) {
    x = elem;
    y = elem;
}

template< typename T >
BasePointN< T, 2 >::BasePointN()
#ifdef BS_ZERO
    : x( 0 ), y( 0 )
#endif //BS_ZERO
{
}

template< typename T >
template< typename T2 >
BasePointN< T, 2 >::BasePointN( const BasePointN< T2, 2 >& toCopy ) :
    x( static_cast<T>( toCopy.x ) ),
    y( static_cast<T>( toCopy.y ) ) {
}

template< typename T >
BasePointN< T, 2 >::BasePointN( const T x, const T y ) :
    x( x ), y( y ) {
}

template< typename T >
template< typename T2 >
BasePointN< T, 2 >& BasePointN< T, 2 >::operator =( const BasePointN< T2, 2 >& right ) {
    x = static_cast<T>( right.x );
    y = static_cast<T>( right.y );

    return (*this);
}

template< typename T >
void BasePointN< T, 2 >::zero() {
    x = 0;
    y = 0;
}

template< typename T >
bool BasePointN< T, 2 >::isZero() const {
    return x == 0 &&
           y == 0;
}

template< typename T >
void BasePointN< T, 2 >::set( const T x, const T y ) {
    BasePointN::x = x;
    BasePointN::y = y;
}

template< typename T >
void BasePointN< T, 2 >::get( T& xOut, T& yOut ) const {
    xOut = x;
    yOut = y;
}

}

template< typename T >
class PointN< T, 2 > : public Private::BasePointN< T, 2 > {
public:
    //Specializations of generic methods
    BS_POINT2_DECLARE_INHERITED_METHODS( PointN, BS_SPEC_2( T2, 2 ) )
};
BS_POINT2_DEFINE_INHERITED_METHODS( PointN, BS_TMPL_1( typename T ), BS_SPEC_2( T, 2 ), BS_SPEC_2( T2, 2 ) );




template< typename T >
std::ostream& operator <<( std::ostream& left, const PointN< T, 2 >& right ) {
    return left << "( " << right.x << ", " << right.y << " )";
}

template< typename T >
bool operator ==( const PointN< T, 2 >& left, const PointN< T, 2 >& right ) {
    return left.x == right.x &&
           left.y == right.y;
}

template< typename T >
bool operator !=( const PointN< T, 2 >& left, const PointN< T, 2 >& right ) {
    return left.x != right.x ||
           left.y != right.y;
}

template< typename T >
T distanceSq( const PointN< T, 2 >& left, const PointN< T, 2 >& right ) {
    T xoff = ( right.x - left.x );
    T yoff = ( right.y - left.y );

    return xoff * xoff +
           yoff * yoff;
}

template< typename T >
T manhattan( const PointN< T, 2 >& left, const PointN< T, 2 >& right ) {
    return abs( right.x - left.x ) +
           abs( right.y - left.y );
}




//Typedefs
template< typename T >
using Point2 = PointN< T, 2 >;
typedef Point2< int32  > Point2i;
typedef Point2< float  > Point2f;
typedef Point2< double > Point2d;

}




#endif //BS_POINT_POINT2_HPP