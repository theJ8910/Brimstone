/*
point/PointN.hpp
-----------------------
Copyright (c) 2014, theJ89

Description:
    Adds a generic point class, Point.
    You can specify what type of data the point uses (int, float, etc) as well as the dimension of the point.
    2D, 3D and 4D specializations of Point are provided in Point2.hpp, Point3.hpp, and Point4.hpp, respectively.
*/
#ifndef BS_SIZE_SIZEN_HPP
#define BS_SIZE_SIZEN_HPP




//Includes
#include <iostream>                     //std::ostream
#include <algorithm>                    //std::fill
#include <brimstone/util/Array.hpp>     //BS_ARRAY_DECLARE_METHODS(), BS_ARRAY_DEFINE_METHODS()

#include <boost/format.hpp>             //boost::format




//Defines
#define BS_SIZE_DECLARE_METHODS( N )                    \
    Size();                                             \
                                                        \
    template< typename T2 >                             \
    Size( const Size< T2, N >& toCopy );                \
                                                        \
    template< typename T2 >                             \
    Size& operator =( const Size< T2, N >& toCopy );    \
                                                        \
    void zero();                                        \
    bool isZero() const;

namespace Brimstone {

template< typename T, size_t N >
class Size {
public:
    union {
        T data[N];
    };
public:
    BS_ARRAY_DECLARE_INHERITED_METHODS( Size, T );
    BS_ARRAY_DECLARE_METHODS( Size, T );
    BS_SIZE_DECLARE_METHODS( N );

    T getVolume() const;
};

BS_ARRAY_DEFINE_METHODS(         Size, T,    data, BS_TMPL_2( typename T, size_t N ), BS_SPEC_2( T, N ) );
BS_ARRAY_DEFINE_GENERIC_METHODS( Size, T,    data, BS_TMPL_2( typename T, size_t N ), BS_SPEC_2( T, N ) );




template< typename T, size_t N >
Size< T, N >::Size() {
#ifdef BS_ZERO
    std::fill( std::begin( data ), std::end( data ), static_cast< T >( 0 ) );
#endif //BS_ZERO
}

template< typename T, size_t N >
template< typename T2 >
Size< T, N >::Size( const Size< T2, N >& toCopy ) {
    (*this) = toCopy;
}

template< typename T, size_t N >
template< typename T2 >
Size< T, N >& Size< T, N >::operator =( const Size< T2, N >& toCopy ) {
    for( size_t i = 0; i < N; ++i )
        data[i] = static_cast<T>( toCopy.data[i] );

    return *this;
}

template< typename T, size_t N >
void Size< T, N >::zero() {
    std::fill( std::begin( data ), std::end( data ), static_cast< T >( 0 ) );
}

template< typename T, size_t N >
bool Size< T, N >::isZero() const {
    for( size_t i = 0; i < N; ++i )
        if( data[i] != 0 )
            return false;
    return true;
}

template< typename T, size_t N >
T Size< T, N >::getVolume() const {
    T volume = data[0];

    for( size_t i = 1; i < N; ++i )
        volume *= data[i];

    return volume;
}

template< typename T, size_t N >
bool operator ==( const Size< T, N >& left, const Size< T, N >& right ) {
    for( size_t i = 0; i < N; ++i )
        if( left.data[i] != right.data[i] )
            return false;
    return true;
}

template< typename T, size_t N >
bool operator !=( const Size< T, N >& left, const Size< T, N >& right ) {
    for( size_t i = 0; i < N; ++i )
        if( left.data[i] != right.data[i] )
            return true;
    return false;
}

template< typename T, size_t N >
std::ostream& operator <<( std::ostream& left, const Size< T, N >& right ) {
    left << "[ "
         << ( boost::format( "%|.5f|" ) % right.data[0] ).str();
    for( size_t i = 1; i < N; ++i )
        left << ", " << ( boost::format( "%|.5f|" ) % right.data[i] ).str();
    left << " ]";

    return left;
}

}

#endif //BS_SIZE_SIZEN_HPP