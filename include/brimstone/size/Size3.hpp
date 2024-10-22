/*
size/Size3.hpp
--------------
Copyright (c) 2024, theJ89

Description:
    Specialization of Size for 3D sizes.
    Adds the following aliases for convenience:
        Size3< T >: Size<T,3>
        Size3i:     Size<int32,3>
        Size3f:     Size<float,3>
        Size3d:     Size<double,3>
*/
#ifndef BS_SIZE_SIZE3_HPP
#define BS_SIZE_SIZE3_HPP




//Includes
#include <brimstone/size/SizeN.hpp>     //Brimstone::Size




namespace Brimstone {




template< typename T >
class Size< T, 3 > {
public:
//C4201: nonstandard extension used : nameless struct/union
//It's a non-standard feature, but VC++, G++, and LLVM support it so it shouldn't be too much of an issue
#pragma warning( push )
#pragma warning( disable: 4201 )

    union {
        T data[3];
        //width => x, length => y, height => z
        struct {
            T width, length, height;
        };
        //Shorthand names
        struct {
            T w, l, h;
        };
    };

#pragma warning( pop )
public:
    BS_ARRAY_DECLARE_INHERITED_METHODS( Size, T );
    BS_ARRAY_DECLARE_METHODS( Size, T );
    BS_SIZE_DECLARE_METHODS( 3 );

    Size( const T width, const T length, const T height );
    void set( const T width, const T length, const T height );
    void get( T& widthOut, T& lengthOut, T& heightOut );

    //Miscellaneous utility methods
    T       getVolume() const;
};
BS_ARRAY_DEFINE_METHODS( Size, T, data, BS_TMPL_1( typename T ), BS_SPEC_2( T, 3 ) );




template< typename T >
inline Size< T, 3 >::Size( const T& elem ) :
    width(  elem ),
    length( elem ),
    height( elem ) {
}

template< typename T >
template< typename T2 >
inline Size< T, 3 >::Size( const T2& cppRange ) :
    width(  cppRange[0] ),
    length( cppRange[1] ),
    height( cppRange[2] ) {
    BS_ASSERT_SIZE( rangeSize( cppRange ), 3 );
}

template< typename T >
inline Size< T, 3 >::Size( std::initializer_list< T > il ) :
    width(  *( std::begin( il )     ) ),
    length( *( std::begin( il ) + 1 ) ),
    height( *( std::begin( il ) + 2 ) ) {
    BS_ASSERT_SIZE( rangeSize( il ), 3 );
}

template< typename T >
template< typename T2 >
inline void Size< T, 3 >::set( const T2& cppRange ) {
    BS_ASSERT_SIZE( rangeSize( cppRange ), 3 );
    width  = cppRange[0];
    length = cppRange[1];
    height = cppRange[2];
}

template< typename T >
inline void Size< T, 3 >::set( std::initializer_list< T > il ) {
    BS_ASSERT_SIZE( rangeSize( il ), 3 );
    auto it = std::begin( il );
    width  = *( it     );
    length = *( it + 1 );
    height = *( it + 2 );
}

template< typename T >
template< typename T2 >
inline void Size< T, 3 >::get( T2& cppRangeOut ) const {
    BS_ASSERT_SIZE( rangeSize( cppRangeOut ), 3 );
    cppRangeOut[0] = width;
    cppRangeOut[1] = length;
    cppRangeOut[2] = height;
}

template< typename T >
inline void Size< T, 3 >::fill( const T& elem ) {
    width  = elem;
    length = elem;
    height = elem;
}

template< typename T >
template< typename T2 >
Size< T, 3 >::Size( const Size< T2, 3 >& toCopy ) :
    width(  static_cast<T>( toCopy.width  ) ),
    length( static_cast<T>( toCopy.length ) ),
    height( static_cast<T>( toCopy.height ) ) {
}

template< typename T >
template< typename T2 >
Size< T, 3 >& Size< T, 3 >::operator =( const Size< T2, 3 >& toCopy ) {
    width   = static_cast<T>( toCopy.width   );
    length  = static_cast<T>( toCopy.length  );
    height  = static_cast<T>( toCopy.height  );

    return *this;
}

template< typename T >
void Size< T, 3 >::zero() {
    width  = 0;
    length = 0;
    height = 0;
}

template< typename T >
bool Size< T, 3 >::isZero() const {
    return width  == 0 &&
           length == 0 &&
           height == 0;
}

template< typename T >
Size< T, 3 >::Size()
#ifdef BS_ZERO
    : width( 0 ), length( 0 ), height( 0 )
#endif //BS_ZERO
{
}

template< typename T >
Size< T, 3 >::Size( const T width, const T length, const T height ) :
    width( width ), length( length ), height( height ) {
}

template< typename T >
void Size< T, 3 >::set( const T width, const T length, const T height ) {
    Size::width   = width;
    Size::length  = length;
    Size::height  = height;
}

template< typename T >
void Size< T, 3 >::get( T& widthOut, T& lengthOut, T& heightOut ) {
    widthOut   = width;
    lengthOut  = length;
    heightOut  = height;
}

template< typename T >
T Size< T, 3 >::getVolume() const {
    return width * length * height;
}

template< typename T >
std::ostream& operator <<( std::ostream& left, const Size< T, 3 >& right ) {
    return left << "[ "
                << ( boost::format( "%|.5f|" ) % right.width  ).str() << ", "
                << ( boost::format( "%|.5f|" ) % right.length ).str() << ", "
                << ( boost::format( "%|.5f|" ) % right.height ).str()
                << " ]";
}

template< typename T >
bool operator ==( const Size< T, 3 >& left, const Size< T, 3 >& right ) {
    return left.width  == right.width  &&
           left.length == right.length &&
           left.height == right.height;
}

template< typename T >
bool operator !=( const Size< T, 3 >& left, const Size< T, 3 >& right ) {
    return left.width  != right.width  ||
           left.length != right.length ||
           left.height != right.height;
}




//Types
template< typename T >
using Size3  = Size< T, 3 >;
using Size3i = Size3< int32  >;
using Size3f = Size3< float  >;
using Size3d = Size3< double >;




} //namespace Brimstone




#endif //BS_SIZE_SIZE3_HPP
