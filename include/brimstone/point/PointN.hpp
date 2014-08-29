/*
point/PointN.hpp
-----------------------
Copyright (c) 2014, theJ89

Description:
    Adds a generic point class, Point.
    You can specify what type of data the point uses (int, float, etc) as well as the dimension of the point.
    2D, 3D and 4D specializations of Point are provided in Point2.hpp, Point3.hpp, and Point4.hpp, respectively.
*/
#ifndef BS_POINT_POINTN_HPP
#define BS_POINT_POINTN_HPP




//Includes
#include <iostream>                         //std::ostream
#include <initializer_list>                 //std::initializer_list
#include <algorithm>                        //std::fill
#include <iostream>                         //std::ostream

#include <boost/format.hpp>                 //boost::format

#include <brimstone/util/Array.hpp>         //BS_ARRAY_DECLARE_METHODS(), BS_ARRAY_DEFINE_METHODS()
#include <brimstone/util/Macros.hpp>        //BS_ASSERT_NON_NULLPTR, BS_ASSERT_SIZE, etc
#include <brimstone/util/Math.hpp>          //fastSqrt




//Macros
//Classes that inherit from BasePoint should use these macros.
//They implement constructors and other methods that forward calls back to the base class.
//className is expected to be the name of a class template that takes two parameters:
//  A typename named "T", and a size_t.
//N is supplied for the second parameter of the class template. You can provide the name
//of a template parameter( e.g. N) or a number (2, 3, etc) if used in a specialization.
//spec2 is the specialization that should be used for the parameter in the copy constructor
//and assignment operator.
//It is usually BS_SPEC_2( T2, N ) which translates to < T2, N >.
#define BS_BASEPOINT_DECLARE_INHERITED_METHODS( className, N, spec2 )                   \
    className();                                                                        \
                                                                                        \
    template< typename T2 >                                                             \
    className( const className spec2& toCopy );                                         \
                                                                                        \
    template< typename T2 >                                                             \
    className& operator =( const className spec2& right );
#define BS_BASEPOINT_DEFINE_INHERITED_METHODS( className, N, tmpl, spec, spec2 )        \
    tmpl                                                                                \
    className spec::className() : BaseClass() {}                                        \
                                                                                        \
    tmpl                                                                                \
    template< typename T2 >                                                             \
    className spec::className( const className spec2& toCopy ) :                        \
        BaseClass( static_cast< const Private::BasePoint spec2& >( toCopy ) ) {         \
    }                                                                                   \
                                                                                        \
    tmpl                                                                                \
    template< typename T2 >                                                             \
    className spec& className spec::operator =( const className spec2& right ) {        \
        BaseClass::operator =( right );                                                 \
        return *this;                                                                   \
    }

//Variants of BasePoint use this to redeclare methods common to all specialiations.
#define BS_BASEPOINT_DECLARE_METHODS( N )                                               \
    void zero();                                                                        \
    bool isZero() const;

#define BS_POINT_DECLARE_METHODS( N )                                                   \
    explicit   operator       Vector< T, N >&();                                        \
    explicit   operator const Vector< T, N >&() const;

#define BS_POINT_DEFINE_METHODS( N, tmpl )                                              \
    tmpl                                                                                \
    Point< T, N >::operator Vector< T, N >&() {                                         \
        return reinterpret_cast< Vector< T, N >& >( *this );                            \
    }                                                                                   \
    tmpl                                                                                \
    Point< T, N >::operator const Vector< T, N >&() const {                             \
        return reinterpret_cast< const Vector< T, N >& >( *this );                      \
    }


