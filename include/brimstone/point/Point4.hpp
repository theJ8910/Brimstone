/*
point/Point4.hpp
-----------------------
Copyright (c) 2014, theJ89

Description:
    Specialization of Point for 4D points.
    Adds the following typedefs for convenience:
        Point4< T >: Point<T,4>
        Point4i:     Point<int32,4>
        Point4f:     Point<float,4>
        Point4d:     Point<double,4>
*/
#ifndef BS_POINT_POINT4_HPP
#define BS_POINT_POINT4_HPP




//Includes
#include <brimstone/point/PointN.hpp>   //Point




//Macros
#define BS_BASEPOINT4_DECLARE_METHODS( className )                                          \
    className( const T x, const T y, const T z, const T w );                                \
    void set( const T x, const T y, const T z, const T w );                                 \
    void get( T& xOut, T& yOut, T& zOut, T& wOut ) const;
#define BS_BASEPOINT4_DEFINE_METHODS( className )                                           \
    template< typename T >                                                                  \
    inline className< T, 4 >::className( const T& elem ) :                                  \
        x( elem ),                                                                          \
        y( elem ),                                                                          \
        z( elem ),                                                                          \
        w( elem ) {                                                                         \
    }                                                                                       \
    template< typename T >                                                                  \
    template< typename T2 >                                                                 \
    inline className< T, 4 >::className( const T2& cppRange ) :                             \
        x( cppRange[0] ),                                                                   \
        y( cppRange[1] ),                                                                   \
        z( cppRange[2] ),                                                                   \
        w( cppRange[3] ) {                                                                  \
        BS_ASSERT_SIZE( rangeSize( cppRange ), 4 );                                         \
    }                                                                                       \
    template< typename T >                                                                  \
    inline className< T, 4 >::className( std::initializer_list< T > il ) :                  \
        x( *( std::begin( il )     ) ),                                                     \
        y( *( std::begin( il ) + 1 ) ),                                                     \
        z( *( std::begin( il ) + 2 ) ),                                                     \
        w( *( std::begin( il ) + 3 ) ) {                                                    \
        BS_ASSERT_SIZE( rangeSize( il ), 4 );                                               \
    }                                                                                       \
    template< typename T >                                                                  \
    template< typename T2 >                                                                 \
    inline void className< T, 4 >::set( const T2& cppRange ) {                              \
        BS_ASSERT_SIZE( rangeSize( cppRange ), 4 );                                         \
        x = cppRange[0];                                                                    \
        y = cppRange[1];                                                                    \
        z = cppRange[2];                                                                    \
        w = cppRange[3];                                                                    \
    }                                                                                       \
    template< typename T >                                                                  \
    inline void className< T, 4 >::set( std::initializer_list< T > il ) {                   \
        BS_ASSERT_SIZE( rangeSize( il ), 4 );                                               \
        auto it = std::begin( il );                                                         \
        x = *( it     );                                                                    \
        y = *( it + 1 );                                                                    \
        z = *( it + 2 );                                                                    \
        w = *( it + 3 );                                                                    \
    }                                                                                       \
    template< typename T >                                                                  \
    template< typename T2 >                                                                 \
    inline void className< T, 4 >::get( T2& cppRangeOut ) const {                           \
        BS_ASSERT_SIZE( rangeSize( cppRangeOut ), 4 );                                      \
        cppRangeOut[0] = x;                                                                 \
        cppRangeOut[1] = y;                                                                 \
        cppRangeOut[2] = z;                                                                 \
        cppRangeOut[3] = w;                                                                 \
    }                                                                                       \
    template< typename T >                                                                  \
    inline void className< T, 4 >::fill( const T& elem ) {                                  \
        x = elem;                                                                           \
        y = elem;                                                                           \
        z = elem;                                                                           \
        w = elem;                                                                           \
    }                                                                                       \
    template< typename T >                                                                  \
    template< typename T2 >                                                                 \
    className< T, 4 >::className( const className< T2, 4 >& toCopy ) :                      \
        x( static_cast<T>( toCopy.x ) ),                                                    \
        y( static_cast<T>( toCopy.y ) ),                                                    \
        z( static_cast<T>( toCopy.z ) ),                                                    \
        w( static_cast<T>( toCopy.w ) ) {                                                   \
    }                                                                                       \
    template< typename T >                                                                  \
    void className< T, 4 >::zero() {                                                        \
        x = 0;                                                                              \
        y = 0;                                                                              \
        z = 0;                                                                              \
        w = 0;                                                                              \
    }                                                                                       \
    template< typename T >                                                                  \
    bool className< T, 4 >::isZero() const {                                                \
        return x == 0 &&                                                                    \
               y == 0 &&                                                                    \
               z == 0 &&                                                                    \
               w == 0;                                                                      \
    }                                                                                       \
    template< typename T >                                                                  \
    className< T, 4 >::className( const T x, const T y, const T z, const T w ) :            \
        x( x ), y( y ), z( z ), w( w ) {                                                    \
    }                                                                                       \
    template< typename T >                                                                  \
    void className< T, 4 >::set( const T x, const T y, const T z, const T w ) {             \
        className::x = x;                                                                   \
        className::y = y;                                                                   \
        className::z = z;                                                                   \
        className::w = w;                                                                   \
    }                                                                                       \
    template< typename T >                                                                  \
    void className< T, 4 >::get( T& xOut, T& yOut, T& zOut, T& wOut ) const {               \
        xOut = x;                                                                           \
        yOut = y;                                                                           \
        zOut = z;                                                                           \
        wOut = w;                                                                           \
    }                                                                                       \
    template< typename T >                                                                  \
    template< typename T2 >                                                                 \
    className< T, 4 >& className< T, 4 >::operator =( const className< T2, 4 >& right ) {   \
        x = static_cast< T >( right.x );                                                    \
        y = static_cast< T >( right.y );                                                    \
        z = static_cast< T >( right.z );                                                    \
        w = static_cast< T >( right.w );                                                    \
                                                                                            \
        return (*this);                                                                     \
    }

