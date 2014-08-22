/*
Point4.hpp
-----------------------
Copyright (c) 2014, theJ89

Description:
    Specialization of PointN for 4D points.
    Adds the following typedefs for convenience:
        Point4< T >: PointN<T,4>
        Point4i:     PointN<int32,4>
        Point4f:     PointN<float,4>
        Point4d:     PointN<double,4>
*/
#ifndef BS_POINT_POINT4_HPP
#define BS_POINT_POINT4_HPP




//Includes
#include <brimstone/point/PointN.hpp>   //PointN




//Macros
#define BS_POINT4_DECLARE_INHERITED_METHODS( className, spec2 )                 \
    BS_POINT_DECLARE_INHERITED_METHODS( className, 4, spec2 )                   \
    className( const T x, const T y, const T z, const T w );
#define BS_POINT4_DEFINE_INHERITED_METHODS( className, tmpl, spec, spec2 )      \
    BS_POINT_DEFINE_INHERITED_METHODS( className, 4, tmpl, spec, spec2 )        \
    tmpl                                                                        \
    className spec::className( const T x, const T y, const T z, const T w ) :   \
        BasePointN( x, y, z, w ) {                                              \
    }

namespace Brimstone {
namespace Private {

template< typename T >
class BasePointN< T, 4 > {
public:
//C4201: nonstandard extension used : nameless struct/union
//It's a non-standard feature, but VC++, G++, and LLVM support it so it shouldn't be too much of an issue
#pragma warning( push )
#pragma warning( disable: 4201 )

    union {
        T data[4];
        struct { T x, y, z, w; };
    };

#pragma warning( pop )
public:
    BS_POINT4_DECLARE_INHERITED_METHODS( BasePointN, BS_SPEC_2( T2, 4 ) )

    //Generic methods
    BS_POINT_DECLARE_METHODS( 4 )

