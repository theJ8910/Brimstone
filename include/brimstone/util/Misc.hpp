/*
util/Misc.hpp
-------------
Copyright (c) 2024, theJ89

Description:
    This file contains miscellaneous utilities used in the engine.
*/
#ifndef BS_UTIL_MISC_HPP
#define BS_UTIL_MISC_HPP




//Includes
#include <cstddef>              //std::size_t
#include <tuple>                //std::tuple
#include <type_traits>          //std::integral_constant, std::underlying_type

#include <brimstone/types.hpp>  //Brimstone::int32, Brimstone::ustring




//Macros
#define BS_MAKE_ENUM_HASHER( type )                                     \
    namespace std {                                                     \
        template<>                                                      \
        struct hash< type > : public Brimstone::EnumHasher< type > {};  \
    }




namespace Brimstone {




//Forward declarations
ustring getExecutablePath();




//Returns how many elements are in a C++ range
template< typename T >
inline std::size_t rangeSize( const T& cppRange ) {
    return static_cast< std::size_t >( std::end( cppRange ) - std::begin( cppRange ) );
}
template< typename T >
inline std::size_t rangeSize( std::initializer_list< T > il ) {
    return static_cast< std::size_t >( std::end( il ) - std::begin( il ) );
}
template< typename T, std::size_t N >
inline std::size_t rangeSize( const T (&/* cppRange */)[N] ) {
    return N;
}

//Counts how many elements a given object has.
//Non-tuples have a size of "1".
template< typename T >
struct TupleSize {
    using size = std::integral_constant< int32, 1 >;
};

//void has a size of 0
template<>
struct TupleSize< void > {
    using size = std::integral_constant< int32, 0 >;
};

//tuples have a size dependent upon the number of types provided
template< typename... Types >
struct TupleSize< std::tuple< Types... > > {
    using size = std::integral_constant< int32, sizeof...( Types ) >;
};

//Gets the underlying type of the given enum class, T,
//and delegates the task of hashing a value of T to the std::hash
//defined for its underlying type.
//Original code written by Daniel Frey
//http://stackoverflow.com/questions/18837857/cant-use-enum-class-as-unordered-map-key
template< typename T >
struct EnumHasher {
    using Enum       = T;
    using Underlying = typename std::underlying_type< Enum >::type;
    using Result     = typename std::hash< Underlying >::result_type;

    Result operator()( const Enum& enumr ) const {
        return std::hash< Underlying >()( static_cast< Underlying >( enumr ) );
    }
};




} //namespace Brimstone




#endif //BS_UTIL_MISC_HPP
