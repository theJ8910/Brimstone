/*
size/Size4.hpp
-----------------------
Copyright (c) 2014, theJ89

Description:
    Specialization of Size for 4D sizes.
    Adds the following typedefs for convenience:
        Size4< T >: Size<T,4>
        Size4i:     Size<int32,4>
        Size4f:     Size<float,4>
        Size4d:     Size<double,4>
*/
#ifndef BS_SIZE_SIZE4_HPP
#define BS_SIZE_SIZE4_HPP




//Includes
#include <brimstone/size/SizeN.hpp>     //Brimstone::Size




namespace Brimstone {

template< typename T >
class Size< T, 4 > {
public:
//C4201: nonstandard extension used : nameless struct/union
//It's a non-standard feature, but VC++, G++, and LLVM support it so it shouldn't be too much of an issue
#pragma warning( push )
#pragma warning( disable: 4201 )

    union {
        T data[4];
        //width => x, length => y, height => z, depth => w
        struct {
            T width, length, height, depth;
        };
        //Shorthand names
        struct {
            T w, l, h, d;
        };
    };

#pragma warning( pop )
public:
    BS_ARRAY_DECLARE_INHERITED_METHODS( Size, T );
    BS_ARRAY_DECLARE_METHODS( Size, T );
    BS_SIZE_DECLARE_METHODS( 4 );

    Size( const T width, const T length, const T height, const T depth );
    void set( const T width, const T length, const T height, const T depth );
    void get( T& widthOut, T& lengthOut, T& heightOut, T& depthOut );

    //Miscellaneous utility methods
    T       getVolume() const;
};
BS_ARRAY_DEFINE_METHODS( Size, T, data, BS_TMPL_1( typename T ), BS_SPEC_2( T, 4 ) );




template< typename T >
inline Size< T, 4 >::Size( const T& elem ) :
    width(  elem ),
    length( elem ),
    height( elem ),
    depth(  elem ) {
}

template< typename T >
template< typename T2 >
inline Size< T, 4 >::Size( const T2& cppRange ) :
    width(  cppRange[0] ),
    length( cppRange[1] ),
    height( cppRange[2] ),
    depth(  cppRange[3] ) {
    BS_ASSERT_SIZE( rangeSize( cppRange ), 4 );
}

template< typename T >
inline Size< T, 4 >::Size( std::initializer_list< T > il ) :
    width(  *( std::begin( il )     ) ),
    length( *( std::begin( il ) + 1 ) ),
    height( *( std::begin( il ) + 2 ) ),
    depth(  *( std::begin( il ) + 3 ) ) {
    BS_ASSERT_SIZE( rangeSize( il ), 4 );
}

template< typename T >
template< typename T2 >
inline void Size< T, 4 >::set( const T2& cppRange ) {
    BS_ASSERT_SIZE( rangeSize( cppRange ), 4 );
    width  = cppRange[0];
    length = cppRange[1];
    height = cppRange[2];
    depth  = cppRange[3];
}

template< typename T >
inline void Size< T, 4 >::set( std::initializer_list< T > il ) {
    BS_ASSERT_SIZE( rangeSize( il ), 4 );
    auto it = std::begin( il );
    width  = *( it     );
    length = *( it + 1 );
    height = *( it + 2 );
    depth  = *( it + 3 );
}

template< typename T >
template< typename T2 >
inline void Size< T, 4 >::get( T2& cppRangeOut ) const {
    BS_ASSERT_SIZE( rangeSize( cppRangeOut ), 4 );
    cppRangeOut[0] = width;
    cppRangeOut[1] = length;
    cppRangeOut[2] = height;
    cppRangeOut[3] = depth;
}

template< typename T >
inline void Size< T, 4 >::fill( const T& elem ) {
    width  = elem;
    length = elem;
    height = elem;
    depth  = elem;
}

template< typename T >
template< typename T2 >
Size< T, 4 >::Size( const Size< T2, 4 >& toCopy ) :
    width(  static_cast<T>( toCopy.width   ) ),
    length( static_cast<T>( toCopy.length  ) ),
    height( static_cast<T>( toCopy.height  ) ),
    depth(  static_cast<T>( toCopy.depth   ) ) {
}

template< typename T >
template< typename T2 >
Size< T, 4 >& Size< T, 4 >::operator =( const Size< T2, 4 >& toCopy ) {
    width  = static_cast<T>( toCopy.width   );
    length = static_cast<T>( toCopy.length  );
    height = static_cast<T>( toCopy.height  );
    depth  = static_cast<T>( toCopy.depth   );

    return *this;
}

template< typename T >
void Size< T, 4 >::zero() {
    width  = 0;
    length = 0;
    height = 0;
    depth  = 0;
}

template< typename T >
bool Size< T, 4 >::isZero() const {
    return width  == 0 &&
           length == 0 &&
           height == 0 &&
           depth  == 0;
}

template< typename T >
Size< T, 4 >::Size()
#ifdef BS_ZERO
    : width( 0 ), length( 0 ), height( 0 ), depth( 0 )
#endif //BS_ZERO
{
}

template< typename T >
Size< T, 4 >::Size( const T width, const T length, const T height, const T depth ) :
    width( width ), length( length ), height( height ), depth( depth ) {
}

template< typename T >
void Size< T, 4 >::set( const T width, const T length, const T height, const T depth ) {
    Size::width  = width;
    Size::length = length;
    Size::height = height;
    Size::depth  = depth;
}

template< typename T >
void Size< T, 4 >::get( T& widthOut, T& lengthOut, T& heightOut, T& depthOut ) {
    widthOut  = width;
    lengthOut = length;
    heightOut = height;
    depthOut  = depth;
}

template< typename T >
T Size< T, 4 >::getVolume() const {
    return width * length * height * depth;
}

template< typename T >
std::ostream& operator <<( std::ostream& left, const Size< T, 4 >& right ) {
    return left << "[ "
                << ( boost::format( "%|.5f|" ) % right.width  ).str() << ", "
                << ( boost::format( "%|.5f|" ) % right.length ).str() << ", "
                << ( boost::format( "%|.5f|" ) % right.height ).str() << ", "
                << ( boost::format( "%|.5f|" ) % right.depth  ).str()
                << " ]";
}

template< typename T >
bool operator ==( const Size< T, 4 >& left, const Size< T, 4 >& right ) {
    return left.width  == right.width   &&
           left.length == right.length  &&
           left.height == right.height  &&
           left.depth  == right.depth;
}

template< typename T >
bool operator !=( const Size< T, 4 >& left, const Size< T, 4 >& right ) {
    return left.width  != right.width   ||
           left.length != right.length  ||
           left.height != right.height  ||
           left.depth  != right.depth;
}

//Typedefs
template< typename T >
using Size4 = Size< T, 4 >;
typedef Size4< int32  > Size4i;
typedef Size4< float  > Size4f;
typedef Size4< double > Size4d;

}

#endif //BS_SIZE_SIZE4_HPP