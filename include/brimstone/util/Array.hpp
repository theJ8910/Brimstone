/*
util/Array.hpp
--------------
Copyright (c) 2024, theJ89

Description:
    Adds a class for constant-size arrays, Array.
*/
#ifndef BS_UTIL_ARRAY_HPP
#define BS_UTIL_ARRAY_HPP




//Includes
#include <cstddef>                    //std::size_t
#include <algorithm>                  //std::copy, std::fill
#include <iterator>                   //std::begin, std::end
#include <initializer_list>           //std::initializer_list

#include <brimstone/util/Macros.hpp>  //BS_ASSERT_SIZE, BS_ASSERT_INDEX, BS_TMPL_2, BS_SPEC_2
#include <brimstone/util/Misc.hpp>    //rangeSize



//Macros
//Note: The array mixins are implemented as preprocessor macros.
//Macros with "DECLARE" are meant to be used inside of a class definition.
//Macros with "DEFINE" are meant to be used outside of a class definition.
//Gives the class a fill constructor and method that sets every element in the member array to
//the given value.
//Gives the class an initializer list constructor and setter,
//and a constructor, setter, and getter that works with any C++ range.
//Gives the class begin, cbegin, end, and cend methods that iterate over the array member
//Gives the class two overloaded indexing operators that return mutable (or the object is
//const, immutable) elements from the array member.
#define BS_ARRAY_DECLARE_INHERITED_METHODS( className, memberType )                             \
    inline explicit className( const memberType& fill );                                        \
    template< typename T2 >                                                                     \
    inline explicit className( const T2& cppRange );                                            \
    inline explicit className( std::initializer_list< memberType > il );
#define BS_ARRAY_DECLARE_METHODS( className, memberType )                                       \
    template< typename T2 >                                                                     \
    inline void set( const T2& cppRange );                                                      \
    inline void set( std::initializer_list< memberType > il );                                  \
    template< typename T2 >                                                                     \
    inline void get( T2& cppRangeOut ) const;                                                   \
    inline void fill( const memberType& elem );                                                 \
    inline       memberType* begin();                                                           \
    inline const memberType* begin() const;                                                     \
    inline const memberType* cbegin() const;                                                    \
    inline       memberType* end();                                                             \
    inline const memberType* end() const;                                                       \
    inline const memberType* cend() const;                                                      \
    inline       memberType& operator []( const std::size_t index );                            \
    inline const memberType& operator []( const std::size_t index ) const;
#define BS_ARRAY_DEFINE_GENERIC_METHODS( className, memberType, memberName, tmpl, spec )        \
    tmpl                                                                                        \
    inline className spec::className( const memberType& elem ) {                                \
        fill( elem );                                                                           \
    }                                                                                           \
    tmpl                                                                                        \
    template< typename T2 >                                                                     \
    inline className spec::className( const T2& cppRange ) {                                    \
        set( cppRange );                                                                        \
    }                                                                                           \
    tmpl                                                                                        \
    inline className spec::className( std::initializer_list< memberType > il ) {                \
        set( il );                                                                              \
    }                                                                                           \
    tmpl                                                                                        \
    template< typename T2 >                                                                     \
    inline void className spec::set( const T2& cppRange ) {                                     \
        BS_ASSERT_SIZE(                                                                         \
            ::Brimstone::rangeSize( cppRange ),                                                 \
            ::Brimstone::rangeSize( memberName )                                                \
        );                                                                                      \
        std::copy( std::begin( cppRange ), std::end( cppRange ), memberName );                  \
    }                                                                                           \
    tmpl                                                                                        \
    inline void className spec::set( std::initializer_list< memberType > il ) {                 \
        BS_ASSERT_SIZE(                                                                         \
            ::Brimstone::rangeSize( il ),                                                       \
            ::Brimstone::rangeSize( memberName )                                                \
        );                                                                                      \
        std::copy( std::begin( il ), std::end( il ), memberName );                              \
    }                                                                                           \
    tmpl                                                                                        \
    template< typename T2 >                                                                     \
    inline void className spec::get( T2& cppRangeOut ) const {                                  \
        BS_ASSERT_SIZE(                                                                         \
            ::Brimstone::rangeSize( cppRangeOut ),                                              \
            ::Brimstone::rangeSize( memberName )                                                \
        );                                                                                      \
        std::copy( std::begin( memberName ), std::end( memberName ), cppRangeOut );             \
    }                                                                                           \
    tmpl                                                                                        \
    inline void className spec::fill( const memberType& elem ) {                                \
        std::fill( std::begin( memberName ), std::end( memberName ), elem );                    \
    }