namespace Brimstone {
namespace Private {

template< typename T, size_t N >
class BasePoint {
public:
    T data[N];
public:
    //Generic methods
    BS_ARRAY_DECLARE_INHERITED_METHODS( BasePoint, T )
    BS_BASEPOINT_DECLARE_INHERITED_METHODS( BasePoint, N, BS_SPEC_2( T2, N ) )
    BS_ARRAY_DECLARE_METHODS( BasePoint, T )
    BS_BASEPOINT_DECLARE_METHODS( N )
};

BS_ARRAY_DEFINE_GENERIC_METHODS( BasePoint, T, data, BS_TMPL_2( typename T, size_t N ), BS_SPEC_2( T, N ) )
BS_ARRAY_DEFINE_METHODS( BasePoint, T, data, BS_TMPL_2( typename T, size_t N ), BS_SPEC_2( T, N ) )

template< typename T, size_t N >
BasePoint< T, N >::BasePoint() {
#ifdef BS_ZERO
    std::fill( std::begin( data ), std::end( data ), static_cast< T >( 0 ) );
#endif //BS_ZERO
}

template< typename T, size_t N >
template< typename T2 >
BasePoint< T, N >::BasePoint( const BasePoint< T2, N >& toCopy ) {
    (*this) = toCopy;
}

template< typename T, size_t N >
template< typename T2 >
BasePoint< T, N >& BasePoint< T, N >::operator =( const BasePoint< T2, N >& right ) {
    for( size_t i = 0; i < N; ++i )
        data[i] = static_cast<T>( right.data[i] );

    return (*this);
}

template< typename T, size_t N >
void BasePoint< T, N >::zero() {
    std::fill( std::begin( data ), std::end( data ), static_cast< T >( 0 ) );
}

template< typename T, size_t N >
bool BasePoint< T, N >::isZero() const {
    for( size_t i = 0; i < N; ++i )
        if( data[i] != 0 )
            return false;
    return true;
}

}




//Forward declarations
template< typename T, size_t N >
class Vector;



template< typename T, size_t N >
class Point : public Private::BasePoint< T, N > {
private:
    typedef Private::BasePoint< T, N > BaseClass;
public:
    BS_ARRAY_DECLARE_INHERITED_METHODS( Point, T )
    BS_BASEPOINT_DECLARE_INHERITED_METHODS( Point, N, BS_SPEC_2( T2, N ) )
    BS_POINT_DECLARE_METHODS( N )
};
BS_ARRAY_DEFINE_INHERITED_METHODS( Point, T, BaseClass, BS_TMPL_2( typename T, size_t N ), BS_SPEC_2( T, N ) )
BS_BASEPOINT_DEFINE_INHERITED_METHODS( Point, N, BS_TMPL_2( typename T, size_t N ), BS_SPEC_2( T, N ), BS_SPEC_2( T2, N ) )
BS_POINT_DEFINE_METHODS( N, BS_TMPL_2( typename T, size_t N ) )

template< typename T, size_t N >
bool operator ==( const Point< T, N >& left, const Point< T, N >& right ) {
    for( size_t i = 0; i < N; ++i )
        if( left.data[i] != right.data[i] )
            return false;
    return true;
}

template< typename T, size_t N >
bool operator !=( const Point< T, N >& left, const Point< T, N >& right ) {
    for( size_t i = 0; i < N; ++i )
        if( left.data[i] != right.data[i] )
            return true;
    return false;
}

template< typename T, size_t N >
std::ostream& operator <<( std::ostream& left, const Point< T, N >& right ) {
    left << "( "
         << ( boost::format( "%|.5f|" ) % right.data[0] ).str();
    for( int i = 1; i < N; ++i )
        left << ", " << ( boost::format( "%|.5f|" ) % right.data[i] ).str();
    left << " )";

    return left;
}

template< typename T, size_t N >
T distanceSq( const Point< T, N >& left, const Point< T, N >& right ) {
    T dist = 0;

    T offset;
    for( size_t i = 0; i < N; ++i ) {
        offset = right.data[i] - left.data[i];
        dist += offset * offset;
    }

    return dist;
}

template< typename T, size_t N >
T distance( const Point< T, N >& left, const Point< T, N >& right ) {
    return (T)fastSqrt( (float)distanceSq( left, right ) );
}

template< typename T, size_t N >
T manhattan( const Point< T, N >& left, const Point< T, N >& right ) {
    T md = 0;

    for( size_t i = 0; i < N; ++i )
        md += abs( right.data[i] - left.data[i] );

    return md;
}

}




#endif //BS_POINT_POINT_HPP