    //Point3-specific methods
    void set( const T x, const T y, const T z, const T w );
    void get( T& xOut, T& yOut, T& zOut, T& wOut ) const;
};

BS_ARRAY_DEFINE_METHODS( BasePointN, T, data, BS_TMPL_1( typename T ), BS_SPEC_2( T, 4 ) )

template< typename T >
inline BasePointN< T, 4 >::BasePointN( const T& elem ) :
    x( elem ),
    y( elem ),
    z( elem ),
    w( elem ) {
}

template< typename T >
template< typename T2 >
inline BasePointN< T, 4 >::BasePointN( const T2& cppRange ) :
    x( cppRange[0] ),
    y( cppRange[1] ),
    z( cppRange[2] ),
    w( cppRange[3] ) {
    BS_ASSERT_SIZE( rangeSize( cppRange ), 4 );
}

template< typename T >
inline BasePointN< T, 4 >::BasePointN( std::initializer_list< T > il ) :
    x( *( std::begin( il )     ) ),
    y( *( std::begin( il ) + 1 ) ),
    z( *( std::begin( il ) + 2 ) ),
    w( *( std::begin( il ) + 3 ) ) {
    BS_ASSERT_SIZE( rangeSize( il ), 4 );
}

template< typename T >
template< typename T2 >
inline void BasePointN< T, 4 >::set( const T2& cppRange ) {
    BS_ASSERT_SIZE( rangeSize( cppRange ), 4 );
    x = cppRange[0];
    y = cppRange[1];
    z = cppRange[2];
    w = cppRange[3];
}

template< typename T >
inline void BasePointN< T, 4 >::set( std::initializer_list< T > il ) {
    BS_ASSERT_SIZE( rangeSize( il ), 4 );
    auto it = std::begin( il );
    x = *( it     );
    y = *( it + 1 );
    z = *( it + 2 );
    w = *( it + 3 );
}

template< typename T >
template< typename T2 >
inline void BasePointN< T, 4 >::get( T2& cppRangeOut ) const {
    BS_ASSERT_SIZE( rangeSize( cppRangeOut ), 4 );
    cppRangeOut[0] = x;
    cppRangeOut[1] = y;
    cppRangeOut[2] = z;
    cppRangeOut[3] = w;
}

template< typename T >
inline void BasePointN< T, 4 >::fill( const T& elem ) {
    x = elem;
    y = elem;
    z = elem;
    w = elem;
}

template< typename T >
BasePointN< T, 4 >::BasePointN()
#ifdef BS_ZERO
    : x( 0 ), y( 0 ), z( 0 ), w( 0 )
#endif //BS_ZERO
{
}

template< typename T >
template< typename T2 >
BasePointN< T, 4 >::BasePointN( const BasePointN< T2, 4 >& toCopy ) :
    x( static_cast<T>( toCopy.x ) ),
    y( static_cast<T>( toCopy.y ) ),
    z( static_cast<T>( toCopy.z ) ),
    w( static_cast<T>( toCopy.w ) ){
}

template< typename T >
void BasePointN< T, 4 >::zero() {
    x = 0;
    y = 0;
    z = 0;
    w = 0;
}

template< typename T >
bool BasePointN< T, 4 >::isZero() const {
    return x == 0 &&
           y == 0 &&
           z == 0 &&
           w == 0;
}

template< typename T >
BasePointN< T, 4 >::BasePointN( const T x, const T y, const T z, const T w ) :
    x( x ), y( y ), z( z ), w( w ) {
}

template< typename T >
void BasePointN< T, 4 >::set( const T x, const T y, const T z, const T w ) {
    BasePointN::x = x;
    BasePointN::y = y;
    BasePointN::z = z;
    BasePointN::w = w;
}

template< typename T >
void BasePointN< T, 4 >::get( T& xOut, T& yOut, T& zOut, T& wOut ) const {
    xOut = x;
    yOut = y;
    zOut = z;
    wOut = w;
}

template< typename T >
template< typename T2 >
BasePointN< T, 4 >& BasePointN< T, 4 >::operator =( const BasePointN< T2, 4 >& right ) {
    x = static_cast< T >( right.x );
    y = static_cast< T >( right.y );
    z = static_cast< T >( right.z );
    w = static_cast< T >( right.w );

    return (*this);
}

}




template< typename T >
class PointN< T, 4 > : public Private::BasePointN< T, 4 > {
public:
    //Specializations of generic methods
    BS_POINT4_DECLARE_INHERITED_METHODS( PointN, BS_SPEC_2( T2, 4 ) )
};
BS_POINT4_DEFINE_INHERITED_METHODS( PointN, BS_TMPL_1( typename T ), BS_SPEC_2( T, 4 ), BS_SPEC_2( T2, 4 ) );




template< typename T >
std::ostream& operator <<( std::ostream& left, const PointN< T, 4 >& right ) {
    return left << "( " << right.x << ", " << right.y << ", " << right.z << ", " << right.w << " )";
}

template< typename T >
bool operator ==( const PointN< T, 4 >& left, const PointN< T, 4 >& right ) {
    return left.x == right.x &&
           left.y == right.y &&
           left.z == right.z &&
           left.w == right.w;
}

template< typename T >
bool operator !=( const PointN< T, 4 >& left, const PointN< T, 4 >& right ) {
    return left.x != right.x ||
           left.y != right.y ||
           left.z != right.z ||
           left.w != right.w;
}

template< typename T >
T distanceSq( const PointN< T, 4 >& left, const PointN< T, 4 >& right ) {
    T xoff = ( right.x - left.x );
    T yoff = ( right.y - left.y );
    T zoff = ( right.z - left.z );
    T woff = ( right.w - left.w );

    return xoff * xoff +
           yoff * yoff +
           zoff * zoff +
           woff * woff;
}

template< typename T >
T manhattan( const PointN< T, 4 >& left, const PointN< T, 4 >& right ) {
    return abs( right.x - left.x ) +
           abs( right.y - left.y ) +
           abs( right.z - left.z ) +
           abs( right.w - left.w );
}




//Typedefs
template< typename T >
using Point4 = PointN< T, 4 >;
typedef Point4< int32  > Point4i;
typedef Point4< float  > Point4f;
typedef Point4< double > Point4d;

}




#endif //BS_POINT_POINT4_HPP