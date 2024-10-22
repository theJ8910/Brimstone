/*
util/Range.hpp
--------------
Copyright (c) 2024, theJ89

Description:
    Adds a class for representing a range of values, Range.
    To avoid confusion:
        * A C++ range (sometimes written cppRange) is a type that can be given
          to std::begin() and std::end(), such an an array, std::vector, etc.
          to get a pair of iterators denoting the beginning and end of the values stored
          by that type.
        * A Range (always written with a capital R) is a class introduced by Brimstone in this file.
          Ranges store a pair of iterators, begin and end.
          These iterators do not change for the lifetime of the Range.
          All Ranges are C++ ranges, but not all C++ ranges are Ranges.

    Various classes in Brimstone take C++ ranges as arguments.
    Sometimes you want to pass only part of a C++ range, though.
    In this case, you'd construct a Range for that part and pass it to the function.
    This file defines a helper function, slice, which aids doing this.
*/
#ifndef BS_UTIL_RANGE_HPP
#define BS_UTIL_RANGE_HPP




//Includes
#include <cstddef>                    //std::size_t
#include <initializer_list>           //std::initializer_list
#include <algorithm>                  //std::fill, std::copy
#include <type_traits>                //std::remove_reference

#include <brimstone/util/Macros.hpp>  //BS_ASSERT_NON_NULLPTR, BS_ASSERT_SIZE
#include <brimstone/util/Misc.hpp>    //Brimstone::rangeSize




namespace Brimstone {




template< typename Iter, typename ConstIter >
class Range {
private:
    const Iter m_begin;
    const Iter m_end;
public:
    //Note: MSVC 2013 is buggy. It fails to compile decltype statements that work on other compilers, like G++.
    //Originally, this took the decltype() of *m_begin, but MSVC reports the following error:
    //    error C2171: '*' : illegal on operands of type 'int *const '
    using Ref      = decltype( **static_cast< Iter*      >( nullptr ) );
    using ConstRef = decltype( **static_cast< ConstIter* >( nullptr ) );
    using Value    = typename std::remove_reference< Ref >::type;
public:
    template< typename T >
    inline Range( T& cppRange );
    inline Range( const Iter begin, const Iter end );
    inline Range( const Iter begin, const std::size_t size );

    template< typename T >
    inline void set( const T& cppRange );
    inline void set( std::initializer_list< Value > il );
    template< typename T >
    inline void get( T& cppRangeOut ) const;

    inline Iter        begin();
    inline ConstIter   begin() const;
    inline ConstIter   cbegin() const;
    inline Iter        end();
    inline ConstIter   end() const;
    inline ConstIter   cend() const;

    inline Ref         operator []( const std::size_t index );
    inline ConstRef    operator []( const std::size_t index ) const;

