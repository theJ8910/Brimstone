/*
PointN.hpp
-----------------------
Copyright (c) 2014, theJ89

Description:
    Adds a generic point class, PointN.
    You can specify what type of data the point uses (int, float, etc) as well as the dimension of the point.
    2D and 3D specializations of PointN are provided in Point2.hpp and Point3.hpp.
*/
#ifndef BS_POINTN_HPP
#define BS_POINTN_HPP




//Includes
#include <iostream>                     //std::ostream
#include <initializer_list>             //std::initializer_list

#include <brimstone/util/Macros.hpp>    //BS_ASSERT_NON_NULLPTR, BS_ASSERT_SIZE, etc
#include <brimstone/util/Math.hpp>      //fastSqrt




//Macros
/*
template< typename T, int N >
PointN< T, N >
*/
#define BS_POINT_THIS_TMPL()                                                \
    template< typename T, int N >

#define BS_POINT_TMPL()                                                     \
    template< typename T >

#define BS_POINT_DECLARE_METHODS( N )                                       \
    PointN();                                                               \
    PointN( std::initializer_list< T > il );                                \
    PointN( const T* const values, const uintN count );                     \
    template< typename T2 >                                                 \
    PointN( const PointN< T2, N >& toCopy );                                \
                                                                            \
    void    set( const T* const values, const uintN count );                \
    void    get( T* const valuesOut, const uintN count ) const;             \
                                                                            \
    void    zero();                                                         \
    bool    isZero() const;                                                 \
                                                                            \
    template< typename T2 >                                                 \
    PointN& operator =( const PointN< T2, N >& right );                     \
                                                                            \
    explicit operator T*();                                                 \
    explicit operator const T*() const;                                     \
                                                                            \
    T&  operator []( const intN component );                                \
    T   operator []( const intN component ) const;

#define BS_POINT_DEFINE_METHODS( N, tmpl )                                  \
    tmpl                                                                    \
    PointN< T, N >::PointN( std::initializer_list< T > il ) {               \
        set( il.begin(), il.size() );                                       \
    }                                                                       \
    tmpl                                                                    \
    PointN< T, N >::PointN( const T* const values, const uintN count ) {    \
        set( values, count );                                               \
    }                                                                       \
    tmpl                                                                    \
    PointN< T, N >::operator T*() {                                         \
        return data;                                                        \
    }                                                                       \
    tmpl                                                                    \
    PointN< T, N >::operator const T*() const {                             \
        return data;                                                        \
    }                                                                       \
    tmpl                                                                    \
    T& PointN< T, N >::operator []( const intN component ) {                \
        BS_ASSERT_INDEX( component, N - 1 );                                \
        return data[ component ];                                           \
    }                                                                       \
    tmpl                                                                    \
    T PointN< T, N >::operator []( const intN component ) const {           \
        BS_ASSERT_INDEX( component, N - 1 );                                \
        return data[ component ];                                           \
    }

namespace Brimstone {




template< typename T, int N >
class PointN {
public:
    T data[N];
public:
    BS_POINT_DECLARE_METHODS( N )
};

BS_POINT_DEFINE_METHODS( N, BS_POINT_THIS_TMPL() )

template< typename T, int N >
PointN< T, N >::PointN() {
#ifdef BS_ZERO
    for( int i = 0; i < N; ++i )
        data[i] = 0;
#endif //BS_ZERO
}

template< typename T, int N >
template< typename T2 >
PointN< T, N >::PointN( const PointN< T2, N >& toCopy ) {
    (*this) = toCopy;
}

template< typename T, int N >
void PointN< T, N >::set( const T* const values, const uintN count ) {
    BS_ASSERT_NON_NULLPTR( values );
    BS_ASSERT_SIZE( count, N );

    for( int i = 0; i < N; ++i )
        data[i] = values[i];
}

template< typename T, int N >
void PointN< T, N >::get( T* const valuesOut, const uintN count ) const {
    BS_ASSERT_NON_NULLPTR( valuesOut );
    BS_ASSERT_SIZE( count, N );

    for( int i = 0; i < N; ++i )
        valuesOut[i] = data[i];
}

template< typename T, int N >
void PointN< T, N >::zero() {
    for( int i = 0; i < N; ++i )
        data[i] = 0;
}

template< typename T, int N >
bool PointN< T, N >::isZero() const {
    for( int i = 0; i < N; ++i )
        if( data[i] != 0 )
            return false;
    return true;
}

template< typename T, int N >
template< typename T2 >
PointN< T, N >& PointN< T, N >::operator =( const PointN< T2, N >& right ) {
    for( int i = 0; i < N; ++i )
        data[i] = (T)right.data[i];

    return (*this);
}

template< typename T, int N >
std::ostream& operator <<( std::ostream& left, const PointN< T, N >& right ) {
    left << "( ";
    
    left << right.data[0];
    for( int i = 1; i < N; ++i )
        left << ", " << right.data[i];

    left << " )";

    return left;
}

template< typename T, int N >
bool operator ==( const PointN< T, N >& left, const PointN< T, N >& right ) {
    for( int i = 0; i < N; ++i )
        if( left.data[i] != right.data[i] )
            return false;
    return true;
}

template< typename T, int N >
bool operator !=( const PointN< T, N >& left, const PointN< T, N >& right ) {
    for( int i = 0; i < N; ++i )
        if( left.data[i] != right.data[i] )
            return true;
    return false;
}

template< typename T, int N >
T distanceSq( const PointN< T, N >& left, const PointN< T, N >& right ) {
    T dist = 0;

    T offset;
    for( int i = 0; i < N; ++i ) {
        offset = right.data[i] - left.data[i];
        dist += offset * offset;
    }

    return dist;
}

template< typename T, int N >
T distance( const PointN< T, N >& left, const PointN< T, N >& right ) {
    return (T)fastSqrt( (float)distanceSq( left, right ) );
}

template< typename T, int N >
T manhattan( const PointN< T, N >& left, const PointN< T, N >& right ) {
    T md = 0;

    for( int i = 0; i < N; ++i )
        md += abs( right.data[i] - left.data[i] );

    return md;
}

}




#endif //BS_POINTN_HPP