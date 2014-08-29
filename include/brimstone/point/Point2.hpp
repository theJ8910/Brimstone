/*
point/Point2.hpp
-----------------------
Copyright (c) 2014, theJ89

Description:
    Specialization of Point for 2D points.
    Adds the following typedefs for convenience:
        Point2< T >: Point<T,2>
        Point2i:     Point<int32,2>
        Point2f:     Point<float,2>
        Point2d:     Point<double,2>
*/
#ifndef BS_POINT_POINT2_HPP
#define BS_POINT_POINT2_HPP




//Includes
#include <brimstone/point/PointN.hpp>   //Point




//Macros
//Inherited methods from Point2 specializaton, Point, and Array
#define BS_BASEPOINT2_DECLARE_INHERITED_METHODS( className, spec2 )             \
    className( const T x, const T y );
#define BS_BASEPOINT2_DEFINE_INHERITED_METHODS( className, tmpl, spec, spec2 )  \
    tmpl                                                                        \
    className spec::className( const T x, const T y ) :                         \
        BaseClass( x, y ) {                                                     \
    }
namespace Brimstone {
namespace Private {

template< typename T >
class BasePoint< T, 2 > {
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
    BS_ARRAY_DECLARE_INHERITED_METHODS( BasePoint, T )
    BS_BASEPOINT_DECLARE_INHERITED_METHODS( BasePoint, 2, BS_SPEC_2( T2, 2 ) )
    BS_BASEPOINT2_DECLARE_INHERITED_METHODS( BasePoint, BS_SPEC_2( T2, 2 ) )

    //Generic methods
    BS_ARRAY_DECLARE_METHODS( BasePoint, T )
    BS_BASEPOINT_DECLARE_METHODS( 2 )

    //Point2-specific methods
    void set( const T x, const T y );
    void get( T& xOut, T& yOut ) const;
};

BS_ARRAY_DEFINE_METHODS( BasePoint, T, data, BS_TMPL_1( typename T ), BS_SPEC_2( T, 2 ) )

template< typename T >
inline BasePoint< T, 2 >::BasePoint( const T& elem ) :
    x( elem ),
    y( elem ) {
}

template< typename T >
template< typename T2 >
inline BasePoint< T, 2 >::BasePoint( const T2& cppRange ) :
    x( cppRange[0] ),
    y( cppRange[1] ) {
    BS_ASSERT_SIZE( rangeSize( cppRange ), 2 );
}

template< typename T >
inline BasePoint< T, 2 >::BasePoint( std::initializer_list< T > il ) :
    x( *( std::begin( il )     ) ),
    y( *( std::begin( il ) + 1 ) ) {
    BS_ASSERT_SIZE( rangeSize( il ), 2 );
}

template< typename T >
template< typename T2 >
inline void BasePoint< T, 2 >::set( const T2& cppRange ) {
    BS_ASSERT_SIZE( rangeSize( cppRange ), 2 );
    x = cppRange[0];
    y = cppRange[1];
}
template< typename T >
inline void BasePoint< T, 2 >::set( std::initializer_list< T > il ) {
    BS_ASSERT_SIZE( rangeSize( il ), 2 );
    auto it = std::begin( il );
    x = *( it     );
    y = *( it + 1 );
}
template< typename T >
template< typename T2 >
inline void BasePoint< T, 2 >::get( T2& cppRangeOut ) const {
    BS_ASSERT_SIZE( rangeSize( cppRangeOut ), 2 );
    cppRangeOut[0] = x;
    cppRangeOut[1] = y;
}
template< typename T >
inline void BasePoint< T, 2 >::fill( const T& elem ) {
    x = elem;
    y = elem;
}

template< typename T >
BasePoint< T, 2 >::BasePoint()
#ifdef BS_ZERO
    : x( 0 ), y( 0 )
#endif //BS_ZERO
{
}

template< typename T >
template< typename T2 >
BasePoint< T, 2 >::BasePoint( const BasePoint< T2, 2 >& toCopy ) :
    x( static_cast<T>( toCopy.x ) ),
    y( static_cast<T>( toCopy.y ) ) {
}

template< typename T >
BasePoint< T, 2 >::BasePoint( const T x, const T y ) :
    x( x ), y( y ) {
}

template< typename T >
template< typename T2 >
BasePoint< T, 2 >& BasePoint< T, 2 >::operator =( const BasePoint< T2, 2 >& right ) {
    x = static_cast<T>( right.x );
    y = static_cast<T>( right.y );

    return (*this);
}

template< typename T >
void BasePoint< T, 2 >::zero() {
    x = 0;
    y = 0;
}

template< typename T >
bool BasePoint< T, 2 >::isZero() const {
    return x == 0 &&
           y == 0;
}

template< typename T >
void BasePoint< T, 2 >::set( const T x, const T y ) {
    BasePoint::x = x;
    BasePoint::y = y;
}

template< typename T >
void BasePoint< T, 2 >::get( T& xOut, T& yOut ) const {
    xOut = x;
    yOut = y;
}

}

template< typename T >
class Point< T, 2 > : public Private::BasePoint< T, 2 > {
private:
    typedef Private::BasePoint< T, 2 > BaseClass;
public:
    //Specializations of generic methods
    BS_ARRAY_DECLARE_INHERITED_METHODS( Point, T )
    BS_BASEPOINT_DECLARE_INHERITED_METHODS( Point, 2, BS_SPEC_2( T2, 2 ) )
    BS_BASEPOINT2_DECLARE_INHERITED_METHODS( Point, BS_SPEC_2( T2, 2 ) )
    BS_POINT_DECLARE_METHODS( 2 )
};
BS_ARRAY_DEFINE_INHERITED_METHODS( Point, T, BaseClass, BS_TMPL_1( typename T ), BS_SPEC_2( T, 2 ) )
BS_BASEPOINT_DEFINE_INHERITED_METHODS( Point, 2, BS_TMPL_1( typename T ), BS_SPEC_2( T, 2 ), BS_SPEC_2( T2, 2 ) )
BS_BASEPOINT2_DEFINE_INHERITED_METHODS( Point, BS_TMPL_1( typename T ), BS_SPEC_2( T, 2 ), BS_SPEC_2( T2, 2 ) );
BS_POINT_DEFINE_METHODS( 2, BS_TMPL_1( typename T ) )



template< typename T >
std::ostream& operator <<( std::ostream& left, const Point< T, 2 >& right ) {
    return left << "( "
                << ( boost::format( "%|.5f|" ) % right.x ).str() << ", "
                << ( boost::format( "%|.5f|" ) % right.y ).str()
                << " )";
}

template< typename T >
bool operator ==( const Point< T, 2 >& left, const Point< T, 2 >& right ) {
    return left.x == right.x &&
           left.y == right.y;
}

template< typename T >
bool operator !=( const Point< T, 2 >& left, const Point< T, 2 >& right ) {
    return left.x != right.x ||
           left.y != right.y;
}

template< typename T >
T distanceSq( const Point< T, 2 >& left, const Point< T, 2 >& right ) {
    T xoff = ( right.x - left.x );
    T yoff = ( right.y - left.y );

    return xoff * xoff +
           yoff * yoff;
}

template< typename T >
T manhattan( const Point< T, 2 >& left, const Point< T, 2 >& right ) {
    return abs( right.x - left.x ) +
           abs( right.y - left.y );
}




//Typedefs
template< typename T >
using Point2 = Point< T, 2 >;
typedef Point2< int32  > Point2i;
typedef Point2< float  > Point2f;
typedef Point2< double > Point2d;

}




#endif //BS_POINT_POINT2_HPP