    inline void        fill( ConstRef elem );
    inline std::size_t size() const;
    inline bool        empty() const;
private:
    inline Range& operator =( const Range& right );
};

template< typename Iter, typename ConstIter >
template< typename T >
inline Range< Iter, ConstIter >::Range( T& cppRange ) :
    m_begin( std::begin( cppRange ) ),
    m_end(   std::end(   cppRange ) ) {
}

template< typename Iter, typename ConstIter >
inline Range< Iter, ConstIter >::Range( const Iter begin, const Iter end ) :
    m_begin( begin ),
    m_end(   end   ) {
    BS_ASSERT_NON_NULLPTR( begin );
    BS_ASSERT_NON_NULLPTR( end );
    BS_ASSERT_BOUNDS( begin, end );
}

template< typename Iter, typename ConstIter >
inline Range< Iter, ConstIter >::Range( const Iter begin, const std::size_t size ) :
    m_begin( begin        ),
    m_end(   begin + size ) {
    BS_ASSERT_NON_NULLPTR( begin );
}

template< typename Iter, typename ConstIter >
template< typename T >
inline void Range< Iter, ConstIter >::set( const T& cppRange ) {
    BS_ASSERT_SIZE( rangeSize( cppRange ), size() );
    std::copy( std::begin( cppRange ), std::end( cppRange ), m_begin );
}

template< typename Iter, typename ConstIter >
inline void Range< Iter, ConstIter >::set( std::initializer_list< Value > il ) {
    BS_ASSERT_SIZE( rangeSize( il ), size() );
    std::copy( std::begin( il ), std::end( il ), m_begin );
}

template< typename Iter, typename ConstIter >
template< typename T >
inline void Range< Iter, ConstIter >::get( T& cppRangeOut ) const {
    BS_ASSERT_SIZE( rangeSize( cppRangeOut ), size() );
    std::copy( m_begin, m_end, std::begin( cppRangeOut ) );
}

template< typename Iter, typename ConstIter >
inline Iter Range< Iter, ConstIter >::begin() {
    return m_begin;
}

template< typename Iter, typename ConstIter >
inline ConstIter Range< Iter, ConstIter >::begin() const {
    return m_begin;
}

template< typename Iter, typename ConstIter >
inline ConstIter Range< Iter, ConstIter >::cbegin() const {
    return m_begin;
}

template< typename Iter, typename ConstIter >
inline Iter Range< Iter, ConstIter >::end() {
    return m_end;
}

template< typename Iter, typename ConstIter >
inline ConstIter Range< Iter, ConstIter >::end() const {
    return m_end;
}

template< typename Iter, typename ConstIter >
inline ConstIter Range< Iter, ConstIter >::cend() const {
    return m_end;
}

template< typename Iter, typename ConstIter >
inline auto Range< Iter, ConstIter >::operator []( const std::size_t index ) -> Ref {
    BS_ASSERT_INDEX( index, size() - 1 );

    return m_begin[ index ];
}

template< typename Iter, typename ConstIter >
inline auto Range< Iter, ConstIter >::operator []( const std::size_t index ) const -> ConstRef {
    BS_ASSERT_INDEX( index, size() - 1 );

    return m_begin[ index ];
}

template< typename Iter, typename ConstIter >
inline void Range< Iter, ConstIter >::fill( ConstRef elem ) {
    std::fill( m_begin, m_end, elem );
}

template< typename Iter, typename ConstIter >
inline std::size_t Range< Iter, ConstIter >::size() const {
    return rangeSize( *this );
}

template< typename Iter, typename ConstIter >
inline bool Range< Iter, ConstIter >::empty() const {
    return m_begin == m_end;
}

template< typename Iter, typename ConstIter >
inline Range< Iter, ConstIter >& Range< Iter, ConstIter >::operator =( const Range& right ) {
    //warning C4512: 'Brimstone::Range<int *,const int *>' : assignment operator could not be generated
    //Solved by implementing a private, do-nothing operator =.
}




} //namespace Brimstone




namespace Brimstone::Private {




//Given a C++ range of type T, defines an alias for a Range that iterates over that container.
template< typename T >
struct RangeFromCpp {
    //Note: Objects of this struct are never instantiated.
    //These are defined here for use in the decltype below.
    T m_instance;
    const T m_constInstance;

    using type = Range<
        decltype( std::begin( m_instance      ) ),
        decltype( std::begin( m_constInstance ) )
    >;
};




} //namespace Brimstone::Private




namespace Brimstone {




//slice{1}
//Returns a Range of values from a C++ range between two indices, [begin, end).
//Usage example:
//    int myArray[5] = { 1, 2, 3, 4, 5 };
//    Vector3i v( slice( myArray, 1, 4 ) );
//NOTE: "end" is exclusive, so the range returned by slice
//in the example above will cover indices 1, 2, and 3.
template< typename T >
inline typename Private::RangeFromCpp<T>::type slice( T& cppRange, const std::size_t begin, const std::size_t end ) {
    BS_ASSERT_INDEX( begin, rangeSize( cppRange ) - 1 );
    BS_ASSERT_INDEX( end,   rangeSize( cppRange ) );

    return typename Private::RangeFromCpp< T >::type(
        std::begin( cppRange ) + begin,
        std::begin( cppRange ) + end
    );
}

//slice{2}
//Same as slice{1}, but only takes a single index, begin.
//Returns a Range of values with indices greater than or equal to begin.
//e.g. if an cppRange is int myArray[] = { 0, 1, 2, 3, 4 },
//then slice( myArray, 2 ) would return a Range covering { 2, 3, 4 }.
template< typename T >
inline typename Private::RangeFromCpp<T>::type slice( T& cppRange, const std::size_t begin ) {
    std::size_t size = rangeSize( cppRange );
    BS_ASSERT_INDEX( begin, size - 1 );

    return typename Private::RangeFromCpp< T >::type(
        std::begin( cppRange ) + begin,
        std::begin( cppRange ) + size
    );
}




} //namespace Brimstone




#endif //BS_UTIL_RANGE_HPP
