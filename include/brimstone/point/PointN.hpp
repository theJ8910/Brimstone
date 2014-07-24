﻿/*
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




//Macros
/*
template< typename T, int N >
PointN< T, N >
*/
#define BS_POINT_THIS_TMPL() \
    template< typename T, int N >

#define BS_POINT_TMPL() \
    template< typename T >

#define BS_POINT_DECLARE_METHODS() \
    PointN( std::initializer_list< T > il ); \
    PointN( const T* const values, const uintN count );\
    \
    operator T*(); \
    operator const T*() const; \
    \
    T&  operator []( const intN component ); \
    T   operator []( const intN component ) const;

#define BS_POINT_DEFINE_METHODS( N, tmpl ) \
    tmpl \
    PointN<T,N>::PointN( std::initializer_list< T > il ) { \
        set( il.begin(), il.size() ); \
    } \
    tmpl \
    PointN<T,N>::PointN( const T* const values, const uintN count ) { \
        set( values, count ); \
    } \
    tmpl \
    PointN<T,N>::operator T*() { \
        return data; \
    } \
    tmpl \
    PointN<T,N>::operator const T*() const { \
        return data; \
    } \
    tmpl \
    T& PointN<T,N>::operator []( const intN component ) { \
        BS_ASSERT_INDEX( component, N - 1 ) \
        return data[ component ]; \
    } \
    tmpl \
    T PointN<T,N>::operator []( const intN component ) const { \
        BS_ASSERT_INDEX( component, N - 1 ) \
        return data[ component ]; \
    }

namespace Brimstone {




template< typename T, int N >
class PointN {
public:
    T data[N];
public:
    BS_POINT_DECLARE_METHODS()

    //Constructors
    PointN();

    //Set / get the coordinates
    void    set( const T* const values, const uintN count );
    void    get( T* const valuesOut, const uintN count ) const;

    //Miscellaneous utility methods
    void    zero();
    bool    isZero() const;

    //Related free functions
    template< typename T2, int N2 >
    friend  std::ostream&   operator <<( std::ostream& left, const PointN< T2, N2 >& right );

    template< typename T2, int N2 >
    friend  bool            operator ==( const PointN< T2, N2 >& left, const PointN< T2, N2 >& right );
    template< typename T2, int N2 >
    friend  bool            operator !=( const PointN< T2, N2 >& left, const PointN< T2, N2 >& right );
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
void PointN< T, N >::set( const T* const values, const uintN count ) {
    BS_ASSERT_NON_NULLPTR( values );
    BS_ASSERT_SIZE( count, N );

    for( int i = 0; i < N; ++i )
        data[i] = values[i];
}

template< typename T, int N >
void PointN< T, N >::get( T* const valuesOut, const uintN count ) const {
    BS_ASSERT_NON_NULLPTR( values );
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
std::ostream& operator <<( std::ostream& left, const PointN< T, N >& right ) {
    left << "( ";
    
    left << right[0];
    for( int i = 1; i < N; ++i )
        left << ", " << right[i];

    left << " )";

    return left;
}

template< typename T, int N >
bool operator ==( const PointN< T, N >& left, PointN< T, N >& right ) {
    for( int i = 0; i < N; ++i )
        if( left[i] != right[i] )
            return false;
    return true;
}

template< typename T, int N >
bool operator !=( const PointN< T, N >& left, const PointN< T, N >& right ) {
    for( int i = 0; i < N; ++i )
        if( left[i] == right[i] )
            return true;
    return false;
}

}




#endif //BS_POINTN_HPP