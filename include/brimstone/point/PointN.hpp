/*
point/PointN.hpp
----------------
Copyright (c) 2024, theJ89

Description:
    Adds a generic point class, Point.
    You can specify what type of data the point uses (int, float, etc) as well as the dimension of the point.
    2D, 3D and 4D specializations of Point are provided in Point2.hpp, Point3.hpp, and Point4.hpp, respectively.
*/
#ifndef BS_POINT_POINTN_HPP
#define BS_POINT_POINTN_HPP




//Includes
#include <cstddef>                          //std::size_t
#include <iostream>                         //std::ostream
#include <initializer_list>                 //std::initializer_list
#include <algorithm>                        //std::fill

#include <boost/format.hpp>                 //boost::format

#include <brimstone/util/Array.hpp>         //BS_ARRAY_DECLARE_METHODS(), BS_ARRAY_DEFINE_METHODS()
#include <brimstone/util/Macros.hpp>        //BS_ASSERT_NON_NULLPTR, BS_ASSERT_SIZE, etc
#include <brimstone/util/Math.hpp>          //fastSqrt




//Macros
//Classes that inherit from BasePoint should use these macros.
//They implement constructors and other methods that forward calls back to the base class.
//className is expected to be the name of a class template that takes two parameters:
//  A typename named "T", and an std::size_t.
//N is supplied for the second parameter of the class template. You can provide the name
//of a template parameter( e.g. N) or a number (2, 3, etc) if used in a specialization.
//spec2 is the specialization that should be used for the parameter in the copy constructor
//and assignment operator.
//It is usually BS_SPEC_2( T2, N ) which translates to < T2, N >.

//Classes that base themself off of points define these methods
#define BS_BASEPOINT_DECLARE_METHODS( className, N )                                        \
    className();                                                                            \
                                                                                            \
    template< typename T2 >                                                                 \
    className( const className< T2, N >& toCopy );                                          \
                                                                                            \
    template< typename T2 >                                                                 \
    className& operator =( const className< T2, N >& right );                               \
                                                                                            \
    void zero();                                                                            \
    bool isZero() const;

//Methods for generic case of classes that base themselves off of points
#define BS_BASEPOINTN_DEFINE_METHODS( className )                                           \
    template< typename T, std::size_t N >                                                   \
    template< typename T2 >                                                                 \
    className< T, N >::className( const className< T2, N >& toCopy ) {                      \
        (*this) = toCopy;                                                                   \
    }                                                                                       \
    template< typename T, std::size_t N >                                                   \
    template< typename T2 >                                                                 \
    className< T, N >& className< T, N >::operator =( const className< T2, N >& right ) {   \
        for( std::size_t i = 0; i < N; ++i )                                                \
            data[i] = static_cast<T>( right.data[i] );                                      \
                                                                                            \
        return (*this);                                                                     \
    }                                                                                       \
                                                                                            \
    template< typename T, std::size_t N >                                                   \
    void className< T, N >::zero() {                                                        \
        std::fill( std::begin( data ), std::end( data ), static_cast< T >( 0 ) );           \
    }                                                                                       \
    template< typename T, std::size_t N >                                                   \
    bool className< T, N >::isZero() const {                                                \
        for( std::size_t i = 0; i < N; ++i )                                                \
            if( data[i] != 0 )                                                              \
                return false;                                                               \
        return true;                                                                        \
    }

#define BS_POINT_DECLARE_METHODS( N )                                                       \
    explicit   operator       Vector< T, N >&();                                            \
    explicit   operator const Vector< T, N >&() const;

#define BS_POINT_DEFINE_METHODS( N, tmpl )                                                  \
    tmpl                                                                                    \
    Point< T, N >::operator Vector< T, N >&() {                                             \
        return reinterpret_cast< Vector< T, N >& >( *this );                                \
    }                                                                                       \
    tmpl                                                                                    \
    Point< T, N >::operator const Vector< T, N >&() const {                                 \
        return reinterpret_cast< const Vector< T, N >& >( *this );                          \
    }



namespace Brimstone {




//Forward declarations
template< typename T, std::size_t N >
class Vector;



template< typename T, std::size_t N >
class Point {
public:
    T data[N];
public:
    BS_ARRAY_DECLARE_INHERITED_METHODS( Point, T    )
    BS_ARRAY_DECLARE_METHODS(           Point, T    )
    BS_BASEPOINT_DECLARE_METHODS(       Point,    N )
    BS_POINT_DECLARE_METHODS(                     N )
};
BS_ARRAY_DEFINE_GENERIC_METHODS( Point, T,    data, BS_TMPL_2( typename T, std::size_t N ), BS_SPEC_2( T, N ) )
BS_ARRAY_DEFINE_METHODS(         Point, T,    data, BS_TMPL_2( typename T, std::size_t N ), BS_SPEC_2( T, N ) )
BS_BASEPOINTN_DEFINE_METHODS(    Point                                                                   )
BS_POINT_DEFINE_METHODS(                   N,       BS_TMPL_2( typename T, std::size_t N )                    )




//Forward declarations
template< typename T, std::size_t N >
T distanceSq( const Point< T, N >& left, const Point< T, N >& right );
template< typename T, std::size_t N >
T distance( const Point< T, N >& left, const Point< T, N >& right );
template< typename T, std::size_t N >
T manhattan( const Point< T, N >& left, const Point< T, N >& right );




template< typename T, std::size_t N >
Point< T, N >::Point() {
#ifdef BS_ZERO
    std::fill( std::begin( data ), std::end( data ), static_cast< T >( 0 ) );
#endif //BS_ZERO
}

template< typename T, std::size_t N >
bool operator ==( const Point< T, N >& left, const Point< T, N >& right ) {
    for( std::size_t i = 0; i < N; ++i )
        if( left.data[i] != right.data[i] )
            return false;
    return true;
}

template< typename T, std::size_t N >
bool operator !=( const Point< T, N >& left, const Point< T, N >& right ) {
    for( std::size_t i = 0; i < N; ++i )
        if( left.data[i] != right.data[i] )
            return true;
    return false;
}

template< typename T, std::size_t N >
std::ostream& operator <<( std::ostream& left, const Point< T, N >& right ) {
    left << "( "
         << ( boost::format( "%|.5f|" ) % right.data[0] ).str();
    for( std::size_t i = 1; i < N; ++i )
        left << ", " << ( boost::format( "%|.5f|" ) % right.data[i] ).str();
    left << " )";

    return left;
}

template< typename T, std::size_t N >
T distanceSq( const Point< T, N >& left, const Point< T, N >& right ) {
    T dist = 0;

    T offset;
    for( std::size_t i = 0; i < N; ++i ) {
        offset = right.data[i] - left.data[i];
        dist += offset * offset;
    }

    return dist;
}

template< typename T, std::size_t N >
T distance( const Point< T, N >& left, const Point< T, N >& right ) {
    return (T)fastSqrt( (float)distanceSq( left, right ) );
}

template< typename T, std::size_t N >
T manhattan( const Point< T, N >& left, const Point< T, N >& right ) {
    T md = 0;

    for( std::size_t i = 0; i < N; ++i )
        md += abs( right.data[i] - left.data[i] );

    return md;
}

}




#endif //BS_POINT_POINT_HPP
