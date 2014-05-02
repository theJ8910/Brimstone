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
#define BS_MAKE_ENUM_HASHER( type ) \
namespace std \
{ \
    template<> \
    struct hash< type > : public Brimstone::EnumHasher< type > {}; \
}

namespace Brimstone {

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
    typedef T                                               Enum_t;
    typedef typename std::underlying_type< Enum_t >::type   Underlying_t;
    typedef typename std::hash< Underlying_t >::result_type Result_t;

    Result_t operator()( const Enum_t& eEnum ) const {
        return std::hash< Underlying_t >()( static_cast< Underlying_t >( eEnum ) );
    }
};

}




#endif //BS_UTIL_MISC_HPP
