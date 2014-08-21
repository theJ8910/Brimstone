/*
util/Misc.hpp
-----------------------
Copyright (c) 2014, theJ89

Description:
    This file contains miscellaneous utilities used in the engine.
*/
#ifndef BS_UTIL_MISC_HPP
#define BS_UTIL_MISC_HPP




//Includes
#include <tuple>            //std::tuple
#include <type_traits>      //std::integral_constant, std::underlying_type




//Macros
#define BS_MAKE_ENUM_HASHER( type )                                     \
    namespace std {                                                     \
        template<>                                                      \
        struct hash< type > : public Brimstone::EnumHasher< type > {};  \
    }




namespace Brimstone {

//Returns how many elements are in a C++ range
template< typename T >
inline size_t rangeSize( const T& cppRange ) {
    return static_cast< size_t >( std::end( cppRange ) - std::begin( cppRange ) );
}
template< typename T >
inline size_t rangeSize( std::initializer_list< T > il ) {
    return static_cast< size_t >( std::end( il ) - std::begin( il ) );
}
template< typename T, size_t N >
inline size_t rangeSize( const T (&/* cppRange */)[N] ) {
    return N;
}

//Counts how many elements a given object has.
//Non-tuples have a size of "1".
template< typename T >
struct TupleSize {
    typedef std::integral_constant< int32, 1 > size;
};

//void has a size of 0
template<>
struct TupleSize< void > {
    typedef std::integral_constant< int32, 0 > size;
};

//tuples have a size dependent upon the number of types provided
template< typename... Types >
struct TupleSize< std::tuple< Types... > > {
    typedef std::integral_constant< int32, sizeof...( Types ) > size;
};

//Gets the underlying type of the given enum class, T,
//and delegates the task of hashing a value of T to the std::hash
//defined for its underlying type.
//Original code written by Daniel Frey
//http://stackoverflow.com/questions/18837857/cant-use-enum-class-as-unordered-map-key
template< typename T >
struct EnumHasher {
    typedef T                                               Enum;
    typedef typename std::underlying_type< Enum >::type     Underlying;
    typedef typename std::hash< Underlying >::result_type   Result;

    Result operator()( const Enum& enumr ) const {
        return std::hash< Underlying >()( static_cast< Underlying >( enumr ) );
    }
};

}




#endif //BS_UTIL_MISC_HPP
