/*
util/Macros.hpp
-----------------------

Description:
    This file contains macros that are used by various classes in the engine.
    The behavior of many of these macros can be controlled with switches.
    Typically turning on a feature helps to the debug the program, and turning off a feature improves performance.

    If BS_CHECK_NULLPTR is enabled, then BS_ASSERT_NON_NULLPTR( ptr ) will throw a NullPointerException if ptr == nullptr. Otherwise, nothing happens.
    If BS_CHECK_SIZE is enabled, then BS_ASSERT_SIZE( size, required ) will throw a SizeException if size < required. Otherwise, nothing happens.
*/
#ifndef BS_MACROS_HPP
#define BS_MACROS_HPP




//Includes
#include <brimstone/Exception.hpp>




#ifdef BS_CHECK_NULLPTR

#define BS_ASSERT_NON_NULLPTR( ptr ) \

    if( ptr == nullptr ) \
        throw ::Brimstone::NullPointerException();

#else

#define BS_ASSERT_NON_NULLPTR( ptr )

#endif //BS_CHECK_NULLPTR




#ifdef BS_CHECK_SIZE

#define BS_ASSERT_SIZE( size, required ) \
    if( size < required ) \
        throw ::Brimstone::SizeException();

#else

#define BS_ASSERT_SIZE( size, required )

#endif //BS_CHECK_SIZE




#ifdef BS_CHECK_INDEX

#define BS_ASSERT_INDEX( index, maxIndex ) \
    if( index < 0 || index > maxIndex ) \
        throw ::Brimstone::OutOfBoundsException();

#else

#define BS_ASSERT_INDEX( index, maxIndex )

#endif //BS_CHECK_INDEX




#endif //BS_MACROS_HPP