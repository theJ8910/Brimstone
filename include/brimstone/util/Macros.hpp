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
#ifndef BS_UTIL_MACROS_HPP
#define BS_UTIL_MACROS_HPP




//Includes
#include <brimstone/Exception.hpp>




//Macros to generate "template< param1, param2, ..., param5 > for use in other macros
#define BS_TMPL_1( param1                                 ) template< param1                                 >
#define BS_TMPL_2( param1, param2                         ) template< param1, param2                         >
#define BS_TMPL_3( param1, param2, param3                 ) template< param1, param2, param3                 >
#define BS_TMPL_4( param1, param2, param3, param4         ) template< param1, param2, param3, param4         >
#define BS_TMPL_5( param1, param2, param3, param4, param5 ) template< param1, param2, param3, param4, param5 >

//Macros to generate "< arg1, arg2, ..., arg5 > for use in other macros
#define BS_SPEC_1( arg1                                   ) < arg1                                           >
#define BS_SPEC_2( arg1, arg2                             ) < arg1, arg2                                     >
#define BS_SPEC_3( arg1, arg2, arg3                       ) < arg1, arg2, arg3                               >
#define BS_SPEC_4( arg1, arg2, arg3, arg4                 ) < arg1, arg2, arg3, arg4                         >
#define BS_SPEC_5( arg1, arg2, arg3, arg4, arg5           ) < arg1, arg2, arg3, arg4, arg5                   >




#ifdef BS_CHECK_NULLPTR

#define BS_ASSERT_NON_NULLPTR( ptr ) \
    if( ptr == nullptr ) \
        throw ::Brimstone::NullPointerException();

#else  //BS_CHECK_NULLPTR

#define BS_ASSERT_NON_NULLPTR( ptr )

#endif //BS_CHECK_NULLPTR




#ifdef BS_CHECK_SIZE

#define BS_ASSERT_SIZE( size, required )        \
    if( size < required )                       \
        throw ::Brimstone::SizeException();

#else  //BS_CHECK_SIZE

#define BS_ASSERT_SIZE( size, required )

#endif //BS_CHECK_SIZE




#ifdef BS_CHECK_INDEX

#define BS_ASSERT_INDEX( index, maxIndex )      \
    if( index < 0 || index > maxIndex )         \
        throw ::Brimstone::BoundsException();

#define BS_ASSERT_BOUNDS( min, max )            \
    if( min > max )                             \
        throw ::Brimstone::BoundsException();

#else  //BS_CHECK_INDEX

#define BS_ASSERT_INDEX( index, maxIndex )

#endif //BS_CHECK_INDEX




#ifdef BS_CHECK_DIVBYZERO

#define BS_ASSERT_NONZERO_DIVISOR( divisor )    \
    if( divisor == 0 )                          \
        throw ::Brimstone::DivideByZeroException();

#define BS_ASSERT_CAN_NORMALIZE( target )       \
    if( (target).isZero() )                     \
        throw ::Brimstone::DivideByZeroException();

#else //BS_CHECK_DIVBYZERO

#define BS_ASSERT_NONZERO_DIVISOR( divisor )

#define BS_ASSERT_CAN_NORMALIZE( target )

#endif //BS_CHECK_DIVBYZERO




#ifdef BS_CHECK_DOMAIN

#define BS_ASSERT_DOMAIN( condition )           \
    if( !( condition ) )                        \
        throw ::Brimstone::DomainException();

#define BS_ASSERT_DOMAIN_GTE( value, min )      \
    if( value < min )                           \
        throw ::Brimstone::DomainException();

#define BS_ASSERT_DOMAIN_GT( value, min )       \
    if( value <= min )                          \
        throw ::Brimstone::DomainException();

#define BS_ASSERT_DOMAIN_LTE( value, max )      \
    if( value > max )                           \
        throw ::Brimstone::DomainException();

#define BS_ASSERT_DOMAIN_LT( value, max )       \
    if( value >= max )                          \
        throw ::Brimstone::DomainException();

#else  //BS_CHECK_DOMAIN

#define BS_ASSERT_DOMAIN( condition )
#define BS_ASSERT_DOMAIN_GTE( value, min )
#define BS_ASSERT_DOMAIN_GT( value, min )
#define BS_ASSERT_DOMAIN_LTE( value, max )
#define BS_ASSERT_DOMAIN_LT( value, max )

#endif //BS_CHECK_DOMAIN




#endif //BS_UTIL_MACROS_HPP