namespace Brimstone {




template< typename T >
class Point< T, 4 > {
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
    //Specializations of generic methods
    BS_ARRAY_DECLARE_INHERITED_METHODS( Point, T    )
    BS_ARRAY_DECLARE_METHODS(           Point, T    )
    BS_BASEPOINT_DECLARE_METHODS(       Point,    4 )
    BS_BASEPOINT4_DECLARE_METHODS(      Point       )
    BS_POINT_DECLARE_METHODS(                     4 )
};
BS_ARRAY_DEFINE_METHODS(         Point, T,    data, BS_TMPL_1( typename T ), BS_SPEC_2( T, 4 ) )
BS_BASEPOINT4_DEFINE_METHODS(    Point                                                         )
BS_POINT_DEFINE_METHODS(                   4,       BS_TMPL_1( typename T )                    )




//Forward declarations
template< typename T >
T distanceSq( const Point< T, 4 >& left, const Point< T, 4 >& right );
template< typename T >
T distance( const Point< T, 4 >& left, const Point< T, 4 >& right );
template< typename T >
T manhattan( const Point< T, 4 >& left, const Point< T, 4 >& right );




template< typename T >
Point< T, 4 >::Point()
#ifdef BS_ZERO
    : x( 0 ), y( 0 ), z( 0 ), w( 0 )
#endif //BS_ZERO
{
}

template< typename T >
std::ostream& operator <<( std::ostream& left, const Point< T, 4 >& right ) {
    return left << "( "
                << ( boost::format( "%|.5f|" ) % right.x ).str() << ", "
                << ( boost::format( "%|.5f|" ) % right.y ).str() << ", "
                << ( boost::format( "%|.5f|" ) % right.z ).str() << ", "
                << ( boost::format( "%|.5f|" ) % right.w ).str()
                << " )";
}

template< typename T >
bool operator ==( const Point< T, 4 >& left, const Point< T, 4 >& right ) {
    return left.x == right.x &&
           left.y == right.y &&
           left.z == right.z &&
           left.w == right.w;
}

template< typename T >
bool operator !=( const Point< T, 4 >& left, const Point< T, 4 >& right ) {
    return left.x != right.x ||
           left.y != right.y ||
           left.z != right.z ||
           left.w != right.w;
}

template< typename T >
T distanceSq( const Point< T, 4 >& left, const Point< T, 4 >& right ) {
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
T manhattan( const Point< T, 4 >& left, const Point< T, 4 >& right ) {
    return abs( right.x - left.x ) +
           abs( right.y - left.y ) +
           abs( right.z - left.z ) +
           abs( right.w - left.w );
}




//Typedefs
template< typename T >
using Point4 = Point< T, 4 >;
typedef Point4< int32  > Point4i;
typedef Point4< float  > Point4f;
typedef Point4< double > Point4d;

}




#endif //BS_POINT_POINT4_HPP