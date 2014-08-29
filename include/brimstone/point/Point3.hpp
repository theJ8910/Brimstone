/*
point/Point3.hpp
-----------------------
Copyright (c) 2014, theJ89

Description:
    Specialization of Point for 3D points.
    Adds the following typedefs for convenience:
        Point3< T >: Point<T,3>
        Point3i:     Point<int32,3>
        Point3f:     Point<float,3>
        Point3d:     Point<double,3>
*/
#ifndef BS_POINT_POINT3_HPP
#define BS_POINT_POINT3_HPP




//Includes
#include <brimstone/point/PointN.hpp>   //Point




//Macros
#define BS_BASEPOINT3_DECLARE_INHERITED_METHODS( className, spec2 )             \
    className( const T x, const T y, const T z );
#define BS_BASEPOINT3_DEFINE_INHERITED_METHODS( className, tmpl, spec, spec2 )  \
    tmpl                                                                        \
    className spec::className( const T x, const T y, const T z ) :              \
        BaseClass( x, y, z ) {                                                  \
    }
namespace Brimstone {
namespace Private {

template< typename T >
class BasePoint< T, 3 > {
public:
//C4201: nonstandard extension used : nameless struct/union
//It's a non-standard feature, but VC++, G++, and LLVM support it so it shouldn't be too much of an issue
#pragma warning( push )
#pragma warning( disable: 4201 )

    union {
        T data[3];
        struct { T x, y, z; };
    };

#pragma warning( pop )
public:
    BS_ARRAY_DECLARE_INHERITED_METHODS( BasePoint, T )
    BS_BASEPOINT_DECLARE_INHERITED_METHODS( BasePoint, 3, BS_SPEC_2( T2, 3 ) )
    BS_BASEPOINT3_DECLARE_INHERITED_METHODS( BasePoint, BS_SPEC_2( T2, 3 ) )

    //Generic methods
    BS_ARRAY_DECLARE_METHODS( BasePoint, T )
    BS_BASEPOINT_DECLARE_METHODS( 3 )

