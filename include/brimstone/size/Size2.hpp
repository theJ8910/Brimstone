/*
size/Size2.hpp
--------------
Copyright (c) 2024, theJ89

Description:
    Specialization of Size for 2D sizes.
    Adds the following typedefs for convenience:
        Size2< T >: Size<T,2>
        Size2i:     Size<int32,2>
        Size2f:     Size<float,2>
        Size2d:     Size<double,2>
*/
#ifndef BS_SIZE_SIZE2_HPP
#define BS_SIZE_SIZE2_HPP




//Includes
#include <brimstone/size/SizeN.hpp>     //Brimstone::Size




namespace Brimstone {

template< typename T >
class Size< T, 2 > {
public:
//C4201: nonstandard extension used : nameless struct/union
//It's a non-standard feature, but VC++, G++, and LLVM support it so it shouldn't be too much of an issue
#pragma warning( push )
#pragma warning( disable: 4201 )

    union {
        T data[2];
        //width => x, height => y
        struct {
            T width, height;
        };
        //Shorthand names
        struct {
            T w, h;
        };
    };

#pragma warning( pop )
public:
    BS_ARRAY_DECLARE_INHERITED_METHODS( Size, T );
    BS_ARRAY_DECLARE_METHODS( Size, T );
    BS_SIZE_DECLARE_METHODS( 2 );

    Size( const T width, const T height );
    void set( const T width, const T height );
    void get( T& widthOut, T& heightOut );

    //Miscellaneous utility methods
    T       getArea() const;
};
BS_ARRAY_DEFINE_METHODS( Size, T, data, BS_TMPL_1( typename T ), BS_SPEC_2( T, 2 ) );



template< typename T >
inline Size< T, 2 >::Size( const T& elem ) :
    width(   elem ),
    height(  elem ) {
}

template< typename T >
template< typename T2 >
inline Size< T, 2 >::Size( const T2& cppRange ) :
    width(  cppRange[0] ),
    height( cppRange[1] ) {
    BS_ASSERT_SIZE( rangeSize( cppRange ), 2 );
}

template< typename T >
inline Size< T, 2 >::Size( std::initializer_list< T > il ) :
    width(  *( std::begin( il )     ) ),
    height( *( std::begin( il ) + 1 ) ) {
    BS_ASSERT_SIZE( rangeSize( il ), 2 );
}

template< typename T >
template< typename T2 >
inline void Size< T, 2 >::set( const T2& cppRange ) {
    BS_ASSERT_SIZE( rangeSize( cppRange ), 2 );
    width  = cppRange[0];
    height = cppRange[1];
}

template< typename T >
inline void Size< T, 2 >::set( std::initializer_list< T > il ) {
    BS_ASSERT_SIZE( rangeSize( il ), 2 );
    auto it = std::begin( il );
    width  = *( it     );
    height = *( it + 1 );
}

template< typename T >
template< typename T2 >
inline void Size< T, 2 >::get( T2& cppRangeOut ) const {
    BS_ASSERT_SIZE( rangeSize( cppRangeOut ), 2 );
    cppRangeOut[0] = width;
    cppRangeOut[1] = height;
}

template< typename T >
inline void Size< T, 2 >::fill( const T& elem ) {
    width  = elem;
    height = elem;
}

template< typename T >
template< typename T2 >
Size< T, 2 >::Size( const Size< T2, 2 >& toCopy ) :
    width(  static_cast<T>( toCopy.width  ) ),
    height( static_cast<T>( toCopy.height ) ) {
}

template< typename T >
template< typename T2 >
Size< T, 2 >& Size< T, 2 >::operator =( const Size< T2, 2 >& toCopy ) {
    width   = static_cast<T>( toCopy.width   );
    height  = static_cast<T>( toCopy.height  );

    return *this;
}

template< typename T >
void Size< T, 2 >::zero() {
    width  = 0;
    height = 0;
}

template< typename T >
bool Size< T, 2 >::isZero() const {
    return width  == 0 &&
           height == 0;
}

template< typename T >
Size< T, 2 >::Size()
#ifdef BS_ZERO
    : width( 0 ), height( 0 )
#endif //BS_ZERO
{
}

template< typename T >
Size< T, 2 >::Size( const T width, const T height ) :
    width( width ), height( height ) {
}

template< typename T >
void Size< T, 2 >::set( const T width, const T height ) {
    Size::width   = width;
    Size::height  = height;
}

template< typename T >
void Size< T, 2 >::get( T& widthOut, T& heightOut ) {
    widthOut   = width;
    heightOut  = height;
}

template< typename T >
T Size< T, 2 >::getArea() const {
    return width * height;
}

template< typename T >
std::ostream& operator <<( std::ostream& left, const Size< T, 2 >& right ) {
    return left << "[ "
                << ( boost::format( "%|.5f|" ) % right.width  ).str() << ", "
                << ( boost::format( "%|.5f|" ) % right.height ).str()
                << " ]";
}

template< typename T >
bool operator ==( const Size< T, 2 >& left, const Size< T, 2 >& right ) {
    return left.width  == right.width  &&
           left.height == right.height;
}

template< typename T >
bool operator !=( const Size< T, 2 >& left, const Size< T, 2 >& right ) {
    return left.width  != right.width  ||
           left.height != right.height;
}

//Typedefs
template< typename T >
using Size2 = Size< T, 2 >;
typedef Size2< int32  > Size2i;
typedef Size2< float  > Size2f;
typedef Size2< double > Size2d;

}

#endif //BS_SIZE_SIZE2_HPP