#define BS_ARRAY_DEFINE_METHODS( className, memberType, memberName, tmpl, spec )                \
    tmpl                                                                                        \
    inline memberType* className spec::begin() {                                                \
        return std::begin( memberName );                                                        \
    }                                                                                           \
    tmpl                                                                                        \
    inline const memberType* className spec::begin() const {                                    \
        return std::begin( memberName );                                                        \
    }                                                                                           \
    tmpl                                                                                        \
    inline const memberType* className spec::cbegin() const {                                   \
        return std::begin( memberName );                                                        \
    }                                                                                           \
    tmpl                                                                                        \
    inline memberType* className spec::end() {                                                  \
        return std::end( memberName );                                                          \
    }                                                                                           \
    tmpl                                                                                        \
    inline const memberType* className spec::end() const {                                      \
        return std::end( memberName );                                                          \
    }                                                                                           \
    tmpl                                                                                        \
    inline const memberType* className spec::cend() const {                                     \
        return std::end( memberName );                                                          \
    }                                                                                           \
    tmpl                                                                                        \
    inline memberType& className spec::operator []( const std::size_t index ) {                 \
        BS_ASSERT_INDEX( index, ::Brimstone::rangeSize( memberName ) - 1 );                     \
        return memberName[ index ];                                                             \
    }                                                                                           \
    tmpl                                                                                        \
    inline const memberType& className spec::operator []( const std::size_t index ) const {     \
        BS_ASSERT_INDEX( index, ::Brimstone::rangeSize( memberName ) - 1 );                     \
        return memberName[ index ];                                                             \
    }
#define BS_ARRAY_DEFINE_INHERITED_METHODS( className, memberType, parentClassName, tmpl, spec ) \
    tmpl                                                                                        \
    inline className spec::className( const memberType& fill ) :                                \
        parentClassName( fill ) {                                                               \
    }                                                                                           \
    tmpl                                                                                        \
    inline className spec::className( std::initializer_list< memberType > il ) :                \
        parentClassName( il ) {                                                                 \
    }                                                                                           \
    tmpl                                                                                        \
    template< typename T2 >                                                                     \
    inline className spec::className( const T2& cppRange ) :                                    \
        parentClassName( cppRange ) {                                                           \
    }




namespace Brimstone {

//Wrapper class for constant-size arrays
template< typename T, std::size_t N >
class Array {
public:
    T m_data[N];
public:
    BS_ARRAY_DECLARE_INHERITED_METHODS( Array, T )
    BS_ARRAY_DECLARE_METHODS( Array, T )

    inline Array();

    inline std::size_t size() const;
    inline bool        empty() const;
};
BS_ARRAY_DEFINE_GENERIC_METHODS( Array, T, m_data, BS_TMPL_2( typename T, std::size_t N ), BS_SPEC_2( T, N ) )
BS_ARRAY_DEFINE_METHODS( Array, T, m_data, BS_TMPL_2( typename T, std::size_t N ), BS_SPEC_2( T, N ) )

template< typename T, std::size_t N >
inline Array< T, N >::Array() {}

template< typename T, std::size_t N >
inline std::size_t Array< T, N >::size() const {
    return N;
}

template< typename T, std::size_t N >
inline bool Array< T, N >::empty() const {
    return false;
}

}

#endif //BS_UTIL_ARRAY_HPP
