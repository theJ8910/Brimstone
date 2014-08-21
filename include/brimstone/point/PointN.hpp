/*
PointN.hpp
-----------------------
Copyright (c) 2014, theJ89

Description:
    Adds a generic point class, PointN.
    You can specify what type of data the point uses (int, float, etc) as well as the dimension of the point.
    2D and 3D specializations of PointN are provided in Point2.hpp and Point3.hpp.
*/
#ifndef BS_POINT_POINTN_HPP
#define BS_POINT_POINTN_HPP




//Includes
#include <iostream>                         //std::ostream
#include <initializer_list>                 //std::initializer_list
#include <algorithm>                        //std::fill

#include <brimstone/util/Array.hpp>         //BS_ARRAY_DECLARE_METHODS(), BS_ARRAY_DEFINE_METHODS()
#include <brimstone/util/Macros.hpp>        //BS_ASSERT_NON_NULLPTR, BS_ASSERT_SIZE, etc
#include <brimstone/util/Math.hpp>          //fastSqrt




//Macros
//Classes that inherit from BasePointN should use these macros.
//They implement constructors and other methods that forward calls back to the base class.
//className is expected to be the name of a class template that takes two parameters:
//  A typename named "T", and a size_t.
//N is supplied for the second parameter of the class template. You can provide the name
//of a template parameter( e.g. N) or a number (2, 3, etc) if used in a specialization.
//spec2 is the specialization that should be used for the parameter in the copy constructor
//and assignment operator.
//It is usually BS_SPEC_2( T2, N ) which translates to < T2, N >.
#define BS_POINT_DECLARE_INHERITED_METHODS( className, N, spec2 )                       \
    BS_ARRAY_DECLARE_INHERITED_METHODS( className, T )                                  \
    className();                                                                        \
                                                                                        \
    template< typename T2 >                                                             \
    className( const className spec2& toCopy );                                         \
                                                                                        \
    template< typename T2 >                                                             \
    className& operator =( const className spec2& right );
#define BS_POINT_DEFINE_INHERITED_METHODS( className, N, tmpl, spec, spec2 )            \
    BS_ARRAY_DEFINE_INHERITED_METHODS( className, T, BasePointN, tmpl, spec )           \
    tmpl                                                                                \
    className spec::className() : BasePointN() {}                                       \
                                                                                        \
    tmpl                                                                                \
    template< typename T2 >                                                             \
    className spec::className( const className spec2& toCopy ) :                        \
        BasePointN( static_cast< const BasePointN spec2& >( toCopy ) ) {                \
    }                                                                                   \
                                                                                        \
    tmpl                                                                                \
    template< typename T2 >                                                             \
    className spec& className spec::operator =( const className spec2& right ) {        \
        BasePointN::operator =( right );                                                \
        return *this;                                                                   \
    }

//Variants of BasePointN use this to redeclare methods common to all specialiations.
#define BS_POINT_DECLARE_METHODS( N )                                                   \
    BS_ARRAY_DECLARE_METHODS( BasePointN, T )                                           \
    void zero();                                                                        \
    bool isZero() const;




namespace Brimstone {
namespace Private {

template< typename T, size_t N >
class BasePointN {
public:
    T data[N];
public:
    //Generic methods
    BS_POINT_DECLARE_INHERITED_METHODS( BasePointN, N, BS_SPEC_2( T2, N ) )
    BS_POINT_DECLARE_METHODS( N )
};

BS_ARRAY_DEFINE_GENERIC_METHODS( BasePointN, T, data, BS_TMPL_2( typename T, size_t N ), BS_SPEC_2( T, N ) )
BS_ARRAY_DEFINE_METHODS( BasePointN, T, data, BS_TMPL_2( typename T, size_t N ), BS_SPEC_2( T, N ) )

template< typename T, size_t N >
BasePointN< T, N >::BasePointN() {
#ifdef BS_ZERO
    std::fill( std::begin( data ), std::end( data ), static_cast< T >( 0 ) );
#endif //BS_ZERO
}

template< typename T, size_t N >
template< typename T2 >
BasePointN< T, N >::BasePointN( const BasePointN< T2, N >& toCopy ) {
    (*this) = toCopy;
}

template< typename T, size_t N >
template< typename T2 >
BasePointN< T, N >& BasePointN< T, N >::operator =( const BasePointN< T2, N >& right ) {
    for( size_t i = 0; i < N; ++i )
        data[i] = static_cast<T>( right.data[i] );

    return (*this);
}

template< typename T, size_t N >
void BasePointN< T, N >::zero() {
    std::fill( std::begin( data ), std::end( data ), static_cast< T >( 0 ) );
}

template< typename T, size_t N >
bool BasePointN< T, N >::isZero() const {
    for( size_t i = 0; i < N; ++i )
        if( data[i] != 0 )
            return false;
    return true;
}

}




template< typename T, size_t N >
class PointN : public Private::BasePointN< T, N > {
public:
    BS_POINT_DECLARE_INHERITED_METHODS( PointN, N, BS_SPEC_2( T2, N ) )
};

BS_POINT_DEFINE_INHERITED_METHODS( PointN, N, BS_TMPL_2( typename T, size_t N ), BS_SPEC_2( T, N ), BS_SPEC_2( T2, N ) )

template< typename T, size_t N >
bool operator ==( const PointN< T, N >& left, const PointN< T, N >& right ) {
    for( size_t i = 0; i < N; ++i )
        if( left.data[i] != right.data[i] )
            return false;
    return true;
}

template< typename T, size_t N >
bool operator !=( const PointN< T, N >& left, const PointN< T, N >& right ) {
    for( size_t i = 0; i < N; ++i )
        if( left.data[i] != right.data[i] )
            return true;
    return false;
}

template< typename T, size_t N >
std::ostream& operator <<( std::ostream& left, const PointN< T, N >& right ) {
    left << "( ";

    left << right.data[0];
    for( size_t i = 1; i < N; ++i )
        left << ", " << right.data[i];

    left << " )";

    return left;
}

template< typename T, size_t N >
T distanceSq( const PointN< T, N >& left, const PointN< T, N >& right ) {
    T dist = 0;

    T offset;
    for( size_t i = 0; i < N; ++i ) {
        offset = right.data[i] - left.data[i];
        dist += offset * offset;
    }

    return dist;
}

template< typename T, size_t N >
T distance( const PointN< T, N >& left, const PointN< T, N >& right ) {
    return (T)fastSqrt( (float)distanceSq( left, right ) );
}

template< typename T, size_t N >
T manhattan( const PointN< T, N >& left, const PointN< T, N >& right ) {
    T md = 0;

    for( size_t i = 0; i < N; ++i )
        md += abs( right.data[i] - left.data[i] );

    return md;
}

}




#endif //BS_POINT_POINTN_HPP