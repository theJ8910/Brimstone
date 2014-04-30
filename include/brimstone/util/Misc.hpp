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
#include <type_traits>      //std::integral_constant




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

}




#endif //BS_UTIL_MISC_HPP