    //Point3-specific methods
    void set( const T x, const T y, const T z );
    void get( T& xOut, T& yOut, T& zOut ) const;
};

BS_ARRAY_DEFINE_METHODS( BasePoint, T, data, BS_TMPL_1( typename T ), BS_SPEC_2( T, 3 ) )

template< typename T >
inline BasePoint< T, 3 >::BasePoint( const T& elem ) :
    x( elem ),
    y( elem ),
    z( elem ) {
}

template< typename T >
template< typename T2 >
inline BasePoint< T, 3 >::BasePoint( const T2& cppRange ) :
    x( cppRange[0] ),
    y( cppRange[1] ),
    z( cppRange[2] ) {
    BS_ASSERT_SIZE( rangeSize( cppRange ), 3 );
}

template< typename T >
inline BasePoint< T, 3 >::BasePoint( std::initializer_list< T > il ) :
    x( *( std::begin( il )     ) ),
    y( *( std::begin( il ) + 1 ) ),
    z( *( std::begin( il ) + 2 ) ) {
    BS_ASSERT_SIZE( rangeSize( il ), 3 );
}

template< typename T >
template< typename T2 >
inline void BasePoint< T, 3 >::set( const T2& cppRange ) {
    BS_ASSERT_SIZE( rangeSize( cppRange ), 3 );
    x = cppRange[0];
    y = cppRange[1];
    z = cppRange[2];
}

template< typename T >
inline void BasePoint< T, 3 >::set( std::initializer_list< T > il ) {
    BS_ASSERT_SIZE( rangeSize( il ), 3 );
    auto it = std::begin( il );
    x = *( it     );
    y = *( it + 1 );
    z = *( it + 2 );
}

template< typename T >
template< typename T2 >
inline void BasePoint< T, 3 >::get( T2& cppRangeOut ) const {
    BS_ASSERT_SIZE( rangeSize( cppRangeOut ), 3 );
    cppRangeOut[0] = x;
    cppRangeOut[1] = y;
    cppRangeOut[2] = z;
}

template< typename T >
inline void BasePoint< T, 3 >::fill( const T& elem ) {
    x = elem;
    y = elem;
    z = elem;
}

template< typename T >
BasePoint< T, 3 >::BasePoint()
#ifdef BS_ZERO
    : x( 0 ), y( 0 ), z( 0 )
#endif //BS_ZERO
{
}

template< typename T >
template< typename T2 >
BasePoint< T, 3 >::BasePoint( const BasePoint< T2, 3 >& toCopy ) :
    x( static_cast<T>( toCopy.x ) ),
    y( static_cast<T>( toCopy.y ) ),
    z( static_cast<T>( toCopy.z ) ) {
}

template< typename T >
void BasePoint< T, 3 >::zero() {
    x = 0;
    y = 0;
    z = 0;
}

template< typename T >
bool BasePoint< T, 3 >::isZero() const {
    return x == 0 &&
           y == 0 &&
           z == 0;
}

template< typename T >
BasePoint< T, 3 >::BasePoint( const T x, const T y, const T z ) :
    x( x ), y( y ), z( z ) {
}

template< typename T >
void BasePoint< T, 3 >::set( const T x, const T y, const T z ) {
    BasePoint::x = x;
    BasePoint::y = y;
    BasePoint::z = z;
}

template< typename T >
void BasePoint< T, 3 >::get( T& xOut, T& yOut, T& zOut ) const {
    xOut = x;
    yOut = y;
    zOut = z;
}

template< typename T >
template< typename T2 >
BasePoint< T, 3 >& BasePoint< T, 3 >::operator =( const BasePoint< T2, 3 >& right ) {
    x = static_cast< T >( right.x );
    y = static_cast< T >( right.y );
    z = static_cast< T >( right.z );

    return (*this);
}

}




template< typename T >
class Point< T, 3 > : public Private::BasePoint< T, 3 > {
private:
    typedef Private::BasePoint< T, 3 > BaseClass;
public:
    //Specializations of generic methods
    BS_ARRAY_DECLARE_INHERITED_METHODS( Point, T )
    BS_BASEPOINT_DECLARE_INHERITED_METHODS( Point, 3, BS_SPEC_2( T2, 3 ) )
    BS_BASEPOINT3_DECLARE_INHERITED_METHODS( Point, BS_SPEC_2( T2, 3 ) )
    BS_POINT_DECLARE_METHODS( 3 )
};
BS_ARRAY_DEFINE_INHERITED_METHODS( Point, T, BaseClass, BS_TMPL_1( typename T ), BS_SPEC_2( T, 3 ) )
BS_BASEPOINT_DEFINE_INHERITED_METHODS( Point, 3, BS_TMPL_1( typename T ), BS_SPEC_2( T, 3 ), BS_SPEC_2( T2, 3 ) )
BS_BASEPOINT3_DEFINE_INHERITED_METHODS( Point, BS_TMPL_1( typename T ), BS_SPEC_2( T, 3 ), BS_SPEC_2( T2, 3 ) );
BS_POINT_DEFINE_METHODS( 3, BS_TMPL_1( typename T ) )



template< typename T >
std::ostream& operator <<( std::ostream& left, const Point< T, 3 >& right ) {
    return left << "( "
                << ( boost::format( "%|.5f|" ) % right.x ).str() << ", "
                << ( boost::format( "%|.5f|" ) % right.y ).str() << ", "
                << ( boost::format( "%|.5f|" ) % right.z ).str()
                << " )";
}

template< typename T >
bool operator ==( const Point< T, 3 >& left, const Point< T, 3 >& right ) {
    return left.x == right.x &&
           left.y == right.y &&
           left.z == right.z;
}

template< typename T >
bool operator !=( const Point< T, 3 >& left, const Point< T, 3 >& right ) {
    return left.x != right.x ||
           left.y != right.y ||
           left.z != right.z;
}

template< typename T >
T distanceSq( const Point< T, 3 >& left, const Point< T, 3 >& right ) {
    T xoff = ( right.x - left.x );
    T yoff = ( right.y - left.y );
    T zoff = ( right.z - left.z );

    return xoff * xoff +
           yoff * yoff +
           zoff * zoff;
}

template< typename T >
T manhattan( const Point< T, 3 >& left, const Point< T, 3 >& right ) {
    return abs( right.x - left.x ) +
           abs( right.y - left.y ) +
           abs( right.z - left.z );
}




//Typedefs
template< typename T >
using Point3 = Point< T, 3 >;
typedef Point3< int32  > Point3i;
typedef Point3< float  > Point3f;
typedef Point3< double > Point3d;

}




#endif //BS_POINT_POINT3_HPP